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

#include "device.hpp"

#include "log.hpp"

#include "vulkan.hpp"

#include <iostream>
#include <functional>
#include <algorithm>
#include <set>
#include <unordered_map>

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

Device::Device(PhysDevice& phys_dev, Instance::ptr inst)
    :create_dev {
        reinterpret_cast<PFN_vkCreateDevice>(
            inst->get_proc_addr("vkCreateDevice")
        )
     },
     get_dev_proc_addr {
         reinterpret_cast<PFN_vkGetDeviceProcAddr>(
             inst->get_proc_addr("vkGetDeviceProcAddr")
         )
     }

{
    using queue_map =
        std::unordered_map<std::string, std::tuple<VkQueue*,uint32_t>>;

#define GET_QUEUE_DAT(name) \
    {\
        #name,\
        {\
            &name,\
            pick_queue_ndx(phys_dev, [](QueueFamily& q) { return q.name(); })\
        }\
    }

    queue_map to_queues = {
        GET_QUEUE_DAT(graphics),
        GET_QUEUE_DAT(compute),
        GET_QUEUE_DAT(present),
        GET_QUEUE_DAT(transfer),
    };

    std::set<uint32_t> indices_to_create;
    for (const auto& [_, t] : to_queues) {
        indices_to_create.insert(std::get<uint32_t>(t));
    }

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

    for (auto&& n : ext_names) {
        std::string ext {n};
        if (std::find(begin(phys_dev.extensions),
                      end(phys_dev.extensions),
                      ext) == std::end(phys_dev.extensions)) {
            throw std::runtime_error("device extension "
                                     + ext
                                     + " is not available!");
        }
    }

    VkPhysicalDeviceFeatures2 dev_ftrs = phys_dev.features;
    VkPhysicalDeviceMaintenance4Features maint4_ftrs = phys_dev.maintenance4;
    dev_ftrs.pNext = &maint4_ftrs;

    VkDeviceCreateInfo dev_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &dev_ftrs,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
        .pQueueCreateInfos = queue_infos.data(),
        .enabledLayerCount = 0, // deprecated + ignored
        .ppEnabledLayerNames = NULL, // deprecated + ignored
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

    for (const auto& [_, t] : to_queues) {
        get_dev_queue(dev, std::get<uint32_t>(t), 0, std::get<VkQueue*>(t));
    }

    log.indent();
    for (const auto& [name, t] : to_queues) {
        log.enter(name + " queue index", std::get<uint32_t>(t));
    }
    log.brk();
}

Device::~Device() noexcept
{
    log.attempt("Vulkan", "destroying logical device");
    destroy_dev(dev, NULL);
    log.finish();
    log.brk();
}

PFN_vkVoidFunction Device::get_proc_addr(const char* name)
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
