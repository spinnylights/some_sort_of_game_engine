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

#include "shader_module.hpp"

#include "log.hpp"

#include "vulkan.hpp"

namespace cu {

ShaderModule::ShaderModule(Device::ptr l_dev,
                           std::string name,
                           const std::uint32_t* bin,
                           std::size_t sz)
    : Deviced(l_dev,
              "shader module '" + name + "'",
              "ShaderModule"),
      nme {name}
{
    VkShaderModuleCreateInfo create_info {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = sz,
        .pCode    = bin
    };

    Vulkan::vk_try(create(dev->inner(), &create_info, nullptr, &nner),
                   "creating " + descrptn());
    log.indent();
    log.enter("filesize", sz);
    log.brk();

    free_inner = true;
}

ShaderModule::ShaderModule(Device::ptr l_dev,
                           std::string name,
                           const BinData& file)
    : ShaderModule(l_dev, name, file.u32(), file.size())
{}

ShaderModule::ShaderModule(ShaderModule&& s)
    : Deviced(s.dev, s.descrptn(), s.create_fn_suffix(), s.destroy_fn_suffix()),
      free_inner {true}
{
    s.nner = nullptr;
    s.free_inner = false;
    s.dev = nullptr;
    s.create = nullptr;
    s.destroy = nullptr;
}

ShaderModule& ShaderModule::operator=(ShaderModule&& s)
{
    std::swap(*this, s);

    return *this;
}

ShaderModule::~ShaderModule() noexcept
{
    if (free_inner) {
        dstrct();
    }
}

} // namespace cu
