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

namespace cu {

Surface::Surface(SDL& sdl, Instance& instance)
     :destroy_surf{
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

} // namespace cu
