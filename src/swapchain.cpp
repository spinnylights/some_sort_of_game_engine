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

#include "swapchain.hpp"

#include "log.hpp"

#include "phys_device.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "vulkan.hpp"

#include <vector>
#include <algorithm>

namespace cu {

void Swapchain::create(VkSwapchainKHR old_swch)
{
    const auto surface_caps = surf.capabilities(p_dev);
    extent = surface_caps.currentExtent;
    uint32_t   min_img_cnt  = 2;
    bool limited_max_imgs = surface_caps.maxImageCount != 0;
    if (limited_max_imgs && surface_caps.maxImageCount < min_img_cnt) {
        min_img_cnt = surface_caps.maxImageCount;
    }

    // support for VK_PRESENT_MODE_FIFO_KHR is required by the
    // spec (see VkSwapchainCreateInfoKHR in 33.9 WSI Swapchain)
    VkPresentModeKHR pres_mode = VK_PRESENT_MODE_FIFO_KHR;

    {
        const auto pres_modes = surf.present_modes(p_dev);

        const bool mailbox_supported =
            std::find(begin(pres_modes),
                      end(pres_modes),
                      VK_PRESENT_MODE_MAILBOX_KHR) != end(pres_modes);

        if (mailbox_supported) {
            pres_mode = VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    log.enter("Vulkan: using present mode "
              + vk::prsnt_mode_str(pres_mode));

    log.brk();

    VkSwapchainCreateInfoKHR create_info {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext                 = NULL,
        .flags                 = 0,
        .surface               = surf.inner(),
        .minImageCount         = min_img_cnt,

        // The Vulkan spec requires that all implementations support
        // the sRGB pixel format and transfer function (see
        // VkColorSpaceKHR in section 33.5.2 Surface Format Support).
        // sRGB is also the only color space supported by my hardware
        // at the time of writing. Therefore, for the sake of both
        // simplicity and testing demands, we go with sRGB in all
        // cases here. I know there are some cards and displays out
        // there right now that support fancier color spaces, but
        // that will have to wait until Lily and I can actually get
        // our hands on such things.
        .imageFormat           = VK_FORMAT_B8G8R8A8_SRGB,
        .imageColorSpace       = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,

        .imageExtent           = extent,
        .imageArrayLayers      = 1,

        // this will probably need to be changed later
        //.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageUsage            = VK_IMAGE_USAGE_TRANSFER_DST_BIT
                                 | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = NULL,
        .preTransform          = surface_caps.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode           = pres_mode,
        .clipped               = VK_TRUE,

        .oldSwapchain          = old_swch,
        //.oldSwapchain          = VK_NULL_HANDLE,
    };

    _img_views.clear();
    imgs.clear();

    VkSwapchainKHR new_swch;
    Vulkan::vk_try(create_swch(dev->inner(), &create_info, NULL, &new_swch),
                   "creating swapchain");
    log.indent();
    log.enter("min image count", min_img_cnt);
    log.enter("image format", vk::format_str(create_info.imageFormat));
    log.enter("image color space",
              vk::color_space_str(create_info.imageColorSpace));
    log.enter("image extent");
    log.indent(2);
    log.enter("width", create_info.imageExtent.width);
    log.enter("height", create_info.imageExtent.height);
    log.indent(1);
    log.enter("image array layers", create_info.imageArrayLayers);
    log.enter("image usage",
              vk::img_usage_flags_cstrs(create_info.imageUsage));
    log.enter("image sharing mode",
              vk::shrng_mode_str(create_info.imageSharingMode));
    log.enter("present mode",
              vk::prsnt_mode_str(create_info.presentMode));
    log.brk();

    if (old_swch != VK_NULL_HANDLE) {
        log.attempt("Vulkan", "destroying old swapchain");
        destroy_swch(dev->inner(), old_swch, NULL);
        log.finish();
        log.brk();
    }

    swch = std::move(new_swch);

    uint32_t imgs_cnt;
    Vulkan::vk_try(get_swch_imgs(dev->inner(), swch, &imgs_cnt, NULL),
                   "getting swapchain images count");
    log.indent();
    log.enter("images count", imgs_cnt);
    log.brk();

    std::vector<VkImage> vk_imgs;
    vk_imgs.resize(imgs_cnt);
    Vulkan::vk_try(get_swch_imgs(dev->inner(),
                                 swch,
                                 &imgs_cnt,
                                 vk_imgs.data()),
                   "getting swapchain images");
    log.brk();

    Image::params ps {
        .extent           = {
            .width  = create_info.imageExtent.width,
            .height = create_info.imageExtent.height,
            .depth  = 1,
        },
        .usage            = create_info.imageUsage,
        .flags            = 0,
        .format           = static_cast<vk::Format>(create_info.imageFormat),
        .mip_lvl_cnt      = 1,
        .layer_cnt        = create_info.imageArrayLayers,
        .samples          = flgs(vk::SampleCountFlag::one),
        .tiling           = vk::ImageTiling::optml,
        .sharing_mode     =
            static_cast<vk::SharingMode>(create_info.imageSharingMode),
        .queue_fam_ndcies = {},
        .layout           = vk::ImageLayout::undfnd,
    };

    for (auto&& vk_img : vk_imgs) {
        imgs.push_back(Image {vk_img, dev, ps});
    }

    for (auto&& img : imgs) {
        _img_views.push_back(ImageView {img});
    }
}

Swapchain::Swapchain(PhysDevice p_dev_in,
                     Device::ptr l_dev,
                     Surface& surf_in)
    :p_dev {p_dev_in},
     dev{l_dev},
     surf {surf_in},
     create_swch{
        reinterpret_cast<PFN_vkCreateSwapchainKHR>(
            l_dev->get_proc_addr("vkCreateSwapchainKHR")
        )
     },
     get_swch_imgs{
        reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(
            l_dev->get_proc_addr("vkGetSwapchainImagesKHR")
        )
     },
     acquire_next_img{
        reinterpret_cast<PFN_vkAcquireNextImageKHR>(
            l_dev->get_proc_addr("vkAcquireNextImageKHR")
        )
     },
     destroy_swch{
        reinterpret_cast<PFN_vkDestroySwapchainKHR>(
            l_dev->get_proc_addr("vkDestroySwapchainKHR")
        )
     }
{
    create();
}

Swapchain::~Swapchain() noexcept
{
    log.attempt("Vulkan", "destroying swapchain");
    destroy_swch(dev->inner(), swch, NULL);
    log.finish();
    log.brk();
}

void Swapchain::recreate()
{
    log.enter("Vulkan", std::string("recreating swapchain"));
    log.brk();
    create(swch);
}

SwapchainResult Swapchain::next_img(VkFence fnce,
                                    VkSemaphore sem,
                                    uint64_t timeout)
{
    log.attempt("Vulkan", "acquiring next swapchain image");
    VkResult res = acquire_next_img(dev->inner(),
                                    swch,
                                    timeout,
                                    sem,
                                    fnce,
                                    &current_ndx);

    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
        log.enter("swapchain needs recreation");
        log.brk();
        return SwapchainResult::needs_recreate;
    } else if (res == VK_TIMEOUT || res == VK_NOT_READY) {
        log.enter("not ready");
        log.brk();
        return SwapchainResult::not_ready;
    } else {
        log.finish();
        Vulkan::vk_try(res, "double-checking swapchain image acquisition");
        log.indent();
        log.enter("index", current_ndx);
        log.brk();
    }

    return SwapchainResult::okay;
}

void fence_response(Fence& fnce, SwapchainResult res)
{
    if (res == SwapchainResult::okay) {
        fnce.wait();
    } else {
        fnce.reset();
    }
}

SwapchainResult Swapchain::next(Fence& fnce,
                                BinarySemaphore& sem,
                                uint64_t timeout)
{
    auto res = next_img(fnce.inner(), sem.inner(), timeout);

    fence_response(fnce, res);
    return res;
}

SwapchainResult Swapchain::next(Fence& fnce, BinarySemaphore& sem)
{
    auto res = next_img(fnce.inner(), sem.inner(), 0);

    fence_response(fnce, res);
    return res;
}

SwapchainResult Swapchain::next(Fence& fnce, uint64_t timeout)
{
    auto res = next_img(fnce.inner(), VK_NULL_HANDLE, timeout);

    fence_response(fnce, res);
    return res;
}

SwapchainResult Swapchain::next(Fence& fnce)
{
    auto res = next_img(fnce.inner(), VK_NULL_HANDLE, 0);

    fence_response(fnce, res);
    return res;
}

SwapchainResult Swapchain::next(BinarySemaphore& sem, uint64_t timeout)
{
    return next_img(VK_NULL_HANDLE, sem.inner(), timeout);
}

SwapchainResult Swapchain::next(BinarySemaphore& sem)
{
    return next_img(VK_NULL_HANDLE, sem.inner(), 0);
}

ImageView& Swapchain::view()
{
    log.enter("Vulkan", "getting view handle to image at index "
              + std::to_string(current_ndx));
    log.brk();
    return _img_views.at(current_ndx);
}

Image& Swapchain::img()
{
    log.enter("Vulkan", "getting handle to image at index "
              + std::to_string(current_ndx));
    log.brk();
    return imgs.at(current_ndx);
}


} // namespace cu
