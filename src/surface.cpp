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

#include "surface.hpp"

#include "log.hpp"

#include "sdl.hpp"
#include "instance.hpp"
#include "phys_device.hpp"

namespace cu {

Surface::Surface(SDL& sdl, Instance& instance)
    :get_surf_caps{
         reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(
             instance.get_proc_addr("vkGetPhysicalDeviceSurfaceCapabilitiesKHR")
         )
     },
     destroy_surf{
         reinterpret_cast<PFN_vkDestroySurfaceKHR>(
             instance.get_proc_addr("vkDestroySurfaceKHR")
         )
     }
{
    sdl.create_surface(instance, &surf);
    inst = instance.inner();
}

Surface::~Surface() noexcept
{
    log.attempt("Vulkan: destroying surface");
    destroy_surf(inst, surf, NULL);
    log.finish();
    log.brk();
}

VkSurfaceCapabilitiesKHR Surface::capabilities(PhysDevice& dev)
{
    VkSurfaceCapabilitiesKHR caps;

    get_surf_caps(dev.inner(), surf, &caps);

    return caps;
}

} // namespace cu
