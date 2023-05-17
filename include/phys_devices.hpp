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
     */
    PhysDevices(Instance::ptr instn);

    /*!
     * \brief Returns the default physical device, chosen according to a set of
     * heuristics.
     *
     * This function assumes that you won't be using the PhysDevice to present
     * to a Surface. Use the default_device(Surface&) form if you do plan to
     * present to surface.
     *
     * \param compute_only If you enable this, this function will only check
     * each PhysDevice for compute support, ignoring whether or not it supports
     * graphics operations (which it would otherwise check for). The PhysDevice
     * you get back may support graphics operations anyway. (Note that if a
     * physical device does support graphics operations, the Vulkan standard
     * requires it to support compute operations as well).
     */
    PhysDevice default_device(bool compute_only = false);

    /*!
     * \copybrief PhysDevice::PhysDevice(bool)
     *
     * This function will query each PhysDevice for present support to the given
     * Surface, as well as support for graphics and compute operations. If no
     * such devices are found, it will throw an excpetion.
     *
     * \param surf The Surface you plan to present to with the PhysDevice.
     */
    PhysDevice default_device(Surface& surf);

private:
    PhysDevices();

    std::vector<std::string> get_dev_exts(VkPhysicalDevice dev);
    std::vector<VkPhysicalDevice> enumerate_devs(Instance::ptr inst);
    std::vector<VkQueueFamilyProperties>
        get_queue_fam_props(VkPhysicalDevice& dev);
    PhysDevice::PhysDeviceProps get_dev_props(VkPhysicalDevice& dev);
    VkPhysicalDeviceMemoryProperties get_mem_props(VkPhysicalDevice& dev);
    void fill_devs(std::vector<PhysDevice>& devs, bool compute_only);
    void fill_devs(std::vector<PhysDevice>& devs, Surface& surf);
    std::vector<PhysDevice>::size_type
        get_default_dev_ndx(const std::vector<PhysDevice>& devs);

    PFN_vkEnumeratePhysicalDevices enum_phys_devs;
    PFN_vkGetPhysicalDeviceProperties2 get_phys_dev_props;
    PFN_vkGetPhysicalDeviceMemoryProperties get_phys_dev_mem_props;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties get_phys_dev_queue_fam_props;
    PFN_vkEnumerateDeviceExtensionProperties enum_dev_ext_props;

    std::vector<VkPhysicalDevice> potential_devs;
    Instance::ptr inst;

};

} // namespace cu

#endif
