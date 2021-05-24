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


#include "instance.hpp"

#include "log.hpp"

#include "vulkan.hpp"

#include <stdexcept>

namespace cu {

void check_under_uint32(std::vector<const char*> items, std::string type)
{
    if (items.size() > UINT32_MAX) {
        throw std::runtime_error("the desired count of " + type + " " +
                                 "must be <= UINT32_MAX");
    }
}

Instance::Instance(std::vector<const char*> exts,
                   std::vector<const char*> layers)
{
    check_under_uint32(exts, "extensions");
    check_under_uint32(layers, "layers");

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "Crypt Underworld",
        .applicationVersion = 0,
        .pEngineName = "Crypt Underworld",
        .engineVersion = 0,
        .apiVersion = VK_MAKE_VERSION(2, 170, 0)
    };

    VkInstanceCreateInfo inst_inf = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(exts.size()),
        .ppEnabledExtensionNames = exts.data()
    };

    Vulkan::vk_try(vkCreateInstance(&inst_inf, NULL, &inst), "creating instance");

    log.indent();
    log.enter("instance layers", layers);
    log.enter("instance extensions", exts);
    log.brk();
}

Instance::~Instance() noexcept
{
    log.attempt("Vulkan: destroying instance");
    vkDestroyInstance(inst, NULL);
    log.finish();
    log.brk();
}

} // namespace cu
