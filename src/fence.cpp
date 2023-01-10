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

#include "fence.hpp"
#include "vulkan_util.hpp"
#include "vulkan.hpp"

namespace cu {

Fence::Fence(Device::ptr l_dev, bool signaled)
    : Deviced(l_dev, "fence", "Fence"),
      vk_wait {
          reinterpret_cast<PFN_vkWaitForFences>(
              dev->get_proc_addr("vkWaitForFences")
          )
      }
{
    vk::FenceCreateFlags flags = 0;
    if (signaled) flags = flgs(vk::FenceCreateFlag::sgnld);

    VkFenceCreateInfo inf = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = NULL,
        .flags = flags,
    };

    Vulkan::vk_try(create(dev->inner(), &inf, NULL, &nner),
                   "creating " + descrptn());
    log.brk();
}

void Fence::wait_for(uint64_t timeout)
{
    Vulkan::vk_try(vk_wait(dev->inner(), 1, &nner, VK_FALSE, timeout),
                   "waiting on fence with timeout " + std::to_string(timeout));
    log.brk();
}

void Fence::wait()
{
    Vulkan::vk_try(vk_wait(dev->inner(), 1, &nner, VK_FALSE, UINT64_MAX),
                   "waiting on fence");
    log.brk();
}

} // namespace cu
