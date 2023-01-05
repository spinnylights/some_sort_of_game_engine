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

#ifndef lb51089594c74cada3fffc0f03030b57
#define lb51089594c74cada3fffc0f03030b57

#include <vulkan/vulkan.hpp>

#include "shader_module.hpp"
#include "vulkan_util.hpp"
#include "pipeline_layout.hpp"
#include "deviced.hpp"

namespace cu {

class ComputePipeline : private Deviced<PFN_vkCreateComputePipelines,
                                        PFN_vkDestroyPipeline,
                                        VkPipeline> {
public:
    ComputePipeline(Device::ptr l_dev,
                    ShaderModule::ptr shader,
                    PipelineLayout::ptr pipel_layt,
                    vk::PipelineCreateFlags flags = 0);

private:
    ShaderModule::ptr   shdr;
    PipelineLayout::ptr pipelt;
};

} // namespace cu

#endif
