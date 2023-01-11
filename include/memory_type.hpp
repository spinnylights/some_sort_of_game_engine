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

#ifndef dd6a4958fb5546e0898682fd102be3cc
#define dd6a4958fb5546e0898682fd102be3cc

#include <vulkan/vulkan.h>

#include <cstddef>

namespace cu {

/*!
 * \brief A Vulkan memory type wrapper.
 */
struct MemoryType {
    VkMemoryType inner;
    std::size_t  _ndx;

    /*!
     * \brief The Vulkan index of the memory type.
     */
    uint32_t ndx() const
    {
        return static_cast<uint32_t>(_ndx);
    }

    /*!
     * \brief The Vulkan index of the heap from which this type of memory
     * emerges (as if from a bog).
     */
    std::size_t heap_ndx() const
    {
        return inner.heapIndex;
    }

    /*!
     * \brief Whether this type of memory is found on the Vulkan \ref PhysDevice
     * "physical device" or not.
     */
    bool device_local() const
    {
        return   inner.propertyFlags
               & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }

    /*!
     * \brief Whether this type of memory is visible to the host (i.e. can be
     * read from and written to through a pointer on the host side).
     */
    bool host_visible() const
    {
        return   inner.propertyFlags
               & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }

    /*!
     * \brief Whether the state of this memory always looks the same from the
     * device and host "perspective" (i.e. manual cache management from the host
     * side isn't needed).
     */
    bool host_coherent() const
    {
        return   inner.propertyFlags
               & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }

    /*!
     * \brief Whether this type of memory is cached on the host. Host-cached
     * memory is faster to access from the host side than uncached memory as you
     * would hope. However, you may need to manually manage the cache
     * (flushing/invalidating) when accessing memory of this type from the host
     * side.
     */
    bool host_cached() const
    {
        return   inner.propertyFlags
               & VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    }
};

} // namespace cu

#endif
