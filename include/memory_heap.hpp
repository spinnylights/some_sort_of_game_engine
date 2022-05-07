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
 * Copyright (c) 2021 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef j09deaa7dd594f34b457535d3b79dfb0
#define j09deaa7dd594f34b457535d3b79dfb0

#include <vulkan/vulkan.h>

#include <cstddef>

namespace cu {

/*!
 * \brief A Vulkan memory heap wrapper.
 */
struct MemoryHeap {
    VkMemoryHeap inner;
    std::size_t  _ndx;

    /*!
     * \brief The Vulkan index of the memory heap.
     */
    std::size_t ndx() const
    {
        return _ndx;
    }

    /*!
     * \brief The size of the heap in bytes.
     */
    std::size_t size() const
    {
        return inner.size;
    }

    /*!
     * \brief Whether the heap is on the Vulkan \ref PhysDevice
     * "physical device" or not.
     */
    bool device_local() const
    {
        return inner.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT;
    }
};


} // namespace cu

#endif
