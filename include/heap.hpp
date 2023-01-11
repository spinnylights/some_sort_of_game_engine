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
#include "device.hpp"
#include "image.hpp"
#include "log.hpp"

#include <list>

namespace cu {

/*!
 * \brief An interface to "graphics memory."
 *
 * A Heap provides an interface to allocate memory on the graphics device, or at
 * least in a way that the graphics device can make use of.
 */
class Heap {
public:
    Heap(Device::ptr l_dev, PhysDevice ph_dev);

    void alloc_on_dev(Image& img);

    ~Heap() noexcept;

private:
    Device::ptr dev;

private:
    PhysDevice phys_dev;

private:
    PhysicalHeap largest_dev_heap;

private:
    struct Block {
        VkDeviceSize sz;
        VkDeviceSize offset;
        bool avail = true;

        VkDeviceSize end() const { return offset + sz; }
    };

    struct Pool {
        VkDeviceMemory nner;
        VkDeviceSize   sz;
        MemoryType     type;
        std::list<Block> blocks;

        Pool(VkDeviceSize size, MemoryType mem_type);

        Block reserve_space(VkDeviceSize sz, VkDeviceSize alignment);
    };

    Pool main_pool;

private:
    PFN_vkAllocateMemory  alloc_mem;
    PFN_vkFreeMemory      free_mem;
    PFN_vkBindImageMemory bind_img_mem;

};

} // namespace cu

#endif
