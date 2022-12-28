#ifndef abfec168d47b4b9f99d0856fd7908488
#define abfec168d47b4b9f99d0856fd7908488

#include <vulkan/vulkan.h>

#include "vulkan_util.hpp"

namespace cu {

/*!
 * \brief A VkDescriptorSetLayoutBinding wrapper.
 */
class DescriptorSetLayoutBinding {
public:
    struct DescriptorSetLayoutBindingArgs {
        uint32_t binding_ndx;
        vk::DescriptorType type;
        uint32_t count;
        vk::ShaderStageFlags shader_stages;
        // TODO: immutable samplers
    };

    DescriptorSetLayoutBinding(DescriptorSetLayoutBindingArgs args);

    /*!
     * The binding number.
     */
    constexpr uint32_t binding_ndx()
    {
        return innr.binding;
    }

    /*!
     * Sets the binding number.
     */
    void binding_ndx(uint32_t new_ndx) { innr.binding = new_ndx; }

    /*!
     * The underlying Vulkan struct.
     */
    constexpr VkDescriptorSetLayoutBinding inner() { return innr; }

    void log_attrs(unsigned indent = 1) const;

private:
    VkDescriptorSetLayoutBinding innr;
};

} // namespace cu

#endif
