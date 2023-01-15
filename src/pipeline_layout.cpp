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

#include "pipeline_layout.hpp"

#include "vulkan.hpp"
#include "log.hpp"

#include <algorithm>
#include <array>

namespace cu {

PipelineLayout::PipelineLayout(Device::ptr l_dev,
                               std::vector<VkDescriptorSetLayout> layts,
                               std::vector<const VkPushConstantRange*> pcrs)
    : Deviced(l_dev, "pipeline layout", "PipelineLayout")
{
    VkPipelineLayoutCreateInfo inf = {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                  = NULL,
        .flags                  = 0,
        .setLayoutCount         = static_cast<uint32_t>(layts.size()),
        .pSetLayouts            = layts.data(),
        .pushConstantRangeCount = static_cast<uint32_t>(pcrs.size()),
        .pPushConstantRanges    = pcrs.empty() ? NULL : *pcrs.data(),
    };

    Vulkan::vk_try(create(dev->inner(), &inf, NULL, &nner),
                   "create " + descrptn());

    log_attrs();
}

std::vector<VkDescriptorSetLayout>
prep_descs(std::vector<DescriptorSetLayout::ptr> dscr_layts)
{
    if (dscr_layts.size() > UINT32_MAX) {
        throw std::runtime_error("too many descriptor set layouts");
    }

    std::vector<VkDescriptorSetLayout> inners(dscr_layts.size());
    std::transform(dscr_layts.cbegin(), dscr_layts.cend(), inners.begin(),
                   [](auto l) { return l->inner(); });

    return inners;
}

void check_overlapping_pc_stages(const std::vector<PCRange*>& push_consts)
{
    constexpr std::array<vk::ShaderStageFlag, 6> stages = {
        vk::ShaderStageFlag::vertex,
        vk::ShaderStageFlag::tsslltn_cntrl,
        vk::ShaderStageFlag::tsslltn_evltn,
        vk::ShaderStageFlag::gmtry,
        vk::ShaderStageFlag::frgmnt,
        vk::ShaderStageFlag::cmpte,
    };

    std::vector<vk::ShaderStageFlag> pc_stages;

    for (auto pc : push_consts) {
        for (auto stage : stages) {
            if (pc->stages() & flgs(stage)) {
                bool already_covered =
                    std::find(pc_stages.cbegin(), pc_stages.cend(), stage)
                    != std::end(pc_stages);

                if (already_covered) {
                    std::string msg = "there is already a push constant range "
                                      "covering stage "
                                      + vk::shader_stage_flag_str(stage);

                    throw std::runtime_error(msg);
                } else {
                    pc_stages.push_back(stage);
                }
            }
        }
    }
}

std::vector<const VkPushConstantRange*>
prep_pcs(std::vector<PCRange*> push_consts)
{
    if (push_consts.size() > UINT32_MAX) {
        throw std::runtime_error("too many PushConstants");
    }

    check_overlapping_pc_stages(push_consts);

    std::vector<const VkPushConstantRange*> pcrs(push_consts.size());
    std::transform(push_consts.cbegin(), push_consts.cend(), pcrs.begin(),
                   [](auto pc) { return &(pc->range()); });

    return pcrs;
}

PipelineLayout::PipelineLayout(Device::ptr l_dev,
                       std::vector<DescriptorSetLayout::ptr> dscrpt_set_layouts)
    : PipelineLayout::PipelineLayout(l_dev,
                                     prep_descs(dscrpt_set_layouts),
                                     {})
{
      dscr_layts = dscrpt_set_layouts;
}

PipelineLayout::PipelineLayout(Device::ptr l_dev,
                       std::vector<DescriptorSetLayout::ptr> dscrpt_set_layouts,
                       std::vector<PCRange*> push_consts)
    : PipelineLayout::PipelineLayout(l_dev,
                                     prep_descs(dscrpt_set_layouts),
                                     prep_pcs(push_consts))
{
      dscr_layts = dscrpt_set_layouts;
}

void PipelineLayout::log_attrs(unsigned indent) const
{
    log.indent(indent);
    log.enter("descriptor set layouts:");
    log.indent(indent + 1);
    for (std::size_t i = 0; i < dscr_layts.size(); ++i) {
        log.enter(std::to_string(i) + " " + dscr_layts.at(i)->name());
    }
    log.brk();
}

} // namespace cu
