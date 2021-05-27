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

#ifndef ac994460237c45fcb74600d711cd72af
#define ac994460237c45fcb74600d711cd72af

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <vector>
#include <string>

namespace cu {

class Instance {
public:
    Instance(std::vector<const char*> exts,
             std::vector<const char*> layers,
             bool debug = false);

    Instance(Instance&&) = delete;
    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    ~Instance() noexcept;

    VkInstance inner() { return inst; }

    PFN_vkVoidFunction get_proc_addr(const char* name);

private:
    VkInstance inst;
    bool dbg;
    VkDebugUtilsMessengerEXT dbg_msgr;

    PFN_vkGetInstanceProcAddr get_inst_proc_addr;
    PFN_vkCreateInstance create_inst;
    PFN_vkDestroyInstance destroy_inst;
    PFN_vkCreateDebugUtilsMessengerEXT create_dbg_msgr;
    PFN_vkDestroyDebugUtilsMessengerEXT destroy_dbg_msgr;

    void setup_debug_utils();
};

} // namespace cu

#endif
