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
 * Copyright (c) 2022 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef fba0bed09a08448f858961b9e1cf34b4
#define fba0bed09a08448f858961b9e1cf34b4

#include "deviced.hpp"
#include "vulkan_util.hpp"

namespace cu {

/*!
 * \brief A Vulkan
 * <a href="https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkCommandPool.html">command pool</a>
 * wrapper.
 */
class CommandPool : public Deviced<PFN_vkCreateCommandPool,
                                   PFN_vkDestroyCommandPool,
                                   VkCommandPool> {
public:
    /*!
     * \brief A pointer to the CommandPool.
     */
    using ptr = std::shared_ptr<CommandPool>;

    CommandPool(Device::ptr l_dev, Device::QueueFlavor q_flav);

    CommandPool(CommandPool&&) = delete;
    CommandPool& operator=(CommandPool&&) = delete;

    CommandPool(const CommandPool&) = delete;
    CommandPool& operator=(const CommandPool&) = delete;

    ~CommandPool() noexcept { Deviced::dstrct(); };

    Device::QueueFlavor flav() const { return flv; }

private:
    Device::QueueFlavor flv;
};

} // namespace 

#endif
