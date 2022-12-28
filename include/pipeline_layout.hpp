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

#ifndef S8d32c3f4f19484b95be7cfec3551dba
#define S8d32c3f4f19484b95be7cfec3551dba

#include "vulkan_util.hpp"
#include "descriptor_set_layout.hpp"
#include "device.hpp"

#include <vector>
#include <algorithm>

namespace cu {

class PipelineLayout {
public:
    struct Args {
        std::vector<DescriptorSetLayout::ptr> dscrpt_set_layouts;
        // TODO: push constants
    };

    PipelineLayout(Device::ptr l_dev,
                   std::vector<DescriptorSetLayout::ptr> dscrpt_set_layouts);

    ~PipelineLayout() noexcept;

private:
    Device::ptr dev;

private:
    std::vector<DescriptorSetLayout::ptr> dscr_layts;

private:
    VkPipelineLayout nner;

private:
    PFN_vkCreatePipelineLayout  create_pipelayt;
    PFN_vkDestroyPipelineLayout destroy_pipelayt;
};

} // namespace cu

#endif
