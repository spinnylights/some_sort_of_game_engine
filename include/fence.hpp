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

#ifndef odb30e1911be49889c5c9aa58c15ef3a
#define odb30e1911be49889c5c9aa58c15ef3a

#include "deviced.hpp"

namespace cu {

class Fence : public Deviced<PFN_vkCreateFence, PFN_vkDestroyFence, VkFence> {
public:
    Fence(Device::ptr l_dev, bool signaled = false);

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

    Fence(Fence&&) = delete;
    Fence& operator=(Fence&&) = delete;

    ~Fence() noexcept { Deviced::dstrct(); }

    /*!
     * \brief Waits on this fence (blocking) until the fence is signaled. Can
     * hang the thread if nothing ever signals the fence. Automatically resets
     * the fence afterwards.
     */
    void wait();

    /*!
     * \brief Waits on this fence (blocking) for ns nanoseconds, then continues.
     * Automatically resets the fence afterwards.
     */
    void wait_for(uint64_t ns);

private:
    PFN_vkWaitForFences vk_wait;
    PFN_vkResetFences   vk_reset;

    void reset();
};

} // namespace cu

#endif
