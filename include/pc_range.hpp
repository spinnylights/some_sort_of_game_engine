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

#ifndef Kd3f9aa0aa7549ce8194d01cc9944cf9
#define Kd3f9aa0aa7549ce8194d01cc9944cf9

#include "vulkan_util.hpp"

namespace cu {

class PCRange {
public:
    const VkPushConstantRange& range() const { return rnge; }

    vk::ShaderStageFlags stages() const { return rnge.stageFlags; }
    uint32_t offset() const { return rnge.offset; }
    uint32_t size() const { return rnge.size; }

    virtual const void* values_voidp() = 0;

    virtual ~PCRange() = default;

    VkPushConstantRange rnge = {};
};

} // namespace cu

#endif
