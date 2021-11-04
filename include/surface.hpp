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

#ifndef b1f51f6bee864d8cb4da54dd7c09f340
#define b1f51f6bee864d8cb4da54dd7c09f340

#include "instance.hpp"

#include <vulkan/vulkan.h>

#include <vector>
#include <memory>

namespace cu {

class SDL;
class PhysDevice;

/*!
 * \brief A Vulkan surface wrapper.
 */
class Surface {
public:
    /*!
     * \brief (constructor)
     *
     * \param sdl The SDL instance in use.
     * \param inst The Instance instance in use.
     */
    Surface(SDL& sdl, Instance::ptr inst);

    Surface(Surface&&) = delete;
    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;

    ~Surface() noexcept;

    /*!
     * \copydoc Instance::inner()
     */
    VkSurfaceKHR inner() { return surf; }

    /*!
     * \brief Returns the capabilities of the surface (see
     * [VkSurfaceCapabilitiesKHR](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSurfaceCapabilitiesKHR.html)
     * in the Vulkan manual).
     *
     * \param dev The PhysDevice in use.
     */
    VkSurfaceCapabilitiesKHR capabilities(PhysDevice& dev);

    /*!
     * \brief Returns a list of the present modes supported by
     * the surface (see
     * [VkPresentModeKHR](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPresentModeKHR.html)
     * in the Vulkan manual).
     *
     * \copydetails capabilities()
     */
    std::vector<VkPresentModeKHR> present_modes(PhysDevice& dev);

private:
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR get_surf_caps;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR get_pres_modes;
    PFN_vkDestroySurfaceKHR destroy_surf;

    Instance::ptr inst;

    VkSurfaceKHR surf;
};

} // namespace cu

#endif
