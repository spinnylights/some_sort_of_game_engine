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
#include "game.hpp"

#include <vulkan/vulkan_core.h>

#include <stdexcept>
#include <algorithm>

namespace cu {

void check_under_uint32(std::vector<const char*> items, std::string type)
{
    if (items.size() > UINT32_MAX) {
        throw std::runtime_error("the desired count of " + type + " " +
                                 "must be <= UINT32_MAX");
    }
}

void Instance::check_avail_layers(const std::vector<const char*>& layers)
{
    uint32_t layer_cnt;
    Vulkan::vk_try(enum_inst_layer_props(&layer_cnt, NULL), "get layer count");
    log.indent();
    log.enter("layer count", layer_cnt);
    log.brk();

    std::vector<VkLayerProperties> layer_props (layer_cnt);
    Vulkan::vk_try(enum_inst_layer_props(&layer_cnt, layer_props.data()),
                   "get layer properties");
    log.indent();
    for (const auto& lp : layer_props) {
        log.enter_obj({
            .name = std::string{lp.layerName},
            .members = {
                {"description", std::string{lp.description}}
            }
        });
    }
    log.brk();

    std::vector<std::string> avail_layers;
    std::transform(layer_props.begin(),
                   layer_props.end(),
                   std::back_inserter(avail_layers),
                   [](VkLayerProperties& lp) -> std::string {
                       return std::string{lp.layerName};
                   });

    for (auto&& l : layers) {
        auto layer = std::string{l};
        if (std::find(begin(avail_layers), end(avail_layers), layer)
            == end(avail_layers)) {
            throw std::runtime_error("layer " + layer + " not available!");
        }
    }
}

void Instance::check_avail_exts(const std::vector<const char*>& exts,
                                const std::vector<const char*>& layers)
{
    std::vector<std::string> avail_exts;

    auto check_for_exts {layers};
    check_for_exts.push_back(NULL);
    for (const auto& n : check_for_exts) {
        std::string name;
        if (n == NULL) {
            name = "implementation";
        } else {
            name = std::string{n};
        }

        uint32_t ext_cnt;
        Vulkan::vk_try(enum_inst_ext_props(n, &ext_cnt, NULL),
                       "get " + name + " extension count");
        log.indent();
        log.enter("extension count", ext_cnt);
        log.brk();

        std::vector<VkExtensionProperties> ext_props (ext_cnt);
        Vulkan::vk_try(enum_inst_ext_props(n, &ext_cnt, ext_props.data()),
                       "get " + name + " extensions");
        log.indent();
        for (const auto& ep : ext_props) {
            log.enter(std::string{ep.extensionName});
        }
        log.brk();

        for (auto&& ep : ext_props) {
            avail_exts.push_back(std::string{ep.extensionName});
        }
    }

    for (auto&& e : exts) {
        auto ext = std::string{e};
        if (std::find(begin(avail_exts), end(avail_exts), ext)
            == end(avail_exts)) {
            throw std::runtime_error("extension " + ext + " not available!");
        }
    }
}

Instance::Instance(std::vector<const char*> exts,
                   std::vector<const char*> layers)
    :get_inst_proc_addr{SDL::get_get_inst_proc_addr()},
     enum_inst_layer_props {
         reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(
             get_inst_proc_addr(NULL, "vkEnumerateInstanceLayerProperties")
         )
     },
     enum_inst_ext_props {
         reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(
             get_inst_proc_addr(NULL, "vkEnumerateInstanceExtensionProperties")
         )
     },
     create_inst {
         reinterpret_cast<PFN_vkCreateInstance>(
             get_inst_proc_addr(NULL, "vkCreateInstance")
         )
     }
{
    // TODO: replace with `expects` when e.g. debian stable moves
    // to gcc 10 (i.e. contracts become available)
    check_under_uint32(exts, "extensions");
    check_under_uint32(layers, "layers");

    check_avail_layers(layers);
    check_avail_exts(exts, layers);

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = Game::name.c_str(),
        .applicationVersion = Game::version,
        .pEngineName = Game::name.c_str(),
        .engineVersion = Game::version,
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
}

Instance::~Instance() noexcept
{
    log.attempt("Vulkan", "destroying instance");
    destroy_inst(inst, NULL);
    log.finish();
    log.brk();
}

PFN_vkVoidFunction Instance::get_proc_addr(const char* name)
{
    auto fn = get_inst_proc_addr(inst, name);

    if (fn == NULL) {
        throw std::runtime_error("instance function "
                                 + std::string{name}
                                 + " is not available!");
    }

    return fn;
}

} // namespace cu
