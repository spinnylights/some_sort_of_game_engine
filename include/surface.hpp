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

#ifndef b1f51f6bee864d8cb4da54dd7c09f340
#define b1f51f6bee864d8cb4da54dd7c09f340

#include <vulkan/vulkan.h>

namespace cu {

class SDL;
class Instance;

class Surface {
public:
    Surface(SDL&, Instance&);

    Surface(Surface&&) = delete;
    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;

    ~Surface() noexcept;

    VkSurfaceKHR inner() { return surf; }

private:
    VkSurfaceKHR surf;
    VkInstance inst;
};

} // namespace cu

#endif
