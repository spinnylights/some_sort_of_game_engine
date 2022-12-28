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
      }
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
        .flags          = 0,
        .setLayoutCount = static_cast<uint32_t>(dscrpt_set_layouts.size()),
        .pSetLayouts    = inners.data(),
        // TODO: push constants
    };

    Vulkan::vk_try(create_pipelayt(dev->inner(), &inf, NULL, &nner),
                   "create pipeline layout");

    log.indent();
    log.enter("flags", inf.flags);
    log.enter("descriptor set layouts", inf.setLayoutCount);
    log.indent(2);
    for (const auto& l : dscr_layts) {
        log.enter(l->name());
    }
    log.brk();
}

PipelineLayout::~PipelineLayout() noexcept
{
    log.attempt("Vulkan", "destroying pipeline layout");
    destroy_pipelayt(dev->inner(), nner, NULL);
    log.finish();
    log.brk();
}

} // namespace cu
