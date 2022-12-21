#include "descriptor_binding.hpp"

#include "log.hpp"

namespace cu {

DescriptorBinding::DescriptorBinding(Access access,
                                     Role role,
                                     KindOfShader kind_of_shader,
                                     uint32_t count,
                                     uint32_t binding_index)
{
    using enum Role;
    using enum Access;
    using enum KindOfShader;

    log.enter("new descriptor bind point");
    log.indent();

    inner.binding = binding_index;
    log.enter("binding", inner.binding);

    // this might be too simple for now but we'll see
    switch(role) {
    case sampled:
        inner.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        log.enter("role", std::string("sampled"));
        break;
    case raw_color:
        inner.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        log.enter("role", std::string("raw color"));
        break;
    case arbitrary:
        switch(access) {
        case r:
            inner.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            log.enter("role", std::string("r-only arbitrary"));
            break;
        case rw:
            inner.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            log.enter("role", std::string("r/w arbitrary"));
            break;
        }
        break;
    }

    inner.descriptorCount = count;
    log.enter("count", inner.descriptorCount);

    switch(kind_of_shader) {
    case vertex:
        inner.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        log.enter("kind of shader", std::string("vertex"));
        break;
    case tess_control:
        inner.stageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        log.enter("kind of shader", std::string("tessellation control"));
        break;
    case tess_eval:
        inner.stageFlags = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        log.enter("kind of shader", std::string("tessellation evaluation"));
        break;
    case geometry:
        inner.stageFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
        log.enter("kind of shader", std::string("geometry"));
        break;
    case fragment:
        inner.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        log.enter("kind of shader", std::string("fragment"));
        break;
    case compute:
        inner.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        log.enter("kind of shader", std::string("compute"));
        break;
    case any_graphics:
        inner.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
        log.enter("kind of shader", std::string("any graphics"));
        break;
    case any:
        inner.stageFlags = VK_SHADER_STAGE_ALL;
        log.enter("kind of shader", std::string("any"));
        break;
    }

    log.brk();
}

} // namespace cu
