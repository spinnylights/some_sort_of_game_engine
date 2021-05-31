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

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "queue_family.hpp"

namespace cu {

/*!
 * \brief A representation of a physical device available to
 * Vulkan. Used by PhysDevices.
 */
struct PhysDevice {
    /*!
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
     * \brief The amount of video memory available to the device
     * in bytes.
     */
    VkDeviceSize mem;

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
};

} // namespace cu

#endif
