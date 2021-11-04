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

#include "queue_family.hpp"

#include "log.hpp"

#include "surface.hpp"
#include "vulkan.hpp"

namespace cu {

QueueFamily::QueueFamily(VkQueueFamilyProperties& q_family_props,
                         uint32_t ndex,
                         VkPhysicalDevice dev,
                         Surface& surf,
                         Instance::ptr inst)
    :get_surf_support{
        reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(
            inst->get_proc_addr("vkGetPhysicalDeviceSurfaceSupportKHR")
        )
     }
{
    ndx = ndex;

    queue_cnt = q_family_props.queueCount;

    VkBool32 pres_supported;
    get_surf_support(dev, ndx, surf.inner(), &pres_supported);
    pres_support = Vulkan::vkbool_to_bool(pres_supported);

    flags = q_family_props.queueFlags;
}

bool QueueFamily::graphics() const
{
    return flags & VK_QUEUE_GRAPHICS_BIT;
}

bool QueueFamily::compute() const
{
    return flags & VK_QUEUE_COMPUTE_BIT;
}

bool QueueFamily::transfer() const
{
    return flags & VK_QUEUE_TRANSFER_BIT;
}

bool QueueFamily::sparse_binding() const
{
    return flags & VK_QUEUE_SPARSE_BINDING_BIT;
}

bool QueueFamily::protected_memory() const
{
    return flags & VK_QUEUE_PROTECTED_BIT;
}

uint32_t QueueFamily::flag_count() const
{
    constexpr uint32_t flags_to_count = 5;

    uint32_t count = 0;
    VkQueueFlags scratch = flags;

    for (uint32_t i = 0; i < flags_to_count; ++i) {
        count += scratch & 1;
        scratch >>= 1;
    }

    return count;
}

void QueueFamily::log_info() const
{
    log.enter({
        .name = "queue family " + std::to_string(index()),
        .members = {
            {"graphics", graphics()},
            {"compute", compute()},
            {"transfer", transfer()},
            {"sparse binding", sparse_binding()},
            {"protected memory", protected_memory()},
            {"present supported", present_supported()},
            {"queue count", queue_count()},
        }
    });

    log.brk();
}

} // namespace cu
