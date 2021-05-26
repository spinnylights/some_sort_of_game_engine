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

#include "log.hpp"

#include "instance.hpp"

#include <sstream>
#include <stdexcept>

namespace cu {

void inner_try(bool result, std::string oper)
{
    log.attempt("SDL: " + oper);
    if (result) {
        SDL::sdl_throw(oper);
    }
    log.finish();
}

void SDL::sdl_try(int result, std::string oper)
{
    inner_try(result != 0, oper);
}

void SDL::sdl_try(void* result, std::string oper)
{
    inner_try(result == NULL, oper);
}

void SDL::sdl_try(SDL_bool result, std::string oper)
{
    inner_try(result == SDL_FALSE, oper);
}

void SDL::sdl_throw(std::string oper)
{
    std::stringstream ss;
    ss << "Failed at " << oper << ": " << SDL_GetError();
    throw std::runtime_error(ss.str());
}

SDL_Window* create_window()
{
    SDL_Window* win;

    const std::string title = "Crypt Underworld";
    const auto x_pos = SDL_WINDOWPOS_UNDEFINED;
    const auto y_pos = SDL_WINDOWPOS_UNDEFINED;
    const int width = 640;
    const int height = 480;
    const auto flags = SDL_WINDOW_VULKAN;
    SDL::sdl_try(win = SDL_CreateWindow(title.c_str(),
                                        x_pos,
                                        y_pos,
                                        width,
                                        height,
                                        flags),
                 "creating window");

    log.indent();
    log.enter_obj({
        .name = "Window",
        .members = {
            { "title", title },
            { "x", std::to_string(x_pos) },
            { "y", std::to_string(x_pos) },
            { "width", std::to_string(width) },
            { "height", std::to_string(height) },
            { "flags", std::to_string(flags) },
        }
    });
    log.brk();

    return win;
}

SDL::SDL()
{
    sdl_try(SDL_Init(SDL_INIT_VIDEO), "initializing SDL");
    log.brk();
    win = create_window();
}

SDL::~SDL() noexcept
{
    log.attempt("SDL: destroying window");
    SDL_DestroyWindow(win);
    log.finish();
    log.brk();

    log.attempt("SDL: quitting");
    SDL_Quit();
    log.finish();
}

std::vector<const char*> SDL::get_req_vulk_exts() const
{
    uint32_t cnt;
    sdl_try(SDL_Vulkan_GetInstanceExtensions(win,
                                             &cnt,
                                             NULL),
            "getting required Vulkan instance extensions count");

    log.indent();
    log.enter("required extensions count: " + std::to_string(cnt));
    log.brk();

    std::vector<const char*> exts (cnt);

    sdl_try(SDL_Vulkan_GetInstanceExtensions(win,
                                             &cnt,
                                             exts.data()),
            "getting names of required Vulkan instance extensions");

    log.indent();
    log.enter("required extensions", exts);
    log.brk();

    return exts;
}

void SDL::create_surface(Instance& inst, VkSurfaceKHR* surf)
{
    sdl_try(SDL_Vulkan_CreateSurface(win, inst.inner(), surf),
            "creating surface");
    log.brk();
}

PFN_vkGetInstanceProcAddr SDL::get_get_inst_proc_addr()
{
    return reinterpret_cast<PFN_vkGetInstanceProcAddr>(
               SDL_Vulkan_GetVkGetInstanceProcAddr()
           );
}
} // namespace cu
