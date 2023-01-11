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

#include "command_pool.hpp"
#include "vulkan.hpp"

namespace cu {

CommandPool::CommandPool(Device::ptr l_dev, Device::QueueFlavor qflav)
    : Deviced(l_dev,
              "command pool (" + l_dev->qflav_str(qflav) + ")",
              "CommandPool"),
      flv {qflav},
      GET_VK_FN_PTR(reset_pool, ResetCommandPool)

{
    VkCommandPoolCreateInfo inf = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = v(vk::CommandPoolCreateFlag::trnsnt),
        .queueFamilyIndex = l_dev->queue_ndx(flv),
    };

    Vulkan::vk_try(create(l_dev->inner(), &inf, NULL, &nner),
                   "creating " + descrptn());
    if (inf.flags) {
        log.indent();
        log.enter("flags", vk::cmmnd_pool_create_flags_cstrs(inf.flags));
    }
    log.brk();
}

void CommandPool::reset(bool release_resources)
{
    Vulkan::vk_try(reset_pool(dev->inner(), nner, release_resources),
                   "resetting command pool " + descrptn());
    log.brk();
}

} // namespace cu
