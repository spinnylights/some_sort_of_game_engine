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

#ifndef J2f15c99194b4bc4bef27446c0d923d0
#define J2f15c99194b4bc4bef27446c0d923d0

#include <string>

#include <vulkan/vulkan.h>

namespace cu {

struct PhysDevice {
    VkPhysicalDevice inner;
    std::string name;
    VkPhysicalDeviceType type;
    VkDeviceSize mem;
};

} // namespace cu

#endif
