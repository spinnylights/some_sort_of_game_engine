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

#ifndef nf881038f94847228e50390dc7437b5e
#define nf881038f94847228e50390dc7437b5e

#include "instance.hpp"

#include <vulkan/vulkan.h>

#include <cstdint>

namespace cu {

class Surface;
class Instance;

/*!
 * \brief A representation of a Vulkan queue family available on
 * a PhysDevice.
 */
class QueueFamily {
public:
    /*!
     * \brief (constructor)
     *
     * \param handle The queue family properties handle populated
     * by Vulkan (see
     * [vkGetPhysicalDeviceQueueFamilyProperties()](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetPhysicalDeviceQueueFamilyProperties.html)
     * in the Vulkan manual).
     *
     * \param ndex The index of the queue family on the device.
     *
     * \param dev The Vulkan physical device handle for the
     * device which has this queue family (needed because this
     * class is involved in PhysDevice instantiation).
     *
     * \param surf The Surface in use.
     *
     * \param inst The Instance in use.
     */
    QueueFamily(VkQueueFamilyProperties& handle,
                uint32_t ndex,
                VkPhysicalDevice dev,
                Surface& surf,
                Instance::ptr inst);

    /*!
     * \brief Whether the queue family supports graphics
     * operations.
     */
    bool graphics() const;

    /*!
     * \brief Whether the queue family supports compute
     * operations.
     */
    bool compute() const;

    /*!
     * \brief Whether the queue family supports transfer
     * operations.
     */
    bool transfer() const;

    /*!
     * \brief Whether the queue family supports sparse memory
     * management operations.
     */
    bool sparse_binding() const;

    /*!
     * \brief Whether the queues in this queue family can be made
     * protected-capable.
     */
    bool protected_memory() const;

    /*!
     * \brief The number of capability flags enabled for the
     * queue family.
     */
    uint32_t flag_count() const;

    /*!
     * \brief Whether the queue family supports presentation to
     * the Surface in use.
     */
    bool     present() const { return pres_support; }

    /*!
     * \brief The number of queues within the queue family.
     */
    uint32_t queue_count()       const { return queue_cnt; }

    /*!
     * \brief The index of the queue family on the device.
     */
    uint32_t index()             const { return ndx; }

    /*!
     * \brief Write a description of the queue family to the log.
     */
    void log_info() const;

private:
    VkQueueFlags flags;
    uint32_t     queue_cnt;
    uint32_t     ndx;
    bool         pres_support;

    PFN_vkGetPhysicalDeviceSurfaceSupportKHR get_surf_support;
};

} // namespace cu

#endif
