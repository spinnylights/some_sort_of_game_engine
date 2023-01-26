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
#include "vulkan_util.hpp"

#include <vulkan/vulkan.h>

#include <bitset>

namespace cu {

class Device;

/*!
 * \brief A Vulkan image wrapper.
 */
class Image {
public:
    /*!
     * \brief See
     * [VkImageCreateInfo](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageCreateInfo.html).
     */
    struct params {

        /*!
         * \brief A
         * [VkExtent3D](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkExtent3D.html)
         * defining the bounds of the image.
         */
        VkExtent3D            extent;

        /*!
         * \brief What the image will be used for; see
         * [VkImageUsageFlagBits](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageUsageFlagBits.html)
         * and
         * [VkImageUsageFlags](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageUsageFlags.html)
         * for the details. If you're going to create an ImageView from this
         * image, the VkImageUsageFlags page gives a list of usage flags to pick
         * from.
         */
        vk::ImageUsageFlags   usage;

        /*!
         * \brief See
         * [VkImageCreateFlagBits](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageCreateFlagBits.html).
         * */
        vk::ImageCreateFlags  flags                = 0;

        /*!
         * \brief The spatial dimensionality of the image (1D, 2D, or 3D).
         */
        vk::ImageType         dimens               = vk::ImageType::twoD;

        /*!
         * \brief The in-memory format of the image data; see
         * [VkFormat](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html).
         * The default value here is for the Swapchain and maybe isn't really
         * appropriate as a general-purpose default.
         */
        vk::Format            format               = vk::Format::b8g8r8a8_srgb;

        /*!
         * \brief This creates space in the image for MIP maps. It must be at
         * least 1 at and at most ⌊log2(max(extent.width, extent.height,
         * extent.depth))⌋ + 1. Also, it doesn't actually create the MIP maps,
         * only _space_ for the MIP maps. Once we actually have MIP mapping
         * going I think maybe this should be replaced with something more
         * user-friendly.
         */
        uint32_t              mip_lvl_cnt          = 1;

        /*!
         * \brief Vulkan allows you to create an "array image" that stores
         * multiple layers of color data. This has a variety of possible uses,
         * such as cube maps or multi-view rendering. You can leave it at 1 if
         * you just want a normal image.
         */
        uint32_t              layer_cnt            = 1;

        /*!
         * \brief The number of samples per texel to take for multisample
         * anti-aliasing (MSAA). If you're planning to use this image as a
         * render target, the idea is that you can set this to a higher value to
         * get some automatic anti-aliasing (see [multisampling in the Vulkan
         * spec](https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#primsrast-multisampling),
         * [multisampling in the Vulkan
         * tutorial](https://vulkan-tutorial.com/Multisampling)). I haven't
         * actually implemented this feature yet on our side, though.
         */
        vk::SampleCountFlags  samples              =
            flgs(vk::SampleCountFlag::one);

        /*!
         * \brief Whether the image is arranged in memory "optimally," i.e. in
         * an implementation-defined GPU-friendly format, or "linearly," i.e. in
         * row-major order in memory with maybe some padding on each row. This
         * is fixed at image creation; it can't be changed later. Linear tiling
         * is much more specialized than optimal, because you can also write
         * color data into a Buffer from the host and then use e.g.
         * [vkCmdCopyBufferToImage()](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCmdCopyBufferToImage.html)
         * to copy that color data directly into an optimally-tiled Image. In
         * certain environments and for specific applications, linearly-tiled
         * images may be fast enough that they can be used without converting
         * their data to optimally-tiled first, which I think is the main use
         * case for them. I get the impression that tends to be true more with
         * integrated graphics or on mobile GPUs.
         */
        vk::ImageTiling       tiling               = vk::ImageTiling::optml;

        /*!
         * \brief An "exclusively-shared" image belongs to a specific queue
         * family at any given time, and you have to explicitly transfer
         * ownership of it to a different queue family if you want to use it
         * from that queue. "Concurrently-shared" images are created with a list
         * of queue families that can access the image simultaneously. So far
         * the engine only supports exclusively-shared images, so this part of
         * the API is kinda placeholder.
         */
        vk::SharingMode       sharing_mode         = vk::SharingMode::exclsv;

        /*!
         * \brief These are the indicies of the queues that would have access to
         * the image if you picked "concurrently-shared." I haven't really built
         * infrastructure around this yet.
         */
        std::vector<uint32_t> queue_fam_ndcies     = {};

        /*!
         * \brief The "layout" of the image in Vulkan terms (see
         * [VkImageLayout](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageLayout.html)).
         * If you're making a "fresh" image, you almost certainly want
         * vk::ImageLayout::undfnd here.
         */
        vk::ImageLayout       layout               = vk::ImageLayout::undfnd;
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

    /*!
     * \brief The size the image takes up in memory, in bytes.
     */
    VkDeviceSize mem_size() const;

    /*!
     * \brief The required alignment of the image in memory, in bytes.
     */
    VkDeviceSize alignment() const;

    /*!
     * \brief Whether the Vulkan memory type supports this image.
     */
    bool mem_type_supported(MemoryType type) const;

private:
    VkImage               _img;
    Device::ptr       _dev;

    PFN_vkDestroyImage    _destroy_img;
    PFN_vkCreateImage     _create_img;
    PFN_vkGetImageMemoryRequirements get_mem_reqs;

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

private:
    VkMemoryRequirements mem_reqs {};
    std::bitset<32>      supported_types;
};

} // namespace cu

#endif
