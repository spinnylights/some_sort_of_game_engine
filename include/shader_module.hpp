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
 * Copyright (c) 2022 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef yf4146aba35141379744cc68f4913e54
#define yf4146aba35141379744cc68f4913e54

#include <vulkan/vulkan.hpp>

#include "bin_file.hpp"
#include "device.hpp"

namespace cu {

/*!
 * \brief A VkShaderModule wrapper.
 */
class ShaderModule {
public:
    /*!
     * \brief (constructor)
     *
     * \param l_dev The current Device.
     * \param file  The compiled (SPIR-V) shader.
     */
    ShaderModule(Device::ptr l_dev, BinFile file);

    ShaderModule(const ShaderModule&) = delete;
    ShaderModule& operator=(const ShaderModule&) = delete;

    ShaderModule(ShaderModule&&);
    ShaderModule& operator=(ShaderModule&&);

    ~ShaderModule() noexcept;

public:
    VkShaderModule inner;

public:
    bool free_inner = false;

public:
    Device::ptr dev;

public:
    BinFile f;

public:
    PFN_vkCreateShaderModule create_shmodul;
    PFN_vkDestroyShaderModule destroy_shmodul;
};

} // namespace cu

#endif
