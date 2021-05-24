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
 * Copyright (c) 2021 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef ac994460237c45fcb74600d711cd72af
#define ac994460237c45fcb74600d711cd72af

#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace cu {

class Instance {
public:
    Instance(std::vector<const char*> exts, std::vector<const char*> layers);

    Instance(Instance&&) = delete;
    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    ~Instance() noexcept;

    VkInstance inner() { return inst; }

private:
    VkInstance inst;
};

} // namespace cu

#endif
