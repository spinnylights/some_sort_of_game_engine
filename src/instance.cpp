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
#include "sdl.hpp"

#include <vulkan/vulkan_core.h>

#include <stdexcept>

namespace cu {

void check_under_uint32(std::vector<const char*> items, std::string type)
{
    if (items.size() > UINT32_MAX) {
        throw std::runtime_error("the desired count of " + type + " " +
                                 "must be <= UINT32_MAX");
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
               VkDebugUtilsMessageTypeFlagsEXT msg_type,
               const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
               void* user_data)
{
    log.enter("Vulkan (validations): " + std::string{callback_data->pMessage});
    log.brk();

    return VK_FALSE;
}

void throw_fn_not_avail(std::string name)
{
    throw(name + " is not available on this system");
}

Instance::Instance(std::vector<const char*> exts,
                   std::vector<const char*> layers,
                   bool debug)
    :dbg{debug},
     get_inst_proc_addr{SDL::get_get_inst_proc_addr()},
     create_inst {
         reinterpret_cast<PFN_vkCreateInstance>(
             get_inst_proc_addr(NULL, "vkCreateInstance")
         )
     }
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
        .apiVersion = VK_MAKE_API_VERSION(0, 1, 2, 0)
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

    Vulkan::vk_try(create_inst(&inst_inf, NULL, &inst), "creating instance");

    destroy_inst = {
        reinterpret_cast<PFN_vkDestroyInstance>(
            get_proc_addr("vkDestroyInstance")
        )
    };

    log.indent();
    log.enter("instance layers", layers);
    log.enter("instance extensions", exts);
    log.brk();

    if (dbg) {
        std::string create_dbg_msgr_name = "vkCreateDebugUtilsMessengerEXT";
        std::string destroy_dbg_msgr_name = "vkDestroyDebugUtilsMessengerEXT";

        create_dbg_msgr =
            reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                get_proc_addr(create_dbg_msgr_name.c_str())
            );
        if (create_dbg_msgr == NULL) {
            throw_fn_not_avail(create_dbg_msgr_name);
        }

        destroy_dbg_msgr =
            reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                get_proc_addr(destroy_dbg_msgr_name.c_str())
            );
        if (destroy_dbg_msgr == NULL) {
            throw_fn_not_avail(destroy_dbg_msgr_name);
        }

        VkDebugUtilsMessengerEXT dbg_msgr;

        VkDebugUtilsMessengerCreateInfoEXT dbg_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = NULL,
            .flags = 0,
            .messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debug_callback
        };

        Vulkan::vk_try(create_dbg_msgr(inst, &dbg_create_info, NULL, &dbg_msgr),
                       "creating debug messenger");
    }
}

Instance::~Instance() noexcept
{
    if (dbg) {
        log.attempt("Vulkan: destroying debug messenger");
        destroy_dbg_msgr(inst, dbg_msgr, NULL);
        log.finish();
        log.brk();
    }
    log.attempt("Vulkan: destroying instance");
    destroy_inst(inst, NULL);
    log.finish();
    log.brk();
}

PFN_vkVoidFunction Instance::get_proc_addr(const char* name)
{
    return get_inst_proc_addr(inst, name);
}

} // namespace cu
