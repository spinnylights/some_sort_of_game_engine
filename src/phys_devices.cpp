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

#include <stdexcept>
#include <iostream>

#include "vulkan.hpp"

#include "phys_devices.hpp"

namespace cu {

void PhysDevices::populate_devs(Instance& inst, Surface& surf)
{
    uint32_t dev_cnt;
    Vulkan::vk_try(vkEnumeratePhysicalDevices(inst.inner(), &dev_cnt, NULL),
                   "get physical device count");

    std::vector<VkPhysicalDevice> potential_devs (dev_cnt);
    Vulkan::vk_try(vkEnumeratePhysicalDevices(inst.inner(),
                                              &dev_cnt,
                                              potential_devs.data()),
                   "enumerate physical devices");

    for (auto potential_dev : potential_devs) {
        VkPhysicalDeviceProperties2 props = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        };
        vkGetPhysicalDeviceProperties2(potential_dev, &props);

        VkPhysicalDeviceMemoryProperties2 mem_props = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2
        };
        vkGetPhysicalDeviceMemoryProperties2(potential_dev, &mem_props);
        VkDeviceSize total_mem = 0;
        for (std::size_t i = 0;
             i < mem_props.memoryProperties.memoryHeapCount;
             ++i) {
            VkMemoryHeap heap = mem_props.memoryProperties.memoryHeaps[i];
            if ((heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                total_mem += heap.size;
            }
        }

        PhysDevice phys_dev {
            .inner = potential_dev,
            .name  = props.properties.deviceName,
            .type  = props.properties.deviceType,
            .mem   = total_mem
        };

        uint32_t q_family_cnt;
        vkGetPhysicalDeviceQueueFamilyProperties(potential_dev,
                                                 &q_family_cnt,
                                                 NULL);
        std::vector<VkQueueFamilyProperties> q_family_props (q_family_cnt);
        vkGetPhysicalDeviceQueueFamilyProperties(potential_dev,
                                                 &q_family_cnt,
                                                 q_family_props.data());

        for (uint32_t i = 0; i < q_family_props.size(); ++i) {
            phys_dev.queue_families.push_back({
                q_family_props.at(i),
                i,
                potential_dev,
                surf
            });
        }

        devs.push_back(phys_dev);
    }
}

void PhysDevices::populate_default()
{
    if (devs.size() == 0) {
        throw std::runtime_error("no Vulkan-capable devices found!");
    } else if (devs.size() == 1) {
        default_dev = 0;
    } else {
        std::vector<PhysDevice>::size_type ndx = 0;
        VkDeviceSize mem = 0;
        VkPhysicalDeviceType type = VK_PHYSICAL_DEVICE_TYPE_CPU;
        for (std::size_t i = 0; i < devs.size(); ++i) {
            if (devs.at(i).type == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
                && type != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                ndx = i;
                mem = devs.at(i).mem;
                type = devs.at(i).type;
            } else if (devs.at(i).type == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
                       && type == VK_PHYSICAL_DEVICE_TYPE_CPU) {
                ndx = i;
                mem = devs.at(i).mem;
                type = devs.at(i).type;
            } else if (devs.at(i).mem > mem && devs.at(i).type == type) {
                ndx = i;
                mem = devs.at(i).mem;
                type = devs.at(i).type;
            }
        }

        default_dev = ndx;
    }
}

PhysDevices::PhysDevices(Instance& inst, Surface& surf)
{
    populate_devs(inst, surf);
    populate_default();
}

} // namespace cu
