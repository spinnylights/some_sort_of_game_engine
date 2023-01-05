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

#ifndef Qb50852edc8a428cae6c2f42e3fe5a89
#define Qb50852edc8a428cae6c2f42e3fe5a89

#include "descriptor_pool.hpp"
#include "vulkan.hpp"

#include <vector>
#include <algorithm>
#include <unordered_map>

namespace cu {


DescriptorPool::DescriptorPool(Device::ptr l_dev,
                               std::vector<DescriptorSetLayout::ptr>
                                   layts)
    : Deviced(l_dev, "descriptor pool", "DescriptorPool")
{
    uint32_t max_descs = layts.size();

    std::unordered_map<VkDescriptorType, uint32_t> dt_allocs;
    for (auto l : layts) {
        for (auto b : l->bindings()) {
            if (dt_allocs.contains(b.descriptorType)) {
                dt_allocs[b.descriptorType] += b.descriptorCount;
            } else {
                dt_allocs[b.descriptorType] = b.descriptorCount;
            }
        }
    }

    std::vector<VkDescriptorPoolSize> dp_sizes(dt_allocs.size());
    std::transform(dt_allocs.cbegin(), dt_allocs.cend(), dp_sizes.begin(),
                   [](const auto& a)
                   {
                       VkDescriptorPoolSize out = {
                          .type            = a.first,
                          .descriptorCount = a.second
                       };

                       return out;
                   });

    VkDescriptorPoolCreateInfo inf {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .maxSets = max_descs,
                         // safe bc VkDescriptorType has far less than
                         // UINT32_MAX elements
        .poolSizeCount = static_cast<uint32_t>(dp_sizes.size()),
        .pPoolSizes    = dp_sizes.data(),
    };

    Vulkan::vk_try(create(dev->inner(), &inf, NULL, &nner),
                   "creating " + descrptn());

    log.indent();
    if (inf.flags) {
        log.enter("flags", vk::dscrpt_pool_create_flags_cstrs(inf.flags));
    }
    log.enter("max sets", inf.maxSets);
    log.enter("pool sizes:");
    for (const auto& s : dp_sizes) {
        log.indent(2);
        log.enter("type", vk::dscrpt_type_str(s.type));
        log.indent(3);
        log.enter("count", s.descriptorCount);
    }
    log.brk();
}

} // namespace cu

#endif
