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

    DescriptorSetLayoutBinding(const DescriptorSetLayoutBinding&) = default;
    DescriptorSetLayoutBinding&
        operator=(const DescriptorSetLayoutBinding&) = default;

    DescriptorSetLayoutBinding(DescriptorSetLayoutBinding&&) = default;
    DescriptorSetLayoutBinding&
        operator=(DescriptorSetLayoutBinding&&) = default;

    /*!
     * The binding number.
     */
    constexpr uint32_t binding_ndx()
    {
        return nner.binding;
    }

    /*!
     * Sets the binding number.
     */
    void binding_ndx(uint32_t new_ndx) { nner.binding = new_ndx; }

    /*!
     * The underlying Vulkan struct.
     */
    constexpr VkDescriptorSetLayoutBinding inner() const { return nner; }

    void log_attrs(unsigned indent = 1) const;

private:
    VkDescriptorSetLayoutBinding nner;
};

} // namespace cu

#endif
