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

LogiDevice::LogiDevice(PhysDevice& phys_dev)
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

    VkDeviceCreateInfo dev_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
        .pQueueCreateInfos = queue_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = NULL,
        .pEnabledFeatures = NULL,
    };

    Vulkan::vk_try(vkCreateDevice(phys_dev.inner(), &dev_info, NULL, &dev),
                   "create logical device");

    vkGetDeviceQueue(dev, graphics_ndx, 0, &graphics);
    vkGetDeviceQueue(dev, present_ndx, 0, &present);

    log.indent();
    log.enter("graphics queue index: " + std::to_string(graphics_ndx));
    log.enter("present queue index: " + std::to_string(present_ndx));
    log.brk();
}

LogiDevice::~LogiDevice()
{
    log.attempt("Vulkan: destroying logical device");
    vkDestroyDevice(dev, NULL);
    log.finish();
    log.brk();
}

} // namespace cu
