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

#include "phys_devices.hpp"

#include "log.hpp"

#include "vulkan.hpp"

#include <stdexcept>
#include <iostream>

namespace cu {


uint32_t PhysDevices::get_dev_cnt(Instance& inst)
{
    uint32_t dev_cnt;
    Vulkan::vk_try(enum_phys_devs(inst.inner(), &dev_cnt, NULL),
                   "getting physical device count");
    log.indent();
    log.enter("physical device count", dev_cnt);
    log.brk();

    return dev_cnt;
}

std::vector<VkPhysicalDevice> PhysDevices::enumerate_devs(Instance& inst,
                                                          uint32_t dev_cnt)
{
    std::vector<VkPhysicalDevice> potential_devs (dev_cnt);
    Vulkan::vk_try(enum_phys_devs(inst.inner(),
                                  &dev_cnt,
                                  potential_devs.data()),
                   "enumerating physical devices");
    log.brk();

    return potential_devs;
}

VkPhysicalDeviceProperties2 PhysDevices::get_dev_props(VkPhysicalDevice& dev)
{
    VkPhysicalDeviceProperties2 props = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
    };
    get_phys_dev_props(dev, &props);

    return props;
}

VkPhysicalDeviceMemoryProperties2
PhysDevices::get_mem_props(VkPhysicalDevice& dev)
{
    VkPhysicalDeviceMemoryProperties2 mem_props = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2
    };
    get_phys_dev_mem_props(dev, &mem_props);

    return mem_props;
}

VkDeviceSize calc_total_mem(VkPhysicalDeviceMemoryProperties2& mem_props)
{
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

    return total_mem;
}

std::vector<VkQueueFamilyProperties>
PhysDevices::get_queue_fam_props(VkPhysicalDevice& dev)
{
    uint32_t q_family_cnt;
    get_phys_dev_queue_fam_props(dev, &q_family_cnt, NULL);
    std::vector<VkQueueFamilyProperties> q_family_props (q_family_cnt);
    get_phys_dev_queue_fam_props(dev, &q_family_cnt, q_family_props.data());

    return q_family_props;
}

void log_dev(VkPhysicalDeviceProperties2& props,
             VkDeviceSize total_mem,
             const std::vector<std::string>& exts)
{
    log.enter_obj({
        .name = std::string{props.properties.deviceName},
        .members {
            {"api version",
                Vulkan::api_ver_to_str(props.properties.apiVersion)},
            {"driver version",
                props.properties.driverVersion},
            {"vendor ID", props.properties.vendorID},
            {"device ID", props.properties.deviceID},
            {"device type", props.properties.deviceType},
            {"video memory", std::to_string(total_mem) + " bytes"},
            {"extensions", exts}
        }
    });

    log.brk();
}

void log_queue_fam(VkQueueFamilyProperties& q_fam_props, uint32_t ndx)
{
    log.indent(2);
    log.enter_obj({
        .name = "queue family " + std::to_string(ndx),
        .members {
            {"flags", q_fam_props.queueFlags},
            {"queue count",
                q_fam_props.queueCount},
            {"timestamp valid bits",
                q_fam_props.timestampValidBits},
        }
    });
    log.brk();

    log.indent(3);
    log.enter_obj({
        .name = "min. img. transfer granularity",
        .members {
            {"width", q_fam_props.minImageTransferGranularity.width},
            {"height", q_fam_props.minImageTransferGranularity.height},
            {"depth", q_fam_props.minImageTransferGranularity.depth},
        }
    });
    log.brk();
}

std::vector<std::string> PhysDevices::get_dev_exts(VkPhysicalDevice dev)
{
        uint32_t ext_cnt;
        Vulkan::vk_try(enum_dev_ext_props(dev, NULL, &ext_cnt, NULL),
                       "get device extensions count");
        log.indent();
        log.enter("device extensions count", ext_cnt);
        log.brk();

        std::vector<VkExtensionProperties> ext_props (ext_cnt);
        Vulkan::vk_try(enum_dev_ext_props(dev,
                                          NULL,
                                          &ext_cnt,
                                          ext_props.data()),
                       "get device extensions");
        log.brk();

        std::vector<std::string> ext_names;
        std::transform(begin(ext_props),
                       end(ext_props),
                       std::back_inserter(ext_names),
                       [](VkExtensionProperties& ep) -> std::string {
                           return std::string{ep.extensionName};
                       });

        return ext_names;
}

void PhysDevices::populate_devs(Instance& inst, Surface& surf)
{
    auto dev_cnt        = get_dev_cnt(inst);
    auto potential_devs = enumerate_devs(inst, dev_cnt);

    for (auto potential_dev : potential_devs) {
        auto props     = get_dev_props(potential_dev);
        auto mem_props = get_mem_props(potential_dev);
        auto total_mem = calc_total_mem(mem_props);
        auto exts      = get_dev_exts(potential_dev);

        PhysDevice phys_dev {
            .dev        = potential_dev,
            .name       = props.properties.deviceName,
            .type       = props.properties.deviceType,
            .mem        = total_mem,
            .extensions = exts,
        };

        log_dev(props, total_mem, exts);

        auto q_family_props = get_queue_fam_props(potential_dev);

        bool supports_graphics = false;
        bool supports_present = false;
        for (uint32_t i = 0; i < q_family_props.size(); ++i) {
            QueueFamily fam = {
                q_family_props.at(i),
                i,
                potential_dev,
                surf,
                inst
            };

            if (fam.graphics()) {
                supports_graphics = true;
            }

            if (fam.present_supported()) {
                supports_present = true;
            }

            phys_dev.queue_families.push_back(fam);

            log.indent(2);
            fam.log_info();
        }

        if (supports_graphics && supports_present) {
            devs.push_back(phys_dev);
        }
    }

    log.brk();
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
            auto& dev = devs.at(i);
            if (   (   dev.type == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
                    && type     != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)

                || (   dev.type == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
                    && type     == VK_PHYSICAL_DEVICE_TYPE_CPU)

                || (dev.mem > mem && dev.type == type)
               ) {
                ndx = i;
                mem = dev.mem;
                type = dev.type;
            }
        }

        default_dev = ndx;
    }

    log.enter("default phys. device", default_dev);
    log.brk();
}

PhysDevices::PhysDevices(Instance& inst, Surface& surf)
    :enum_phys_devs{
         reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(
             inst.get_proc_addr("vkEnumeratePhysicalDevices")
         )
     },
     get_phys_dev_props{
         reinterpret_cast<PFN_vkGetPhysicalDeviceProperties2>(
             inst.get_proc_addr("vkGetPhysicalDeviceProperties2")
         )
     },
     get_phys_dev_mem_props{
         reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties2>(
             inst.get_proc_addr("vkGetPhysicalDeviceMemoryProperties2")
         )
     },
     get_phys_dev_queue_fam_props{
         reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(
             inst.get_proc_addr("vkGetPhysicalDeviceQueueFamilyProperties")
         )
     },
     enum_dev_ext_props{
         reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(
             inst.get_proc_addr("vkEnumerateDeviceExtensionProperties")
         )
     }
{
    populate_devs(inst, surf);
    populate_default();
}

} // namespace cu
