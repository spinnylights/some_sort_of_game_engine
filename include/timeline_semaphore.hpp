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

#ifndef F499663386b94ee8a21953d76da122d3
#define F499663386b94ee8a21953d76da122d3

#include "semaphore.hpp"

namespace cu {

class TimelineSemaphore : public Semaphore {
public:
    explicit TimelineSemaphore(Device::ptr l_dev, uint64_t initial_val = 0);

    TimelineSemaphore(const TimelineSemaphore&) = delete;
    TimelineSemaphore& operator=(const TimelineSemaphore&) = delete;

    TimelineSemaphore(TimelineSemaphore&&) = delete;
    TimelineSemaphore& operator=(TimelineSemaphore&&) = delete;

    ~TimelineSemaphore() noexcept { Deviced::dstrct(); };

    uint64_t max_val_diff() const
    {
        return dev->max_timel_sem_val_diff();
    }

    uint64_t value() const;

    void incr();
    void incr(uint64_t new_val);

    void wait_for(uint64_t val_to_reach);
    void wait_for(uint64_t val_to_reach, uint64_t timeout);
    void wait_for(uint64_t val_to_reach,
              uint64_t timeout,
              bool show_timeout_in_log);

private:
    void incr(uint64_t new_val, uint64_t current_val);

private:
    PFN_vkGetSemaphoreCounterValue get_val;
    PFN_vkSignalSemaphore          vk_signal;
    PFN_vkWaitSemaphores           wait_sems;
};

} // namespace cu

#endif
