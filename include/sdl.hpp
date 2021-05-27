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

#ifndef Zf3e510f4ae1456bbb00eb41d1469268
#define Zf3e510f4ae1456bbb00eb41d1469268

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.h>

namespace cu {

class Instance;

struct WinSize {
    int width;
    int height;
};

class SDL {
public:
    static void sdl_try(int result, std::string oper);
    static void sdl_try(void* result, std::string oper);
    static void sdl_try(SDL_bool result, std::string oper);
    static void sdl_throw(std::string oper);
    static PFN_vkGetInstanceProcAddr get_get_inst_proc_addr();

    SDL();

    SDL(SDL&&) = delete;
    SDL(const SDL&) = delete;
    SDL& operator=(const SDL&) = delete;

    ~SDL() noexcept;

    std::vector<const char*> get_req_vulk_exts() const;

    void create_surface(Instance&, VkSurfaceKHR*);

    WinSize get_win_size();

private:
    SDL_Window* win;
};

} // namespace cu

#endif
