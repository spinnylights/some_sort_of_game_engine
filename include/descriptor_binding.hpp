#ifndef abfec168d47b4b9f99d0856fd7908488
#define abfec168d47b4b9f99d0856fd7908488

#include <vulkan/vulkan.h>

namespace cu {

/*!
 * \brief An abstract representation of a VkDescriptorSetLayoutBinding.
 *
 * Currently sacrifices some of the flexibility of the
 * VkDescriptorSetLayoutBinding in favor of greater ease-of-use; we'll have to
 * see how well that works in the long term.
 */
class DescriptorBinding {
public:
    enum class Role   { sampled, raw_color, arbitrary };

    /*!
     * Right now, Role::sampled is always read-only and Role::raw_color is
     * always read-write, so this only affects anything if you're using
     * Role::arbitrary.
     */
    enum class Access { r, rw };

    // VkShaderStageFlagBits is a bitmask, so this may be too coarse
    enum class KindOfShader {
        vertex,
        tess_control,
        tess_eval,
        geometry,
        fragment,
        compute,
        any_graphics,
        any
    };

    // later i should probably take an optional immutable sampler here
    DescriptorBinding(Access access,
                      Role role,
                      KindOfShader kind_of_shader,
                      uint32_t count = 1,
                      uint32_t binding_index = 0);

    /*!
     * The binding number.
     */
    constexpr uint32_t binding_ndx() { return inner.binding; }
    
    /*!
     * Sets the binding number.
     */
    void binding_ndx(uint32_t new_ndx) { inner.binding = new_ndx; }

private:
    VkDescriptorSetLayoutBinding inner = {};
};

} // namespace cu

#endif
