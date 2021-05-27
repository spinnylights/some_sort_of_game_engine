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

#include "logi_device.hpp"

#include "log.hpp"

#include "vulkan.hpp"

#include <iostream>
#include <functional>
#include <set>

namespace cu {

uint32_t pick_queue_ndx(PhysDevice& phys_dev,
                        std::function<bool(QueueFamily&)> criterion)
{
    uint32_t ndx = 0;
    uint32_t flag_cnt = UINT32_MAX;

    for (auto& fam : phys_dev.queue_families) {
        uint32_t this_flag_cnt = fam.flag_count();

        if (criterion(fam) && this_flag_cnt < flag_cnt) {
            ndx = fam.index();
            flag_cnt = this_flag_cnt;
        }
    }

    return ndx;
}

LogiDevice::LogiDevice(PhysDevice& phys_dev, Instance& inst)
    :create_dev {
        reinterpret_cast<PFN_vkCreateDevice>(
            inst.get_proc_addr("vkCreateDevice")
        )
     },
     get_dev_proc_addr {
         reinterpret_cast<PFN_vkGetDeviceProcAddr>(
             inst.get_proc_addr("vkGetDeviceProcAddr")
         )
     }

{
    uint32_t graphics_ndx = pick_queue_ndx(phys_dev,
                                           [](QueueFamily& q) {
                                               return q.graphics();
                                           });

    uint32_t present_ndx = pick_queue_ndx(phys_dev,
                                           [](QueueFamily& q) {
                                               return q.present_supported();
                                           });

    std::set<uint32_t> indices_to_create;
    indices_to_create.insert(graphics_ndx);
    indices_to_create.insert(present_ndx);

    std::vector<VkDeviceQueueCreateInfo> queue_infos;
    const float queue_priority = 1.0;
    for (auto& i : indices_to_create) {
        queue_infos.push_back({
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .queueFamilyIndex = i,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
        });
    }

    const char* const ext_names[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkDeviceCreateInfo dev_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
        .pQueueCreateInfos = queue_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = ext_names,
        .pEnabledFeatures = NULL,
    };

    Vulkan::vk_try(create_dev(phys_dev.inner(), &dev_info, NULL, &dev),
                   "create logical device");

    get_dev_queue = {
        reinterpret_cast<PFN_vkGetDeviceQueue>(
            get_proc_addr("vkGetDeviceQueue")
        )
    };

    destroy_dev = {
       reinterpret_cast<PFN_vkDestroyDevice>(
           get_proc_addr("vkDestroyDevice")
       )
    };

    get_dev_queue(dev, graphics_ndx, 0, &graphics);
    get_dev_queue(dev, present_ndx, 0, &present);

    log.indent();
    log.enter("graphics queue index", graphics_ndx);
    log.enter("present queue index", present_ndx);
    log.brk();
}

LogiDevice::~LogiDevice() noexcept
{
    log.attempt("Vulkan", "destroying logical device");
    destroy_dev(dev, NULL);
    log.finish();
    log.brk();
}

PFN_vkVoidFunction LogiDevice::get_proc_addr(const char* name)
{
    auto fn = get_dev_proc_addr(dev, name);

    if (fn == NULL) {
        throw std::runtime_error("device function "
                                 + std::string{name}
                                 + " is not available!");
    }

    return fn;
}

} // namespace cu
