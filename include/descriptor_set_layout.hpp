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
#include "deviced.hpp"

namespace cu {

/*!
 * \brief A <a
 * href="https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkDescriptorSetLayout.html">VkDescriptorSetLayout</a>
 * wrapper.
 */
class DescriptorSetLayout : public Deviced<PFN_vkCreateDescriptorSetLayout,
                                           PFN_vkDestroyDescriptorSetLayout,
                                           VkDescriptorSetLayout> {
public:
    using ptr = std::shared_ptr<DescriptorSetLayout>;
    using binding_col = std::vector<DescriptorSetLayoutBinding>;
    using innerbinding_col = std::vector<VkDescriptorSetLayoutBinding>;;

    DescriptorSetLayout(Device::ptr l_dev,
                        std::string name,
                        const std::vector<DescriptorSetLayoutBinding>& bndgs);

    ~DescriptorSetLayout() noexcept { Deviced::dstrct(); }

    void log_attrs(unsigned indent = 1) const;

    constexpr std::string name() const { return nme; }
    const binding_col& bindings() const { return innerbs; }
    innerbinding_col innerbindings() const
    {
        innerbinding_col out(innerbs.size());
        std::transform(innerbs.begin(), innerbs.end(), out.begin(),
                       [](auto a) { return a.inner(); });
        return out;
    }

private:
    std::string nme;
    std::vector<DescriptorSetLayoutBinding> innerbs;

private:
    VkDescriptorSetLayoutCreateInfo inf;
};

} // namespace cu

#endif
