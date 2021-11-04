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

#ifndef q765e99c966249c6aa35adad25b72458
#define q765e99c966249c6aa35adad25b72458

#include <vector>
#include <string>
#include <memory>

#include "instance.hpp"
#include "debug_msgr.hpp"
#include "surface.hpp"
#include "phys_devices.hpp"
#include "logi_device.hpp"
#include "swapchain.hpp"

namespace cu {

class SDL;

/*!
 * \brief The entry point into the Vulkan code; coordinates the
 * activity of the Vulkan classes.
 */
class Vulkan {
public:
    /*!
     * \brief A convenient way to check the return code of a
     * Vulkan operation. Throws in the event of a VK_ERROR* code
     * (see the
     * [VkResult](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkResult.html)
     * entry in the Vulkan manual). Behaves similarly to
     * SDL::sdl_try().
     *
     * \copydetails SDL::sdl_try()
     */
    static void vk_try(VkResult result, std::string oper);

    /*!
     * \brief Converts a Vulkan boolean to a C++ boolean.
     */
    static bool vkbool_to_bool(VkBool32);

    /*!
     * \brief Converts a Vulkan API version value to a
     * human-readable string.
     */
    static std::string api_ver_to_str(uint32_t ver);

    /*!
     * \brief (constructor) Initializes Vulkan, including the
     * various components needed.
     *
     * \param exts The extensions required.
     * \param layers The layers to enable.
     * \param sdl The SDL instance in use.
     * \param debug Whether to enable the debug utils.
     */
    Vulkan(std::vector<const char*> exts,
           std::vector<const char*> layers,
           SDL& sdl,
           bool debug = false);

    Vulkan(Vulkan&&) = delete;
    Vulkan(const Vulkan&) = delete;
    Vulkan& operator=(const Vulkan&) = delete;

    ~Vulkan() = default;

private:
    Instance::ptr inst;
    DebugMsgr dbg_msgr;
    Surface surf;
    PhysDevices phys_devs;
    LogiDevice::ptr logi_dev;
    Swapchain swch;
};

} // namespace cu

#endif
