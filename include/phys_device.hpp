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

#ifndef J2f15c99194b4bc4bef27446c0d923d0
#define J2f15c99194b4bc4bef27446c0d923d0

#include "queue_family.hpp"
#include "memory_heap.hpp"
#include "memory_type.hpp"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace cu {

class Instance;
class Surface;

/*!
 * \brief A representation of a physical device available to
 * Vulkan. Used by PhysDevices.
 *
 *
 */
struct PhysDevice {
    /*!
     * \brief (constructor) PhysicalDevices is a factory for this
     * class; you may find it more convenient to use.
     */
    PhysDevice(VkPhysicalDevice                     device,
               Surface&                             surf,
               Instance::ptr                        inst,
               VkPhysicalDeviceProperties           device_props,
               VkPhysicalDeviceMemoryProperties     vk_memory_props,
               std::vector<VkQueueFamilyProperties> vk_queue_props,
               std::vector<std::string>
                   extensions_supported);

    /*
     * \brief The handle wrapped by the class.
     */
    VkPhysicalDevice dev;

    /*!
     * \brief The name reported by the device.
     */
    std::string name;

    /*!
     * \brief The device's type (i.e. discrete, integrated,
     * etc.).
     */
    VkPhysicalDeviceType type;

    /*!
     * \brief The Vulkan-encoded API version number of the version of
     * Vulkan supported by the device.
     */
    uint32_t raw_vk_ver;

    /*!
     * \brief The Vulkan vendor-specified number of the driver version
     * supported by the device.
     */
    uint32_t raw_vk_driver_ver;

    /*!
     * \brief The Vulkan UUID for the physical device vendor.
     */
    uint32_t vk_vend_id;

    /*!
     * \brief The Vulkan UUID for the physical device, compared to
     * other devices from the same vendor.
     */
    uint32_t vk_vend_dev_id;

    /*!
     * \brief The amount of video memory available to the device
     * in bytes.
     */
    VkDeviceSize mem;

    /*!
     * \brief The Vulkan types of memory available to the device.
     */
    std::vector<MemoryType> mem_types;

    /*!
     * \brief The Vulkan heaps of memory available to the device.
     */
    std::vector<MemoryHeap> mem_heaps;

    /*!
     * \brief The extensions supported by the device.
     */
    std::vector<std::string> extensions;

    /*!
     * \brief The queue families available on the device.
     */
    std::vector<QueueFamily> queue_families;

    /*!
     * \copydoc Instance::inner()
     */
    VkPhysicalDevice inner() { return dev; }

    /*!
     * \brief A Vulkan API version number.
     */
    struct VkVersionNumber {
            uint32_t major;
            uint32_t minor;
            uint32_t patch;

            /*!
             * \brief String representation.
             */
            std::string str() const;
    };

    /*!
     * \brief The number of the Vulkan API version supported by the
     * device, as a convenient struct.
     */
    VkVersionNumber vk_ver_spprtd() const;

    /*!
     * \brief The amount of video memory available to the device,
     * as a string.
     */
    std::string mem_str() const;

    /*!
     * \brief Log a representation of this device.
     */
    void log() const;

private:

    void get_queue_fams(std::vector<VkQueueFamilyProperties>&
                           queue_fam_props,
                       Surface& surf,
                       Instance::ptr inst);

    void populate_mem_props(const VkPhysicalDeviceMemoryProperties&
                                props);

private:
    PFN_vkGetPhysicalDeviceFeatures2 get_phys_dev_ftrs;

public:
    VkPhysicalDeviceMaintenance4Features maintenance4  = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES
    };

public:
    VkPhysicalDeviceFeatures2 features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
    };
};

} // namespace cu

#endif
