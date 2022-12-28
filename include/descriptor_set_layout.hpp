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

#ifndef Rddad8715b474325bb13b3094ee573f2
#define Rddad8715b474325bb13b3094ee573f2

#include <vulkan/vulkan.hpp>

#include <vector>

#include "descriptor_set_layout_binding.hpp"
#include "device.hpp"

namespace cu {

class DescriptorSetLayout {
public:
    using ptr = std::shared_ptr<DescriptorSetLayout>;

    DescriptorSetLayout(Device::ptr l_dev,
                        std::string name,
                        const std::vector<DescriptorSetLayoutBinding>& bndgs);

    ~DescriptorSetLayout() noexcept;

    constexpr VkDescriptorSetLayout inner() { return nner; }

    void log_attrs(unsigned indent = 1) const;

    constexpr std::string name() const { return nme; }

private:
    VkDescriptorSetLayout nner;

private:
    Device::ptr dev;

private:
    std::string nme;

private:
    PFN_vkCreateDescriptorSetLayout create_desc_set_layout;
    PFN_vkDestroyDescriptorSetLayout destroy_desc_set_layout;

private:
    VkDescriptorSetLayoutCreateInfo inf;
};

} // namespace cu

#endif
