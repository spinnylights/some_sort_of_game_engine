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

#ifndef xe47e6d7980140aa9940894be9435fc2
#define xe47e6d7980140aa9940894be9435fc2

#include "instance.hpp"
#include "phys_device.hpp"
#include "memory_heap.hpp"

#include <vulkan/vulkan.h>

namespace cu {

/*!
 * \brief A Vulkan logical device wrapper.
 */
class Device {
public:
    /*!
     * \brief A pointer to the Device.
     */
    using ptr = std::shared_ptr<Device>;

    /*!
     * \brief (constructor)
     *
     * \param dev The PhysDevice in use.
     * \param inst The Instance in use.
     */
    Device(PhysDevice& dev, Instance::ptr inst);

    Device(Device&&) = delete;
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    ~Device() noexcept;

    /*!
     * \copydoc Instance::inner()
     */
    VkDevice inner() { return dev; }

    /*!
     * \brief Loads Vulkan functions that involve a logical
     * device (see
     * [VkGetDeviceProcAddr()](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetDeviceProcAddr.html)
     * in the Vulkan manual).
     *
     * \copydetails Instance::get_proc_addr()
     */
    PFN_vkVoidFunction get_proc_addr(const char* name);

private:
    VkDevice dev;
    VkQueue graphics;
    VkQueue present;

    PFN_vkCreateDevice create_dev;
    PFN_vkGetDeviceQueue get_dev_queue;
    PFN_vkGetDeviceProcAddr get_dev_proc_addr;
    PFN_vkDestroyDevice destroy_dev;
};

} // namespace cu

#endif
