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

#ifndef rbd14ff256e547b690e922fd8ac2dbb9
#define rbd14ff256e547b690e922fd8ac2dbb9

#include "instance.hpp"

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "phys_device.hpp"

namespace cu {

class Surface;

/*!
 * \brief Retrieves the physical devices visible to Vulkan.
 */
class PhysDevices {
public:
    /*!
     * \brief (constructor)
     *
     * \param inst The Instance in use.
     * \param surf The Surface in use.
     */
    PhysDevices(Instance::ptr inst, Surface& surf);

    /*!
     * \brief Returns the default physical device, chosen
     * according to a set of heuristics.
     */
    PhysDevice& default_device() { return devs.at(default_dev); }

private:
    std::vector<PhysDevice> devs;
    std::vector<PhysDevice>::size_type default_dev;

    void populate_devs(Instance::ptr, Surface&);
    void populate_default();
    uint32_t get_dev_cnt(Instance::ptr inst);
    std::vector<std::string> get_dev_exts(VkPhysicalDevice dev);
    std::vector<VkPhysicalDevice> enumerate_devs(Instance::ptr inst,
                                                 uint32_t dev_cnt);
    std::vector<VkQueueFamilyProperties>
    get_queue_fam_props(VkPhysicalDevice& dev);
    VkPhysicalDeviceProperties2 get_dev_props(VkPhysicalDevice& dev);
    VkPhysicalDeviceMemoryProperties get_mem_props(VkPhysicalDevice& dev);

    PFN_vkEnumeratePhysicalDevices enum_phys_devs;
    PFN_vkGetPhysicalDeviceProperties2 get_phys_dev_props;
    PFN_vkGetPhysicalDeviceMemoryProperties get_phys_dev_mem_props;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties get_phys_dev_queue_fam_props;
    PFN_vkEnumerateDeviceExtensionProperties enum_dev_ext_props;
};

} // namespace cu

#endif
