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

#include <stdexcept>

#include "vulkan.hpp"

namespace cu {

std::string map_vk_result(VkResult res)
{
    std::string msg;
    switch (res) {
        case VK_SUCCESS:
            msg = "success";
            break;
        case VK_NOT_READY:
            msg = "not ready";
            break;
        case VK_TIMEOUT:
            msg = "timeout";
            break;
        case VK_EVENT_SET:
            msg = "event set";
            break;
        case VK_EVENT_RESET:
            msg = "event reset";
            break;
        case VK_INCOMPLETE:
            msg = "incomplete";
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            msg = "error! out of host memory";
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            msg = "error! out of device memory";
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            msg = "error! initialization failed";
            break;
        case VK_ERROR_DEVICE_LOST:
            msg = "error! device lost";
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            msg = "error! memory map failed";
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            msg = "error! layer not present";
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            msg = "error! extension not present";
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            msg = "error! feature not present";
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            msg = "error! incompatible driver";
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            msg = "error! too many objects";
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            msg = "error! format not supported";
            break;
        case VK_ERROR_FRAGMENTED_POOL:
            msg = "error! fragmented pool";
            break;
        case VK_ERROR_UNKNOWN:
            msg = "error! unknown";
            break;
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            msg = "error! out of pool memory";
            break;
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            msg = "error! invalid external handle";
            break;
        case VK_ERROR_FRAGMENTATION:
            msg = "error! fragmentation";
            break;
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            msg = "error! invalid opaque capture address";
            break;
        case VK_ERROR_SURFACE_LOST_KHR:
            msg = "error! surface lost";
            break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            msg = "error! native window in use";
            break;
        case VK_SUBOPTIMAL_KHR:
            msg = "suboptimal";
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            msg = "error! out of date";
            break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            msg = "error! incompatible display";
            break;
        case VK_ERROR_VALIDATION_FAILED_EXT:
            msg = "error! validation failed";
            break;
        case VK_ERROR_INVALID_SHADER_NV:
            msg = "error! invalid shader nv";
            break;
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            msg = "error! invalid drm format modifier plane layout";
            break;
        case VK_ERROR_NOT_PERMITTED_EXT:
            msg = "error! not permitted";
            break;
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            msg = "error! full screen exclusive mode lost";
            break;
        case VK_THREAD_IDLE_KHR:
            msg = "thread idle";
            break;
        case VK_THREAD_DONE_KHR:
            msg = "thread done";
            break;
        case VK_OPERATION_DEFERRED_KHR:
            msg = "operation deferred";
            break;
        case VK_OPERATION_NOT_DEFERRED_KHR:
            msg = "operation not deferred";
            break;
        case VK_PIPELINE_COMPILE_REQUIRED_EXT:
            msg = "pipeline compile required";
            break;
        case VK_RESULT_MAX_ENUM:
            msg = "max result enum value (something is very wrong)";
            break;
    }

    return msg;
}

void Vulkan::vk_try(VkResult res, std::string oper)
{
    if (res != VK_SUCCESS) {
        throw std::runtime_error("could not "
                                 + oper
                                 + "; Vulkan says: "
                                 + map_vk_result(res));
    }
}

void check_under_uint32(std::vector<const char*> items, std::string type)
{
    if (items.size() > UINT32_MAX) {
        throw std::runtime_error("the desired count of " + type + " " +
                                 "must be <= UINT32_MAX");
    }
}

Vulkan::Vulkan(std::vector<const char*> exts, std::vector<const char*> layers)
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

    vk_try(vkCreateInstance(&inst_inf, NULL, &inst), "create instance");
}

Vulkan::~Vulkan()
{
    vkDestroyInstance(inst, NULL);
}

} // namespace cu
