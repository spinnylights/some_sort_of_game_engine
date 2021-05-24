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

#include "instance.hpp"
#include "surface.hpp"
#include "phys_devices.hpp"
#include "logi_device.hpp"

namespace cu {

class SDL;

class Vulkan {
public:
    static void vk_try(VkResult, std::string oper);
    static bool vkbool_to_bool(VkBool32);
    static std::string api_ver_to_str(uint32_t ver);

    Vulkan(std::vector<const char*> exts,
           std::vector<const char*> layers,
           SDL&);

    Vulkan(Vulkan&&) = delete;
    Vulkan(const Vulkan&) = delete;
    Vulkan& operator=(const Vulkan&) = delete;

    ~Vulkan() = default;

private:
    Instance inst;
    Surface surf;
    PhysDevices phys_devs;
    LogiDevice logi_dev;
};

} // namespace cu

#endif
