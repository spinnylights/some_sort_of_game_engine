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

} // namespace cu
