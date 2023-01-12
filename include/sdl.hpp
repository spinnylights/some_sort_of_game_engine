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

#include "instance.hpp"

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.h>

namespace cu {

/*!
 * \brief The size of the drawable area of the platform window
 * spawned by SDL.
 */
struct WinSize {
    int width;
    int height;
};

/*!
 * \brief An SDL wrapper class.
 */
class SDL {
public:
    /*!
     * \brief A convenient way to check whether or not an SDL
     * operation succeeded. Logs the attempt if logging is turned
     * on. Throws if SDL reports an error.
     *
     * \param result The result returned by the operation.
     *
     * \param oper A description of what is being attempted (see
     * Log::attempt()).
     */
    static void sdl_try(int result, std::string oper);
    /*!
     * \overload
     */
    static void sdl_try(void* result, std::string oper);
    /*!
     * \overload
     */
    static void sdl_try(SDL_bool result, std::string oper);

    /*!
     * \brief Throws an error, reporting the error message
     * returned by SDL.
     *
     * \param oper A description of what was being attempted.
     */
    static void sdl_throw(std::string oper);

    /*!
     * \brief Returns a pointer to a function that can be used to
     * load Vulkan functions (see
     * [`SDL_Vulkan_GetVkGetInstanceProcAddr()`](https://wiki.libsdl.org/SDL_Vulkan_GetVkInstanceProcAddr)).
     */
    static PFN_vkGetInstanceProcAddr get_get_inst_proc_addr();

    /*!
     * \brief (constructor) Initializes SDL, including the
     * spawning of a platform window.
     */
    SDL();

    SDL(SDL&&) = delete;
    SDL(const SDL&) = delete;
    SDL& operator=(const SDL&) = delete;

    ~SDL() noexcept;

    /*!
     * \brief Returns the names of the Vulkan extensions required
     * by SDL.
     */
    std::vector<const char*> get_req_vulk_exts() const;

    /*!
     * \brief Creates a Vulkan rendering surface for the platform
     * window. Unless you have a specific reason you should
     * create a Surface instead of calling this, which will
     * handle this process automatically.
     *
     * \param inst The Vulkan instance in use.
     *
     * \param surf A pointer to the Vulkan surface handle to
     * initialize. Doesn't take a Surface& because this function
     * is needed in Surface initialization.
     */
    void create_surface(Instance::ptr inst, VkSurfaceKHR* surf);

    /*!
     * \brief Returns the size of the drawable area of the
     * platform window.
     */
    WinSize get_win_size();

    /*!
     * \brief Refreshes the event state. Call this at the top of the frame.
     */
    void poll();

    /*!
     * \brief Whether or not the user has requested to quit.
     */
    bool quit();

private:
    SDL_Window* win;

    struct Events {
        bool quit = false;
    };

    Events evs;
};

} // namespace cu

#endif
