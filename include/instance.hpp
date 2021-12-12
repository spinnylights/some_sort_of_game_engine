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
#include <memory>

namespace cu {

/*!
 * \brief A Vulkan instance wrapper.
 */
class Instance {
public:
    using ptr = std::shared_ptr<Instance>;

    /*!
     * \brief (constructor)
     *
     * \param exts The extensions required.
     * \param layers The layers to enable.
     */
    Instance(std::vector<const char*> exts,
             std::vector<const char*> layers);

    Instance(Instance&&) = delete;
    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    ~Instance() noexcept;

    /*!
     * \brief Returns the handle wrapped by the class.
     */
    VkInstance inner() { return inst; }

    /*!
     * \brief Loads Vulkan functions that do not involve a
     * Device (see
     * [VkGetInstanceProcAddr()](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkGetInstanceProcAddr.html)
     * in the Vulkan manual).
     *
     * \param name The name of the function to load.
     */
    PFN_vkVoidFunction get_proc_addr(const char* name);

private:
    VkInstance inst;

    PFN_vkGetInstanceProcAddr get_inst_proc_addr;
    PFN_vkEnumerateInstanceLayerProperties enum_inst_layer_props;
    PFN_vkEnumerateInstanceExtensionProperties enum_inst_ext_props;
    PFN_vkCreateInstance create_inst;
    PFN_vkDestroyInstance destroy_inst;

    void check_avail_layers(const std::vector<const char*>& layers);
    void check_avail_exts(const std::vector<const char*>& exts,
                          const std::vector<const char*>& layers);
};

} // namespace cu

#endif
