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

#include "semaphore.hpp"
#include "vulkan_util.hpp"
#include "vulkan.hpp"

namespace cu {

void Semaphore::cnstrct(const VkSemaphoreCreateInfo* inf)
{
    Vulkan::vk_try(create(dev->inner(), inf, NULL, &nner),
                   "creating " + descrptn());
    log.brk();
}

Semaphore::Semaphore(Device::ptr l_dev)
    : Deviced {l_dev, "binary semaphore", "Semaphore"}
{
    VkSemaphoreCreateInfo inf {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
    };

    cnstrct(&inf);
}

Semaphore::Semaphore(Device::ptr l_dev, uint64_t initial_val)
    : Deviced {l_dev, "timeline semaphore", "Semaphore"}
{
    VkSemaphoreTypeCreateInfo type_inf {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext = NULL,
        .semaphoreType = v(vk::SemaphoreType::tmlne),
        .initialValue = initial_val,
    };

    VkSemaphoreCreateInfo inf {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &type_inf,
        .flags = 0,
    };

    cnstrct(&inf);
}

} // namespace cu
