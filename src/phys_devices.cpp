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
#include <algorithm>

namespace cu {

std::vector<VkPhysicalDevice> PhysDevices::enumerate_devs(Instance::ptr inst)
{
    uint32_t dev_cnt;
    Vulkan::vk_try(enum_phys_devs(inst->inner(), &dev_cnt, NULL),
                   "getting physical device count");
    log.indent();
    log.enter("physical device count", dev_cnt);
    log.brk();

    std::vector<VkPhysicalDevice> potential_devs (dev_cnt);
    Vulkan::vk_try(enum_phys_devs(inst->inner(),
                                  &dev_cnt,
                                  potential_devs.data()),
                   "enumerating physical devices");
    log.brk();

    return potential_devs;
}

PhysDevice::PhysDeviceProps PhysDevices::get_dev_props(VkPhysicalDevice& dev)
{
    PhysDevice::PhysDeviceProps props {};

    get_phys_dev_props(dev, &props.props);

    return props;
}

VkPhysicalDeviceMemoryProperties
PhysDevices::get_mem_props(VkPhysicalDevice& dev)
{
    VkPhysicalDeviceMemoryProperties mem_props;
    get_phys_dev_mem_props(dev, &mem_props);

    return mem_props;
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

void log_queue_fam(VkQueueFamilyProperties& q_fam_props, uint32_t ndx)
{
    log.indent(2);
    log.enter({
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
    log.enter({
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

void PhysDevices::fill_devs(std::vector<PhysDevice>& devs, bool compute_only)
{
    for (auto potential_dev : potential_devs) {
        auto q_family_props = get_queue_fam_props(potential_dev);

        PhysDevice phys_dev {
            potential_dev,
            inst,
            get_dev_props(potential_dev),
            get_mem_props(potential_dev),
            q_family_props,
            get_dev_exts(potential_dev),
        };

        phys_dev.log();

        for (const auto& fam : phys_dev.queue_families) {
            if ((compute_only && fam.compute()) || fam.graphics()) {
                devs.push_back(phys_dev);
                break;
            }
        }
    }

    log.brk();
}

void PhysDevices::fill_devs(std::vector<PhysDevice>& devs, Surface& surf)
{
    for (auto potential_dev : potential_devs) {
        auto q_family_props = get_queue_fam_props(potential_dev);

        PhysDevice phys_dev {
            potential_dev,
            inst,
            get_dev_props(potential_dev),
            get_mem_props(potential_dev),
            q_family_props,
            get_dev_exts(potential_dev),
            surf,
        };

        phys_dev.log();

        for (const auto& fam : phys_dev.queue_families) {
            if (fam.graphics() && fam.present()) {
                devs.push_back(phys_dev);
                break;
            }
        }
    }

    log.brk();
}

std::vector<PhysDevice>::size_type
PhysDevices::get_default_dev_ndx(const std::vector<PhysDevice>& devs)
{
    std::vector<PhysDevice>::size_type default_dev = 0;

    if (devs.size() == 0) {
        throw std::runtime_error("no valid devices found!");
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

    return default_dev;
}

PhysDevice PhysDevices::default_device(bool compute_only)
{
    std::vector<PhysDevice> devs {};

    fill_devs(devs, compute_only);

    return devs.at(get_default_dev_ndx(devs));
}

PhysDevice PhysDevices::default_device(Surface& surf)
{
    std::vector<PhysDevice> devs {};

    fill_devs(devs, surf);

    return devs.at(get_default_dev_ndx(devs));
}

PhysDevices::PhysDevices(Instance::ptr instn)
    :enum_phys_devs{
         reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(
             instn->get_proc_addr("vkEnumeratePhysicalDevices")
         )
     },
     get_phys_dev_props{
         reinterpret_cast<PFN_vkGetPhysicalDeviceProperties2>(
             instn->get_proc_addr("vkGetPhysicalDeviceProperties2")
         )
     },
     get_phys_dev_mem_props{
         reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>(
             instn->get_proc_addr("vkGetPhysicalDeviceMemoryProperties")
         )
     },
     get_phys_dev_queue_fam_props{
         reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(
             instn->get_proc_addr("vkGetPhysicalDeviceQueueFamilyProperties")
         )
     },
     enum_dev_ext_props{
         reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(
             instn->get_proc_addr("vkEnumerateDeviceExtensionProperties")
         )
     },
     potential_devs(enumerate_devs(instn)),
     inst {instn}
{}

} // namespace cu
