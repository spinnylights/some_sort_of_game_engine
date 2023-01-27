/*
 * This file is part of Crypt Underworld.
 *
 * Crypt Underworld is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * Crypt Underworld is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Crypt Underworld. If not, see
 * <https://www.gnu.org/licenses/>.
 *
 * Copyright (c) 2023 Zoë Sparks <zoe@milky.flowers>
 */

#include "heap.hpp"
#include "iec_ibyte.hpp"
#include "vulkan.hpp"

namespace cu {

void Heap::construct(Device& dev, PhysDevice ph_dev)
{
    largest_dev_heap = ph_dev.largest_dev_local_heap();

    main_pool = Pool (
        largest_dev_heap.size() > 1_GiB ?
            256_MiB : largest_dev_heap.size() / 8,
        largest_dev_heap.optimal_type()
    );

    alloc_mem = reinterpret_cast<PFN_vkAllocateMemory>(
        dev.get_proc_addr("vkAllocateMemory")
    );

    free_mem = reinterpret_cast<PFN_vkFreeMemory>(
        dev.get_proc_addr("vkFreeMemory")
    );

    bind_img_mem = reinterpret_cast<PFN_vkBindImageMemory>(
        dev.get_proc_addr("vkBindImageMemory")
    );

    VkMemoryAllocateInfo main_pool_alloc_inf {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = NULL,
        .allocationSize  = main_pool.sz,
        .memoryTypeIndex = main_pool.type.ndx(),
    };

    Vulkan::vk_try(alloc_mem(dev.inner(),
                             &main_pool_alloc_inf,
                             NULL,
                             &main_pool.nner),
                   "allocating main pool memory");
    log.indent();
    log.enter("heap", largest_dev_heap.ndx());
    log.enter("type", main_pool.type.ndx());
    log.enter("amount", main_pool.sz);
    log.brk();
}

void Heap::free_self(Device& dev) noexcept
{
    log.attempt("Vulkan", "freeing main pool");
    free_mem(dev.inner(), main_pool.nner, NULL);
    log.finish();
    log.brk();
}

Heap::Pool::Pool(VkDeviceSize size, MemoryType mem_type)
    : sz {size},
      type {mem_type}
{
    blocks = new Block { .front = true };
    blocks->nxt = new Block { .sz = sz, .offset = 0 };
    blocks->prv = blocks->nxt;
    blocks->nxt->prv = blocks;
    blocks->nxt->nxt = blocks;

    should_destroy = true;
}

Heap::Pool::Pool(Heap::Pool&& other)
    : nner        {other.nner},
      sz          {other.sz},
      type        {other.type},
      blocks      {other.blocks},
      next_handle {other.next_handle}
{
    other.should_destroy = false;
}

Heap::Pool& Heap::Pool::operator=(Heap::Pool&& other)
{
    std::swap(nner, other.nner);
    std::swap(sz, other.sz);
    std::swap(type, other.type);
    blocks = other.blocks;
    other.blocks = nullptr;
    std::swap(next_handle, other.next_handle);

    other.should_destroy = false;

    return *this;
}

Heap::Pool::~Pool() noexcept
{
    if (should_destroy) {
        Block* p = blocks->nxt;
        while (!p->front) {
            Block* tmp = p->nxt;
            p->erase();
            p = tmp;
        }
        p->erase();
    }
}

void Heap::Block::insert_before(Block* p)
{
    Block* tmp = prv;
    prv = p;
    tmp->nxt = p;
    p->prv = tmp;
    p->nxt = this;
}

void Heap::Block::erase()
{
    prv->nxt = nxt;
    nxt->prv = prv;
    log.enter("Vulkan", std::string("erasing block"));
    log_attrs();
    log.brk();
    delete this;
}

void Heap::Block::log_attrs()
{
    log.indent();
    log.enter("sz", sz);
    log.enter("offset", offset);
    log.enter("handle", handle);
    log.enter("avail", static_cast<int>(avail));
    log.enter("front", static_cast<int>(front));
}

Heap::Block* Heap::Pool::reserve_space(VkDeviceSize sz, VkDeviceSize alignment)
{
    // [{.sz = 128, .offset = 0, .avail = true}]
    //
    // need {.sz = 37, .offset = 16}
    //
    // [1: {.sz = 37, .offset =  0, .avail = false}
    //  2: {.sz = 91, .offset = 37, .avail = true}]
    //
    // need {.sz = 20, .offset = 16}
    //
    // start = 1.sz + (need.offset - (1.sz % need.offset)) = 48
    // end   = start + need.offset = 68
    //
    // 1: not avail
    // 2: 91(2.sz) > 20(need.sz) && 128(2.end) > 68(need.end)
    //
    // [1: {.sz = 37, .offset =  0, .avail = false}
    //  2: {.sz = 11, .offset = 37, .avail = true}
    //  3: {.sz = 20, .offset = 48, .avail = false}
    //  4: {.sz = 60, .offset = 68, .avail = true}]

    VkDeviceSize start = 0;
    VkDeviceSize end = 0;
    auto find_free = [sz, alignment, &start, &end](Block* b)
    {
        if (b->avail) {
            if (b->offset % alignment) {
                start = b->offset + (alignment - (b->offset % alignment));
            } else {
                start = b->offset;
            }

            end = start + sz;

            return b->sz > sz && b->end() > end;
        } else {
            return false;
        }
    };

    log.attempt("Vulkan", "finding free block in main pool");
    Block* free_b = blocks->nxt;
    for (; !free_b->front && find_free(free_b); free_b = free_b->nxt) {
        if (free_b->front) {
            throw std::runtime_error("no space left in main pool "
                                     "(PLACEHOLDER: reserve more memory)");
            log.finish();
        }
    }

    log.indent();

    Block* reserved = new Block {
        .sz     = sz,
        .offset = start,
        .handle = next_handle++,
        .avail  = false,
    };

    if (next_handle == null_handle) {
        next_handle = 1;
    }

    if (free_b->offset < start) {
        Block* before = new Block {
            .sz     = start - free_b->offset,
            .offset = free_b->offset,
            .avail  = true,
        };

        log.enter({
            .name = "before block",
            .members = {
                {"size", before->sz},
                {"offset", before->offset},
            }
        });

        free_b->insert_before(before);
    }

    free_b->insert_before(reserved);

    log.enter({
        .name = "reserved block",
        .members = {
            {"size", reserved->sz},
            {"offset", reserved->offset},
        }
    });

    if (free_b->end() > end) {
        Block* after = new Block {
            .sz     = free_b->end() - end,
            .offset = end,
            .avail  = true,
        };

        log.enter({
            .name = "after block",
            .members = {
                {"size", after->sz},
                {"offset", after->offset},
            }
        });

        free_b->insert_before(after);
    }

    if (!free_b->front) free_b->erase();

    log.brk();

    return reserved;
}

Heap::handle_t Heap::alloc_on_dev(Device& dev, Image& img)
{
    if (!img.mem_type_supported(main_pool.type)) {
        throw std::runtime_error("main pool memory does not support this type "
                                 "of image");
    }

    auto block = main_pool.reserve_space(img.mem_size(), img.alignment());
    Vulkan::vk_try(bind_img_mem(dev.inner(),
                                img.inner(),
                                main_pool.nner,
                                block->offset),
                   "binding image to memory");
    log.brk();

    return block->handle;
}

void Heap::Pool::release(Heap::handle_t h)
{
    if (h == null_handle) {
        return;
    }

    Block* p = blocks->nxt;
    for (; !p->front && p->handle != h; p = p->nxt);

    if (!p->front) p->erase();
}

void Heap::release(handle_t h)
{
    main_pool.release(h);
}

} // namespace cu
