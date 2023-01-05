#include "pipeline_layout.hpp"

#include "vulkan.hpp"
#include "log.hpp"

namespace cu {

PipelineLayout::PipelineLayout(Device::ptr l_dev,
                       std::vector<DescriptorSetLayout::ptr> dscrpt_set_layouts)
    : dev{l_dev},
      dscr_layts{dscrpt_set_layouts},
      create_pipelayt{
          reinterpret_cast<PFN_vkCreatePipelineLayout>(
              dev->get_proc_addr("vkCreatePipelineLayout")
          )
      },
      destroy_pipelayt{
          reinterpret_cast<PFN_vkDestroyPipelineLayout>(
              dev->get_proc_addr("vkDestroyPipelineLayout")
          )
      },
      flgs {0}
{
    if (dscrpt_set_layouts.size() > UINT32_MAX) {
        throw std::runtime_error("too many descriptor set layouts");
    }

    std::vector<VkDescriptorSetLayout> inners(dscr_layts.size());
    std::transform(dscr_layts.cbegin(), dscr_layts.cend(),
                   inners.begin(),
                   [](DescriptorSetLayout::ptr l) { return l->inner(); });

    VkPipelineLayoutCreateInfo inf = {
        .sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext          = NULL,
        .flags          = flgs,
        .setLayoutCount = static_cast<uint32_t>(inners.size()),
        .pSetLayouts    = inners.data(),
        // TODO: push constants
    };

    Vulkan::vk_try(create_pipelayt(dev->inner(), &inf, NULL, &nner),
                   "create pipeline layout");

    log_attrs();
}

PipelineLayout::~PipelineLayout() noexcept
{
    log.attempt("Vulkan", "destroying pipeline layout");
    destroy_pipelayt(dev->inner(), nner, NULL);
    log.finish();
    log.brk();
}

void PipelineLayout::log_attrs(unsigned indent) const
{
    log.indent(indent);
    auto flgs_cstrs = vk::pplne_create_flags_cstrs(flgs);
    if (!flgs_cstrs.empty()) {
        log.enter("flags", flgs_cstrs);
    }
    log.enter("descriptor set layouts:");
    log.indent(indent + 1);
    for (std::size_t i = 0; i < dscr_layts.size(); ++i) {
        log.enter(std::to_string(i) + " " + dscr_layts.at(i)->name());
    }
    log.brk();
}

} // namespace cu
