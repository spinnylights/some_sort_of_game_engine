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

#include "image_view.hpp"

#include "image.hpp"
#include "vulkan.hpp"

#include "log.hpp"

namespace cu {

ImageView::ImageView(Image& img)
    : _dev               {img.device()},
      _create_inner   {
          reinterpret_cast<PFN_vkCreateImageView>(
              _dev->get_proc_addr("vkCreateImageView")
          )
      },
      _destroy_inner  {
          reinterpret_cast<PFN_vkDestroyImageView>(
              _dev->get_proc_addr("vkDestroyImageView")
          )
      },
      _type              {img.view_type()},
      _format            {img.format()},
      _subresource_range {img.all_subresources()},
      _should_destroy    {true}
{
    VkImageViewCreateInfo create_inf = {
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext            = NULL,
        .flags            = _flags,
        .image            = img.inner(),
        .viewType         = _type,
        .format           = _format,
        .subresourceRange = _subresource_range,
    };

    Vulkan::vk_try(_create_inner(_dev->inner(),
                                    &create_inf,
                                    NULL,
                                    &_inner),
                   "create image view");
    log.indent();
    log.enter("flags", vk::img_create_flags_cstrs(_flags));
    log.enter("type", type_str());
    log.enter("format", format_str());
    log.brk();
}

ImageView::ImageView(ImageView&& other)
    : _inner             {other.inner()},
      _dev               {other.device()},
      _create_inner   {
          reinterpret_cast<PFN_vkCreateImageView>(
              _dev->get_proc_addr("vkCreateImageView")
          )
      },
      _destroy_inner  {
          reinterpret_cast<PFN_vkDestroyImageView>(
              _dev->get_proc_addr("vkDestroyImageView")
          )
      },
      _flags             {other.flags()},
      _type              {other.type()},
      _format            {other.format()},
      _swizzle           {other.swizzle()},
      _subresource_range {other.subresource_range()},
      _should_destroy    {other.will_be_destroyed()}

{
    other.should_destroy(false);
}

ImageView::~ImageView() noexcept
{
    if (_should_destroy) {
        log.attempt("Vulkan", "destroying image view");
        _destroy_inner(_dev->inner(), _inner, NULL);
        log.finish();
        log.brk();
    }
}
} // namespace cu
