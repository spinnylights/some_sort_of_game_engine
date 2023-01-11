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

#include "command_buffer.hpp"
#include "vulkan.hpp"

namespace cu {

CommandBuffer::CommandBuffer(Device::ptr l_dev, CommandPool::ptr cmd_pool)
    : pool {cmd_pool},
      alloc {
          reinterpret_cast<PFN_vkAllocateCommandBuffers>(
              l_dev->get_proc_addr("vkAllocateCommandBuffers")
          )
      },
      vk_begin {
          reinterpret_cast<PFN_vkBeginCommandBuffer>(
              l_dev->get_proc_addr("vkBeginCommandBuffer")
          )
      },
      bind_pipel {
          reinterpret_cast<PFN_vkCmdBindPipeline>(
              l_dev->get_proc_addr("vkCmdBindPipeline")
          )
      },
      bind_desc_sets {
          reinterpret_cast<PFN_vkCmdBindDescriptorSets>(
              l_dev->get_proc_addr("vkCmdBindDescriptorSets")
          )
      },
      pipel_barr {
          reinterpret_cast<PFN_vkCmdPipelineBarrier>(
              l_dev->get_proc_addr("vkCmdPipelineBarrier")
          )
      },
      vk_dispatch {
          reinterpret_cast<PFN_vkCmdDispatch>(
              l_dev->get_proc_addr("vkCmdDispatch")
          )
      }
{
    VkCommandBufferAllocateInfo inf {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = pool->inner(),
        .level = v(vk::CommandBufferLevel::prmry),
        .commandBufferCount = 1,
    };

    Vulkan::vk_try(alloc(l_dev->inner(), &inf, &nner),
                   "allocating command buffer from " + pool->descrptn());
    log.brk();
}

void CommandBuffer::begin()
{
    VkCommandBufferBeginInfo inf = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = v(vk::CommandBufferUsageFlag::one_time_submit),
        .pInheritanceInfo = NULL,
    };

    Vulkan::vk_try(vk_begin(nner, &inf),
                   "beginning command buffer from " + pool->descrptn());
    if (inf.flags) {
        log.indent();
        log.enter("flags", vk::cmmnd_buffer_usage_flags_cstrs(inf.flags));
    }
    log.brk();
}

void CommandBuffer::bind(ComputePipeline& p)
{
    bind_pipel(nner, VK_PIPELINE_BIND_POINT_COMPUTE, p.inner());
    log.enter("Vulkan", "binding compute pipeline to command buffer from "
              + pool->descrptn());
    log.brk();
}

void CommandBuffer::bind(ComputePipeline& p,
                         uint32_t set_bndng_offset,
                         std::vector<VkDescriptorSet> sets)
{
    bind(p);

    bind_desc_sets(nner,
                   v(vk::PipelineBindPoint::cmpte),
                   p.layout()->inner(),
                   set_bndng_offset,
                   sets.size(),
                   sets.data(),
                   0,
                   NULL);

    log.enter("Vulkan", "binding desc sets to command buffer from "
              + pool->descrptn());
    log.brk();
}

void CommandBuffer::bind(ComputePipeline& p,
                         std::vector<VkDescriptorSet> sets)
{
    bind(p, 0, sets);
}

void CommandBuffer::dispatch(uint32_t x, uint32_t y, uint32_t z)
{
    vk_dispatch(nner, x, y, z);
    log.enter("Vulkan", "recording dispatch to command buffer from "
              + pool->descrptn());
    log.indent();
    log.enter("x", x);
    if (y > 1) log.enter("y", y);
    if (z > 1) log.enter("z", z);
    log.brk();
}

void CommandBuffer::dispatch(uint32_t x, uint32_t y)
{
    dispatch(x, y, 1);
}

void CommandBuffer::dispatch(uint32_t x)
{
    dispatch(x, 1, 1);
}

void CommandBuffer::barrier(Image&                 img,
                            vk::PipelineStageFlag  src_stage,
                            vk::PipelineStageFlag  dst_stage,
                            vk::AccessFlag         src_access,
                            vk::AccessFlag         dst_access,
                            vk::ImageLayout        old_layt,
                            vk::ImageLayout        new_layt,
                            vk::ImageAspectFlag    aspect)
{
    VkImageMemoryBarrier barr {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = NULL,
        .srcAccessMask = flgs(src_access),
        .dstAccessMask = flgs(dst_access),
        .oldLayout     = v(old_layt),
        .newLayout     = v(new_layt),
        .image         = img.inner(),
        .subresourceRange = {
            .aspectMask = flgs(aspect),
            .levelCount = 1,
            .layerCount = 1,
        }
    };

    pipel_barr(nner,
               flgs(src_stage),
               flgs(dst_stage),
               1,
               0, NULL,
               0, NULL,
               1, &barr);

    log.enter("Vulkan", "recording pipeline barrier to command buffer from "
              + pool->descrptn());
    log.indent();
    log.enter("source stage", vk::pplne_stage_flag_str(src_stage));
    log.enter("dest. stage", vk::pplne_stage_flag_str(dst_stage));
    log.enter("source access", vk::access_flag_str(src_access));
    log.enter("dest. access", vk::access_flag_str(dst_access));
    log.enter("old image layout", vk::img_layout_str(old_layt));
    log.enter("new image layout", vk::img_layout_str(new_layt));
    log.enter("image aspects", vk::img_aspect_flag_str(aspect));
    log.brk();
}

} // namespace cu
