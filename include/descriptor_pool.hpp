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

#ifndef M48c4c7ad27e47a6ac9e93f9d27e4845
#define M48c4c7ad27e47a6ac9e93f9d27e4845

#include "vulkan_util.hpp"
#include "deviced.hpp"
#include "descriptor_set_layout.hpp"
#include "image_view.hpp"

#include <initializer_list>
#include <unordered_map>
#include <string>

namespace cu {

class DescriptorPool : Deviced<PFN_vkCreateDescriptorPool,
                               PFN_vkDestroyDescriptorPool,
                               VkDescriptorPool> {
public:
    DescriptorPool(Device::ptr l_dev,
                   std::vector<DescriptorSetLayout::ptr> layts);

    // write {
    //     set #
    //     binding #
    //     array ndx / byte offset
    //     descriptor cnt
    //     descriptor type
    //     img/buffer/tex_buffer const ptrs
    // }
    //
    // copy {
    //     src set #
    //     src binding #
    //     src array ndx / byte offset
    //     dst set #
    //     dst binding #
    //     dst array ndx / byte offset
    //     descriptor cnt
    // }
    //
    // VkDescriptorImageInfo img_inf {
    //     // used with VK_DESCRIPTOR_TYPE
    //     // * SAMPLER and a sampler
    //     // * COMBINED_IMAGE_SAMPLER and a sampler and image view,
    //     // * SAMPLED_IMAGE and an image view with SAMPLED usage
    //     // * STORAGE_IMAGE and an image view with identity swizzle, STORAGE
    //     //   usage
    //     // * INPUT_ATTACHMENT and an image view with identity swizzle,
    //     //   exactly one aspect, INPUT_ATTACHMENT usage
    //     .sampler     = sampler handle, types SAMPLER or
    //                    COMBINED_IMAGE_SAMPLER (if no immut. smplrs.)
    //     .imageView   = image view, types SAMPLED_IMAGE, STORAGE IMAGE,
    //                    COMBINED_IMAGE_SAMPLER, INPUT_ATTACHMENT
    //     .imageLayout = image layout, types SAMPLED_IMAGE, STORAGE IMAGE,
    //                    COMBINED_IMAGE_SAMPLER, INPUT_ATTACHMENT
    // };
    //
    // VkDescriptorBufferInfo buff_inf {
    //     // used with VK_DESCRIPTOR_TYPE
    //     // * UNIFORM_BUFFER(_DYNAMIC) and a buffer, offset within
    //     //   minUniformBufferOffsetAlignment, buffer non-sparse implies bound
    //     //   "completely and contiguously" to a single VkDeviceMemory,
    //     //   UNIFORM_BUFFER usage
    //     // * STORAGE_BUFFER(_DYNAMIC) and a buffer, offset within
    //     //   minUniformBufferOffsetAlignment, buffer non-sparse implies bound
    //     //   "completely and contiguously" to a single VkDeviceMemory,
    //     //   STORAGE_BUFFER usage, range leq to maxStorageBufferRange,
    //     //   identity swizzle
    // };
    //
    // pTexelBufferView {
    //     // used with VK_DESCRIPTOR_TYPE
    //     // * UNIFORM_TEXEL_BUFFER with a buffer view, UNIFORM_TEXEL_BUFFER
    //     //   usage
    //     // * STORAGE_TEXEL_BUFFER with a buffer view, STORAGE_TEXEL_BUFFER
    //     //   usage
    // };
    //
    // :::storage image write to compute shader:::
    //
    // VkDescriptorImageInfo img_inf {
    //     .sampler     = VK_NULL_HANDLE,
    //     .imageView   = view,
    //     .imageLayout = VK_IMAGE_LAYOUT_GENERAL,
    // };
    //
    // VkWriteDescriptorSet write {
    //     .dstSet           = set,
    //     .dstBinding       = bndng_ndx,
    //     .dstArrayElement  = 0,
    //     .descriptorCount  = 1,
    //     .descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    //     .pImageInfo       = &img_inf,
    //     .pBufferInfo      = nullptr,
    //     .pTexelBufferView = nullptr,
    // };
    //

    // imperative interface
    //
    // pool.write().input_attachment(/* ... */)
    //             .storage_image(/* ... */)
    //             .storage_buffer(/* ... */)
    //     .copy(/* ... */)
    //     .submit();

    struct WriteCopyOps {
        WriteCopyOps(DescriptorPool& p) : pool {p} {}

        WriteCopyOps(const WriteCopyOps&) = delete;
        WriteCopyOps& operator=(const WriteCopyOps&) = delete;

        WriteCopyOps(WriteCopyOps&&) = delete;
        WriteCopyOps& operator=(WriteCopyOps&&) = delete;

        WriteCopyOps& storage_image(std::string desc_name,
                                    uint32_t binding_ndx,
                                    uint32_t offset,
                                    ImageView* v);

        void submit();

        std::vector<VkWriteDescriptorSet> writes;
        std::vector<VkDescriptorImageInfo> img_infs;
        std::vector<VkCopyDescriptorSet> copies;

    private:
        DescriptorPool& pool;

        void clear();
    };

    ~DescriptorPool() noexcept { Deviced::dstrct(); };

    VkDescriptorSet operator[](std::string name);

    void update();

    WriteCopyOps& write() { return wc_ops; }

private:
    std::unordered_map<std::string, VkDescriptorSet*> desc_set_map;
    std::vector<VkDescriptorSet> desc_sets;

private:
    WriteCopyOps wc_ops;

private:
    PFN_vkAllocateDescriptorSets alloc_desc_sets;
    PFN_vkUpdateDescriptorSets   upd_desc_sets;
};

} // namespace cu 

#endif
