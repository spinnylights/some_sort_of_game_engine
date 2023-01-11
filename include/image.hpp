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
 * Copyright (c) 2021 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef g78ee18adaaf43c58cfaf32de84d74b2
#define g78ee18adaaf43c58cfaf32de84d74b2

#include "device.hpp"

#include <vulkan/vulkan.h>

#include "vulkan_util.hpp"

namespace cu {

class Device;

/*!
 * \brief A Vulkan image wrapper.
 */
class Image {
public:
    struct params {
        VkExtent3D            extent;
        VkImageUsageFlags     usage;
        VkImageCreateFlags    flags                = 0;
        VkImageType           dimens               = VK_IMAGE_TYPE_2D;
        VkFormat              format               = VK_FORMAT_B8G8R8A8_SRGB;
        uint32_t              mip_lvl_cnt          = 1;
        uint32_t              layer_cnt            = 1;
        VkSampleCountFlagBits samples              = VK_SAMPLE_COUNT_1_BIT;
        VkImageTiling         tiling               = VK_IMAGE_TILING_OPTIMAL;
        VkSharingMode         sharing_mode         = VK_SHARING_MODE_EXCLUSIVE;
        std::vector<uint32_t> queue_fam_ndcies     = {};
        VkImageLayout         layout               = VK_IMAGE_LAYOUT_UNDEFINED;
    };

    /*!
     * \brief (constructor) Create a new image, without any bound memory.
     *
     * \param l_dev The logical device you want to create the image with.
     * \param ps    The characteristics you want the image to have.
     */
    Image(Device::ptr l_dev, const params& ps);

    /*!
     * \brief (constructor) Wrap an existing VkImage, in order to
     * handle `vkGetSwapchainImagesKHR` and the like.
     *
     * \param existing An already-initialized VkImage.
     * \param l_dev    The logical device used to create the image.
     * \param ps       The image's characteristics.
     * \param destroy  Whether the VkImage should be destroyed in the Image's
     *                 destructor (Swapchain images are destroyed along with the
     *                 Swapchain).
     */
    Image(VkImage existing,
          Device::ptr l_dev,
          const params& ps,
          bool destroy = false);

    Image(const Image&)            = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&&);
    Image& operator=(Image&&);

    ~Image() noexcept;

    /*!
     * \brief The handle wrapped by the class.
     */
    VkImage inner() { return _img; }

    /*!
     * \brief A shared pointer to the device used to create the image.
     */
    Device::ptr device() const { return _dev; }

    /*!
     * \brief The indicies of the queue families that will access the image, if
     * the image is in the concurrent sharing mode.
     */
    std::vector<uint32_t> queue_fam_indicies() const
    {
        return _queue_fam_ndcies;
    }

    /*!
     * \brief The element count on each axis of the image.
     */
    constexpr VkExtent3D extent() { return _extent; }

    /*!
     * \brief The image's format (i.e. its representation in memory).
     */
    constexpr VkFormat format() { return _format; }

    /*!
     * \brief Whether the image can be accessed by more than one queue family
     * simultaneously.
     */
    constexpr VkSharingMode sharing_mode() { return _sharing_mode; }

    /*!
     * \brief How the image is laid out in memory, at a high level.
     */
    constexpr VkImageLayout layout() { return _layout; }

    /*!
     * \brief What the image can be used for (a color attachment, storage image,
     * etc.).
     */
    constexpr VkImageUsageFlags usage() { return _usage; }

    /*!
     * \brief Miscellaneous (Vulkan) properties of the image expressed as a
     * bitmask.
     */
    constexpr VkImageCreateFlags flags() { return _flags; }

    /*!
     * \brief Returns the image's dimensionality (i.e. its Vulkan image type).
     */
    constexpr VkImageType dimens() { return _dimens; }

    /*!
     * \brief Returns the image's Vulkan image type (i.e. its dimensionality).
     */
    constexpr VkImageType image_type() { return _dimens; }

    /*!
     * \brief Returns the image's Vulkan image type/dimensionality as a string.
     */
    constexpr std::string image_type_str()
    {
        return vk::img_type_str(image_type());
    }

    /*!
     * \brief Returns the image's dimensionality/Vulkan image type as a string.
     */
    constexpr std::string dimens_str() { return image_type_str(); }

    /*!
     * \brief The image's number of samples per texel.
     */
    constexpr VkSampleCountFlagBits samples() { return _samples; }

    /*!
     * \brief Whether the image's texels are laid out linearly or for efficient
     * device access in memory.
     */
    constexpr VkImageTiling tiling() { return _tiling; }

    /*!
     * \brief The number of MIP levels available within the image.
     */
    constexpr uint32_t mip_lvl_cnt() { return _mip_lvl_cnt; }

    /*!
     * \brief The number of (Vulkan) layers available within the image.
     */
    constexpr uint32_t layer_cnt() { return _layer_cnt; }

    /*!
     * \brief Whether the underlying Vulkan image will be destroyed in this
     * instance's destructor or not.
     */
    constexpr bool will_be_destroyed() { return _should_destroy; }

    /*!
     * \brief Whether the underlying Vulkan image should be destroyed in this
     * instance's destructor or not.
     */
    void should_destroy(bool destroy) { _should_destroy = destroy; }

    /*!
     * \brief Whether the image can be used as a color attachment.
     */
    bool color() const
    {
        return _usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
               || _usage & VK_IMAGE_USAGE_STORAGE_BIT
               || _usage & VK_IMAGE_USAGE_SAMPLED_BIT;
    }

    /*!
     * \brief Whether the image can be used as a depth/stencil attachment.
     */
    bool depth_stencil() const
    {
        return _usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }

    /*!
     * \brief A subresource range specifying all valid subresources of the
     * image.
     */
    VkImageSubresourceRange all_subresources() const;

    /*!
     * \brief A Vulkan image view type matching the image's dimensionality.
     */
    VkImageViewType view_type() const;

private:
    VkImage               _img;
    Device::ptr       _dev;

    PFN_vkDestroyImage    _destroy_img;
    PFN_vkCreateImage     _create_img;

    std::vector<uint32_t> _queue_fam_ndcies;
    VkExtent3D            _extent;
    VkFormat              _format;
    VkSharingMode         _sharing_mode;
    VkImageLayout         _layout;
    VkImageUsageFlags     _usage;
    VkImageCreateFlags    _flags;
    VkImageType           _dimens;
    VkSampleCountFlagBits _samples;
    VkImageTiling         _tiling;
    uint32_t              _mip_lvl_cnt;
    uint32_t              _layer_cnt;

    bool                  _should_destroy;
};

} // namespace cu

#endif
