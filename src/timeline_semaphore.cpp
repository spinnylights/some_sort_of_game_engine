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

#include "timeline_semaphore.hpp"
#include "vulkan_util.hpp"
#include "vulkan.hpp"

namespace cu {

TimelineSemaphore::TimelineSemaphore(Device::ptr l_dev, uint64_t initial_val)
    : Semaphore {l_dev, initial_val},
      get_val {
          reinterpret_cast<PFN_vkGetSemaphoreCounterValue>(
              dev->get_proc_addr("vkGetSemaphoreCounterValue")
          )
      },
      vk_signal {
          reinterpret_cast<PFN_vkSignalSemaphore>(
              dev->get_proc_addr("vkSignalSemaphore")
          )
      },
      wait_sems {
          reinterpret_cast<PFN_vkWaitSemaphores>(
              dev->get_proc_addr("vkWaitSemaphores")
          )
      }
{}

uint64_t TimelineSemaphore::value() const
{
    uint64_t val;
    Vulkan::vk_try(get_val(dev->inner(), nner, &val),
                   "getting semaphore counter value");
    log.indent();
    log.enter("value", val);
    log.brk();
    return val;
}

void TimelineSemaphore::incr(uint64_t new_val, uint64_t current)
{
    if (new_val <= current) {
        throw std::runtime_error("new timeline semaphore value must be "
                                 "strictly greater than the current value");
    }

    if (new_val - current > max_val_diff()) {
        throw std::runtime_error("can't increment a timeline semaphore by more "
                                 "than the hardware allows");
    }

    VkSemaphoreSignalInfo inf {
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
        .pNext     = NULL,
        .semaphore = nner,
        .value     = new_val,
    };

    Vulkan::vk_try(vk_signal(dev->inner(), &inf),
                   "signaling semaphore with value " + std::to_string(new_val));
    log.brk();
}

void TimelineSemaphore::incr(uint64_t new_val)
{
    incr(new_val, value());
}

void TimelineSemaphore::incr()
{
    auto val = value();
    incr(val + 1, val);
}

void TimelineSemaphore::wait_for(uint64_t val_to_reach,
                             uint64_t timeout,
                             bool     show_timeout_in_log)
{
    VkSemaphoreWaitInfo inf = {
        .sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .pNext          = NULL,
        .flags          = 0,
        .semaphoreCount = 1,
        .pSemaphores    = &nner,
        .pValues        = &val_to_reach,
    };

    std::string to_log = "waiting on timeline semaphore to reach "
                         + std::to_string(val_to_reach);

    if (show_timeout_in_log) {
        to_log += " with timeout " + std::to_string(timeout);
    }

    Vulkan::vk_try(wait_sems(dev->inner(), &inf, timeout), to_log);

    log.brk();
}

void TimelineSemaphore::wait_for(uint64_t val_to_reach, uint64_t timeout)
{
    wait_for(val_to_reach, timeout, true);
}

void TimelineSemaphore::wait_for(uint64_t val_to_reach)
{
    wait_for(val_to_reach, UINT64_MAX, false);
}

} // namespace cu
