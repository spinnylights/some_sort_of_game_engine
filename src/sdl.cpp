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

#include <sstream>
#include <stdexcept>

#include "sdl.hpp"

namespace cu {

void SDL::sdl_try(int result, std::string oper)
{
    if (result != 0) {
        sdl_throw(oper);
    }
}

void SDL::sdl_try(void* result, std::string oper)
{
    if (result == NULL) {
        sdl_throw(oper);
    }
}

void SDL::sdl_try(SDL_bool result, std::string oper)
{
    if (result == SDL_FALSE) {
        sdl_throw(oper);
    }
}

void SDL::sdl_throw(std::string oper)
{
    std::stringstream ss;
    ss << "Unable to " << oper << ": " << SDL_GetError();
    throw std::runtime_error(ss.str());
}

SDL::SDL()
{
    sdl_try(SDL_Init(SDL_INIT_VIDEO), "initialize SDL");
}

SDL::~SDL()
{
    SDL_Quit();
}

std::vector<const char*> SDL::get_req_vulk_exts(Window& win) const
{
    uint32_t cnt;
    sdl_try(SDL_Vulkan_GetInstanceExtensions(win.inner(),
                                             &cnt,
                                             NULL),
            "get SDL-required Vulkan instance exts count");

    std::vector<const char*> exts (cnt);

    sdl_try(SDL_Vulkan_GetInstanceExtensions(win.inner(),
                                             &cnt,
                                             exts.data()),
            "get names of SDL-required Vulkan instance exts");

    return exts;
}

} // namespace cu
