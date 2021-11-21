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

#include "image.hpp"

#include "log.hpp"

#include "vulkan.hpp"
#include "logi_device.hpp"
#include "image_format.hpp"

namespace cu {

Image::Image(LogiDevice::ptr l_dev, const Image::params& ps)
    : _dev              {l_dev},
      _destroy_img      {
          reinterpret_cast<PFN_vkDestroyImage>(
              _dev->get_proc_addr("vkDestroyImage")
          )
      },
      _create_img      {
          reinterpret_cast<PFN_vkCreateImage>(
              _dev->get_proc_addr("vkCreateImage")
          )
      },
      _queue_fam_ndcies {ps.queue_fam_ndcies},
      _extent           {ps.extent},
      _format           {ps.format},
      _sharing_mode     {ps.sharing_mode},
      _layout           {ps.layout},
      _usage            {ps.usage},
      _flags            {ps.flags},
      _dimens           {ps.dimens},
      _samples          {ps.samples},
      _tiling           {ps.tiling},
      _mip_lvl_cnt      {ps.mip_lvl_cnt},
      _layer_cnt        {ps.layer_cnt},
      _should_destroy   {true}
{
    VkImageCreateInfo create_inf = {
        .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext                 = NULL,
        .flags                 = _flags,
        .imageType             = _dimens,
        .format                = _format,
        .extent                = _extent,
        .mipLevels             = _mip_lvl_cnt,
        .arrayLayers           = _layer_cnt,
        .samples               = _samples,
        .tiling                = _tiling,
        .usage                 = _usage,
        .sharingMode           = _sharing_mode,
        .queueFamilyIndexCount =
            static_cast<uint32_t>(_queue_fam_ndcies.size()),
        .pQueueFamilyIndices   = _queue_fam_ndcies.data(),
        .initialLayout         = _layout,
    };

    Vulkan::vk_try(_create_img(_dev->inner(), &create_inf, NULL, &_img),
                   "create image");
}

Image::Image(VkImage existing,
             LogiDevice::ptr l_dev,
             const Image::params& ps,
             bool destroy)
    : _img              {existing},
      _dev              {l_dev},
      _destroy_img      {
          reinterpret_cast<PFN_vkDestroyImage>(
              _dev->get_proc_addr("vkDestroyImage")
          )
      },
      _create_img      {
          reinterpret_cast<PFN_vkCreateImage>(
              _dev->get_proc_addr("vkCreateImage")
          )
      },
      _queue_fam_ndcies {ps.queue_fam_ndcies},
      _extent           {ps.extent},
      _format           {ps.format},
      _sharing_mode     {ps.sharing_mode},
      _layout           {ps.layout},
      _usage            {ps.usage},
      _flags            {ps.flags},
      _dimens           {ps.dimens},
      _samples          {ps.samples},
      _tiling           {ps.tiling},
      _mip_lvl_cnt      {ps.mip_lvl_cnt},
      _layer_cnt        {ps.layer_cnt},
      _should_destroy   {destroy}
{}

Image::Image(Image&& other)
    : _img              {other.inner()},
      _dev              {other.device()},
      _destroy_img      {
          reinterpret_cast<PFN_vkDestroyImage>(
              _dev->get_proc_addr("vkDestroyImage")
          )
      },
      _create_img      {
          reinterpret_cast<PFN_vkCreateImage>(
              _dev->get_proc_addr("vkCreateImage")
          )
      },
      _queue_fam_ndcies {other.queue_fam_indicies()},
      _extent           {other.extent()},
      _format           {other.format()},
      _sharing_mode     {other.sharing_mode()},
      _layout           {other.layout()},
      _usage            {other.usage()},
      _flags            {other.flags()},
      _dimens           {other.dimens()},
      _samples          {other.samples()},
      _tiling           {other.tiling()},
      _mip_lvl_cnt      {other.mip_lvl_cnt()},
      _layer_cnt        {other.layer_cnt()},
      _should_destroy   {other.will_be_destroyed()}
{
    other.should_destroy(false);
}

Image::~Image() noexcept
{
    if (_should_destroy) {
        log.attempt("Vulkan", "destroying image");
        _destroy_img(_dev->inner(), _img, NULL);
        log.finish();
        log.brk();
    }
}

VkImageSubresourceRange Image::all_subresources() const
{
    VkImageAspectFlags aspect_mask = 0;

    if (color()) {
        aspect_mask |= VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (depth_stencil()) {
        if ((ImageFormat {_format}).has_depth()) {
            aspect_mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        if ((ImageFormat {_format}).has_stencil()) {
            aspect_mask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }

    return {
            .aspectMask     = aspect_mask,
            .baseMipLevel   = 0,
            .levelCount     = VK_REMAINING_MIP_LEVELS,
            .baseArrayLayer = 0,
            .layerCount     = VK_REMAINING_ARRAY_LAYERS,
    };
}

VkImageViewType Image::view_type() const
{
    switch (_dimens) {
        case VK_IMAGE_TYPE_1D:
            return VK_IMAGE_VIEW_TYPE_1D;
        case VK_IMAGE_TYPE_2D:
            return VK_IMAGE_VIEW_TYPE_2D;
        case VK_IMAGE_TYPE_3D:
            return VK_IMAGE_VIEW_TYPE_3D;
        case VK_IMAGE_TYPE_MAX_ENUM:
            return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
    };

    // to make the compiler happy
    return VK_IMAGE_VIEW_TYPE_2D;
}

} // namespace cu
