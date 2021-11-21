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

#ifndef Sbc8f25127734054a45054192569a723
#define Sbc8f25127734054a45054192569a723

#include "logi_device.hpp"

#include <vulkan/vulkan.h>

namespace cu {

class Image;

/*!
 * \brief A Vulkan image view wrapper.
 */
class ImageView {
public:
    /*!
     * \brief (constructor) Create a new image view that fully covers the
     * referenced image.
     */
    ImageView(Image&);

    ImageView(ImageView&&);

    ImageView(const ImageView&)            = delete;
    ImageView& operator=(const ImageView&) = delete;

    ~ImageView() noexcept;

    /*!
     * \brief Returns the handle wrapped by the class.
     */
    VkImageView inner() { return _inner; }

    /*!
     * \brief Returns a shared pointer to the device used to create the image
     * view.
     */
    LogiDevice::ptr device() const { return _dev; }

    /*!
     * \brief Miscellaneous (Vulkan) properties of the image view, expressed
     * as a bitmask.
     */
    VkImageViewCreateFlags flags() const { return _flags; }

    /*!
     * \brief The (Vulkan) type of image view this is (1–3D, cube, etc.).
     */
    VkImageViewType type() const { return _type; }

    /*!
     * \brief The image view's format (i.e. the manner in which it represents
     * the data in memory of the viewed image).
     */
    VkFormat format() const { return _format; }

    /*!
     * \brief The manner in which the viewed image's color components
     * are swizzled in the view.
     */
    VkComponentMapping swizzle() const { return _swizzle; }

    /*!
     * \brief The subresource range of the viewed image represented in the view.
     */
    VkImageSubresourceRange subresource_range() const
    {
        return _subresource_range;
    }

    /*!
     * \brief Whether the underlying Vulkan image view will be destroyed in this
     * instance's destructor or not.
     */
    bool will_be_destroyed() const { return _should_destroy; }

    /*!
     * \brief Whether the underlying Vulkan image view should be destroyed in
     * this instance's destructor or not.
     */
    void should_destroy(bool destroy) { _should_destroy = destroy; }

private:
    VkImageView             _inner;
    LogiDevice::ptr         _dev;

    PFN_vkCreateImageView   _create_inner;
    PFN_vkDestroyImageView  _destroy_inner;

    VkImageViewCreateFlags  _flags              = 0;
    VkImageViewType         _type;
    VkFormat                _format;
    VkComponentMapping      _swizzle            = {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY,
    };
    VkImageSubresourceRange _subresource_range;

    bool                    _should_destroy;
};

} // namespace cu

#endif
