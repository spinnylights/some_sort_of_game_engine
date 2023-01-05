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

#include "compute_pipeline.hpp"
#include "vulkan.hpp"
#include "log.hpp"

namespace cu {

ComputePipeline::ComputePipeline(Device::ptr l_dev,
                                 ShaderModule::ptr shader,
                                 PipelineLayout::ptr pipel_layt,
                                 vk::PipelineCreateFlags flags)
    : Deviced(l_dev, "compute pipeline", "ComputePipelines", "Pipeline"),
      shdr {shader},
      pipelt {pipel_layt}
{
    VkPipelineShaderStageCreateInfo shdr_stg_inf = {
        .sType               
            = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext               = NULL,
        .flags               = 0,
        .stage               = v(vk::ShaderStageFlag::cmpte),
        .module              = shdr->inner(),
        .pName               = "main",
        // TODO: specialization constants
    };

    VkComputePipelineCreateInfo create_inf = {
        .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .pNext = NULL, // i think you can get pipeline creation feedback for
                       // profiling here
        .flags = flags,
        .stage = shdr_stg_inf,
        .layout = pipelt->inner(),
        // pipeline derivatives untouched
    };

    Vulkan::vk_try(create(dev->inner(),
                                      VK_NULL_HANDLE, // TODO: pipeline cache
                                      1,
                                      &create_inf,
                                      NULL,
                                      &nner),
                   "create compute pipeline");

    log.indent();

    auto flgs_cstrs = vk::pplne_create_flags_cstrs(flags);
    if (!flgs_cstrs.empty()) {
        log.enter("flags", flgs_cstrs);
    }

    log.enter("shader", shader->name());
    pipelt->log_attrs();
}

} // namespace 
