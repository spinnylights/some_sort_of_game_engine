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

ShaderModule::ShaderModule(Device::ptr l_dev, BinFile file)
    : dev {l_dev},
      f {file},
      create_shmodul {
          reinterpret_cast<PFN_vkCreateShaderModule>(
              dev->get_proc_addr("vkCreateShaderModule")
          )
      },
      destroy_shmodul {
          reinterpret_cast<PFN_vkDestroyShaderModule>(
              dev->get_proc_addr("vkDestroyShaderModule")
          )
      }
{
    VkShaderModuleCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .codeSize = file.size(),
        .pCode    = file.u32()
    };

    Vulkan::vk_try(create_shmodul(dev->inner(), &create_info, nullptr, &inner),
                   "create shader module");
    log.indent();
    log.enter("path: ", std::string(file.path()));
    log.enter("filesize: ", file.size());
    log.brk();

    free_inner = true;
}

ShaderModule::ShaderModule(ShaderModule&& s)
    : inner {s.inner},
      free_inner {true},
      dev {s.dev},
      f {std::move(s.f)},
      create_shmodul {s.create_shmodul},
      destroy_shmodul {s.destroy_shmodul}
{
    s.inner = nullptr;
    s.free_inner = false;
    s.dev = nullptr;
    s.create_shmodul = nullptr;
    s.destroy_shmodul = nullptr;
}

ShaderModule& ShaderModule::operator=(ShaderModule&& s)
{
    std::swap(*this, s);

    return *this;
}

ShaderModule::~ShaderModule() noexcept
{
    if (free_inner) {
        log.attempt("Vulkan",
                    "destroying shader module from " + std::string(f.path()));

        destroy_shmodul(dev->inner(), inner, NULL);

        log.finish();
        log.brk();
    }
}

} // namespace cu
