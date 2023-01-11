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

namespace cu {

class CommandBuffer {
public:
    CommandBuffer(Device::ptr l_dev, CommandPool::ptr cmd_pool);

    void begin();

    void bind(ComputePipeline&);
    void bind(ComputePipeline&, std::vector<VkDescriptorSet>);
    void bind(ComputePipeline&,
              uint32_t set_bndng_offset,
              std::vector<VkDescriptorSet>);
    // TODO: dynamic offsets

    void dispatch(uint32_t x);
    void dispatch(uint32_t x, uint32_t y);
    void dispatch(uint32_t x, uint32_t y, uint32_t z);

private:
    VkCommandBuffer nner;

private:
    CommandPool::ptr pool;

private:
    PFN_vkAllocateCommandBuffers alloc;
    PFN_vkBeginCommandBuffer     vk_begin;
    PFN_vkCmdBindPipeline        bind_pipel;
    PFN_vkCmdBindDescriptorSets  bind_desc_sets;
    PFN_vkCmdDispatch            vk_dispatch;
};

} // namespace cu

#endif
