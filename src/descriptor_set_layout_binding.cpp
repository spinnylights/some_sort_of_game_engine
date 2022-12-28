#include "descriptor_set_layout_binding.hpp"

#include "log.hpp"

namespace cu {

DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(DescriptorSetLayoutBindingArgs args)
{
    innr = {
        .binding         = args.binding_ndx,
        .descriptorType  = v(args.type),
        .descriptorCount = args.count,
        .stageFlags      = args.shader_stages,
    };

    log.enter("Vulkan: new descriptor set layout binding");
    log_attrs();
}

void DescriptorSetLayoutBinding::log_attrs(unsigned indent) const
{
    log.indent(indent);
    log.enter("binding", innr.binding);
    log.enter("type", vk::dscrpt_type_str(innr.descriptorType));
    log.enter("count", innr.descriptorCount);
    log.enter("shader stages", vk::shader_stage_flags_cstrs(innr.stageFlags));
    log.brk();
}

} // namespace cu
