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
 * Copyright (c) 2023 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef ga3eb90b5f5a4122996664474a1437dd
#define ga3eb90b5f5a4122996664474a1437dd

#include "vulkan_util.hpp"
#include "device.hpp"
#include "command_pool.hpp"
#include "compute_pipeline.hpp"
#include "image.hpp"

namespace cu {

class CommandBuffer {
public:
    CommandBuffer(Device::ptr l_dev, CommandPool::ptr cmd_pool);

    CommandBuffer& begin();

    CommandBuffer& bind(ComputePipeline&);
    CommandBuffer& bind(ComputePipeline&, std::vector<VkDescriptorSet>);
    CommandBuffer& bind(ComputePipeline&,
                        uint32_t set_bndng_offset,
                        std::vector<VkDescriptorSet>);
    // TODO: dynamic offsets

    CommandBuffer& dispatch(uint32_t x);
    CommandBuffer& dispatch(uint32_t x, uint32_t y);
    CommandBuffer& dispatch(uint32_t x, uint32_t y, uint32_t z);

    CommandBuffer& barrier(Image&                      img,
                           vk::PipelineStageFlag       src_stage,
                           vk::PipelineStageFlag       dst_stage,
                           vk::AccessFlag              src_access,
                           vk::AccessFlag              dst_access,
                           vk::ImageLayout             old_layt,
                           vk::ImageLayout             new_layt,
                           vk::ImageAspectFlag         aspect);

    CommandBuffer& copy(Image& from, Image& to);

    CommandBuffer& end();

    const VkCommandBuffer* inner() const { return &nner; }

private:
    VkCommandBuffer nner;

private:
    CommandPool::ptr pool;

private:
    PFN_vkAllocateCommandBuffers alloc;
    PFN_vkBeginCommandBuffer     vk_begin;
    PFN_vkCmdBindPipeline        bind_pipel;
    PFN_vkCmdBindDescriptorSets  bind_desc_sets;
    PFN_vkCmdPipelineBarrier     pipel_barr;
    PFN_vkCmdDispatch            vk_dispatch;
    PFN_vkCmdCopyImage           copy_image;
    PFN_vkEndCommandBuffer       vk_end;
};

} // namespace cu

#endif
