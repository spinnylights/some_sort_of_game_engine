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

#ifndef nf881038f94847228e50390dc7437b5e
#define nf881038f94847228e50390dc7437b5e

#include <vulkan/vulkan.h>

#include <cstdint>

namespace cu {

class Surface;

class QueueFamily {
public:
    QueueFamily(VkQueueFamilyProperties&,
                uint32_t ndex,
                VkPhysicalDevice,
                Surface&);

    bool graphics() const;
    bool compute() const;
    bool transfer() const;
    bool sparse_binding() const;
    bool protected_memory() const;

    uint32_t flag_count() const;

    uint32_t queue_count()       const { return queue_cnt; }
    uint32_t index()             const { return ndx; }
    bool     present_supported() const { return pres_support; }

    void log_info() const;

private:
    VkQueueFlags flags;
    uint32_t     queue_cnt;
    uint32_t     ndx;
    bool         pres_support;
};

} // namespace cu

#endif
