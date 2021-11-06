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

#include "logi_device.hpp"

#include <vulkan/vulkan.h>

namespace cu {

class LogiDevice;

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
    Image(LogiDevice::ptr l_dev, const params& ps);

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
          LogiDevice::ptr l_dev,
          const params& ps,
          bool destroy = false);

    ~Image() noexcept;
private:
    VkImage               _img;
    LogiDevice::ptr       _dev;

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
