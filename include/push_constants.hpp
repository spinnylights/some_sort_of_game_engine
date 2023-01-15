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

#ifndef oabb124ca7014c518bdac93ea8e535a6
#define oabb124ca7014c518bdac93ea8e535a6

#include "pc_range.hpp"

#include <vulkan/vulkan.h>
#include <stdexcept>

namespace cu {

template<typename T>
class PushConstants : public PCRange {
public:
    PushConstants(vk::ShaderStageFlags, uint32_t offset = 0);

    T* values() { return &vs; }
    const void* values_voidp() override { return values(); }

private:
    T vs;
};

template<typename T>
PushConstants<T>::PushConstants(vk::ShaderStageFlags fs, uint32_t offs)
{
    static_assert(sizeof(T) != 0, "Vulkan PushConstants cannot have 0 size");

    static_assert(sizeof(T) % 4 == 0,
                  "PushConstants size must be a multiple of 4");

    if (rnge.offset % 4 != 0) {
        throw std::runtime_error("PushConstants offset must be a multiple of "
                                 "4");
    }

    rnge.stageFlags = fs;
    rnge.offset = offs;
    rnge.size = sizeof(T);
}

} // namespace cu

#endif
