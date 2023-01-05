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

#include "descriptor_set_layout.hpp"
#include "vulkan.hpp"
#include "log.hpp"
#include "vulkan_util.hpp"

#include <algorithm>

namespace cu {

DescriptorSetLayout::DescriptorSetLayout(Device::ptr l_dev,
                                         std::string name,
                                 const std::vector<DescriptorSetLayoutBinding>& bndgs)
    : Deviced(l_dev, "descriptor set layout '" + name + "'", "DescriptorSetLayout"),
      nme {name}
{
    if (bndgs.size() > UINT32_MAX) {
        throw std::runtime_error("layout binding vector is too large");
    }

    std::vector<VkDescriptorSetLayoutBinding> innerbs(bndgs.size());
    std::transform(bndgs.cbegin(), bndgs.cend(),
                   innerbs.begin(),
                   [](const DescriptorSetLayoutBinding& b) { return b.inner(); });

    inf = {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext        = NULL,
        .flags        = 0,
        .bindingCount = static_cast<uint32_t>(innerbs.size()),
        .pBindings    = innerbs.data(),
    };

    Vulkan::vk_try(create(dev->inner(), &inf, NULL, &nner),
                   "creating descriptor set layout");

    log.indent();
    log.enter("name", "'" + nme + "'");
    log.enter("flags",
              vk::dscrpt_set_layout_create_flags_cstrs(inf.flags));
    log.enter("# of bindings", innerbs.size());
    log.enter("descriptors:");
    for (const auto& b : bndgs) {
        constexpr auto indent_bndg_log = 2;
        b.log_attrs(indent_bndg_log);
    }
}

} // namespace cu
