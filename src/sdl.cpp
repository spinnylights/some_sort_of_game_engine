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

#include "sdl.hpp"

#include "instance.hpp"

#include <sstream>
#include <stdexcept>

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

SDL_Window* create_window()
{
    SDL_Window* win;

    int width = 640;
    int height = 480;
    SDL::sdl_try(win = SDL_CreateWindow("Crypt Underworld",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        width,
                                        height,
                                        SDL_WINDOW_VULKAN),
                 "create window");

    return win;
}

SDL::SDL()
{
    sdl_try(SDL_Init(SDL_INIT_VIDEO), "initialize SDL");
    win = create_window();
}

SDL::~SDL() noexcept
{
    SDL_DestroyWindow(win);
    SDL_Quit();
}

std::vector<const char*> SDL::get_req_vulk_exts() const
{
    uint32_t cnt;
    sdl_try(SDL_Vulkan_GetInstanceExtensions(win,
                                             &cnt,
                                             NULL),
            "get SDL-required Vulkan instance exts count");

    std::vector<const char*> exts (cnt);

    sdl_try(SDL_Vulkan_GetInstanceExtensions(win,
                                             &cnt,
                                             exts.data()),
            "get names of SDL-required Vulkan instance exts");

    return exts;
}

void SDL::create_surface(Instance& inst, VkSurfaceKHR* surf)
{
    sdl_try(SDL_Vulkan_CreateSurface(win, inst.inner(), surf),
            "create surface");
}

} // namespace cu
