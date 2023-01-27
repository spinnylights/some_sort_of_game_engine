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

#ifndef t5366c1c045740cebfcc013c292e1bbc
#define t5366c1c045740cebfcc013c292e1bbc

#include "phys_device.hpp"
#include "log.hpp"

namespace cu {

class Device;
class Image;

/*!
 * \brief An interface to "graphics memory."
 *
 * A Heap provides an interface to allocate memory on the graphics device, or at
 * least in a way that the graphics device can make use of. In most cases, you
 * won't need to create or manage one directly; you can use the Device interface
 * to allocate memory for e.g. an Image. In many ways it's reasonable to think
 * of this class as an implementation detail of Device.
 *
 * If you do create a Heap directly, note that it doesn't follow RAII; you have
 * to explicitly construct it using construct() and free it using the function
 * free_self(). The Device takes care of this automatically for its memory.
 */
class Heap {
public:
    using handle_t = uint64_t;
    static constexpr handle_t null_handle = 0;

    void construct(Device& l_dev, PhysDevice ph_dev);

    handle_t alloc_on_dev(Device& dev, Image& img);

    /*!
     * \brief Free the memory associated with h. If h is Heap::null_handle, does
     * nothing.
     */
    void release(handle_t h);

    void free_self(Device& dev) noexcept;

private:
    PhysicalHeap largest_dev_heap;

private:
    struct Block {
        VkDeviceSize sz;
        VkDeviceSize offset;
        handle_t handle = null_handle;
        Block* prv;
        Block* nxt;
        bool avail = true;
        bool front = false;

        VkDeviceSize end() const { return offset + sz; }

        void insert_before(Block*);
        void erase();
        void log_attrs();
    };

    struct Pool {
        VkDeviceMemory nner;
        VkDeviceSize   sz;
        MemoryType     type;
        Block* blocks;
        handle_t next_handle = null_handle + 1;
        bool should_destroy = false;

        Pool() = default;
        Pool(VkDeviceSize size, MemoryType mem_type);

        Pool(const Pool&) = delete;
        Pool& operator=(const Pool&) = delete;

        Pool(Pool&&);
        Pool& operator=(Pool&&);

        ~Pool() noexcept;

        Block* reserve_space(VkDeviceSize sz, VkDeviceSize alignment);
        void release(handle_t h);
    };

    Pool main_pool;

private:
    PFN_vkAllocateMemory  alloc_mem;
    PFN_vkFreeMemory      free_mem;
    PFN_vkBindImageMemory bind_img_mem;
};

} // namespace cu

#endif
