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

#include "debug_msgr.hpp"

#include "log.hpp"

#include "vulkan.hpp"

namespace cu {

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
               VkDebugUtilsMessageTypeFlagsEXT msg_type,
               const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
               void* user_data)
{
    log.enter("Vulkan (validations)", std::string{callback_data->pMessage});
    log.brk();

    return VK_FALSE;
}

DebugMsgr::DebugMsgr(Instance::ptr outer_instance, bool debug_on)
    : inst{outer_instance},
      dbg_on{debug_on}
{
    if (dbg_on) {
        create_dbg_msgr =
            reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
               outer_instance->get_proc_addr("vkCreateDebugUtilsMessengerEXT")
            );

        destroy_dbg_msgr =
            reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                outer_instance->get_proc_addr("vkDestroyDebugUtilsMessengerEXT")
            );

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

        Vulkan::vk_try(create_dbg_msgr(inst->inner(),
                                       &dbg_create_info,
                                       NULL,
                                       &msgr),
                       "creating debug messenger");

        log.brk();
    }
}

DebugMsgr::~DebugMsgr() noexcept
{
    if (dbg_on) {
        log.attempt("Vulkan", "destroying debug messenger");
        destroy_dbg_msgr(inst->inner(), msgr, NULL);
        log.finish();
        log.brk();
    }
}

} // namespace cu
