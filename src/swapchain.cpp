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
#include "logi_device.hpp"
#include "surface.hpp"
#include "vulkan.hpp"
#include "sdl.hpp"

#include <vector>
#include <algorithm>

namespace cu {

Swapchain::Swapchain(PhysDevice& p_dev,
                     LogiDevice& l_dev,
                     Surface& surf,
                     SDL& sdl)
    :create_swch{
        reinterpret_cast<PFN_vkCreateSwapchainKHR>(
            l_dev.get_proc_addr("vkCreateSwapchainKHR")
        )
     },
     get_swch_imgs{
        reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(
            l_dev.get_proc_addr("vkGetSwapchainImagesKHR")
        )
     },
     destroy_swch{
        reinterpret_cast<PFN_vkDestroySwapchainKHR>(
            l_dev.get_proc_addr("vkDestroySwapchainKHR")
        )
     }
{
    const auto win_size = sdl.get_win_size();

    const auto surface_caps = surf.capabilities(p_dev);
    uint32_t min_img_cnt = 3;
    if (surface_caps.maxImageCount < 3) {
        min_img_cnt = surface_caps.maxImageCount;
    }

    // support for VK_PRESENT_MODE_FIFO_KHR is required by the
    // spec (see VkSwapchainCreateInfoKHR in 33.9 WSI Swapchain)
    VkPresentModeKHR pres_mode = VK_PRESENT_MODE_FIFO_KHR;
    const auto pres_modes = surf.present_modes(p_dev);
    if (std::find(begin(pres_modes), end(pres_modes),
                  VK_PRESENT_MODE_MAILBOX_KHR) != end(pres_modes)) {
        pres_mode = VK_PRESENT_MODE_MAILBOX_KHR;
    }
    log.enter("present mode used", pres_mode);
    log.brk();

    VkSwapchainCreateInfoKHR create_info {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = surf.inner(),
        .minImageCount = min_img_cnt,

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
        .imageFormat = VK_FORMAT_B8G8R8A8_SRGB,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,

        .imageExtent = { .width = static_cast<uint32_t>(win_size.width),
                         .height = static_cast<uint32_t>(win_size.height) },
        .imageArrayLayers = 1,

        // this will probably need to be changed later
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
        .preTransform = surface_caps.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = pres_mode,
        .clipped = VK_TRUE,

        // TODO: remember to set this properly during swap chain
        // recreation
        .oldSwapchain = old_swch,
    };

    Vulkan::vk_try(create_swch(l_dev.inner(), &create_info, NULL, &swch),
                   "creating swapchain");
    log.brk();

    uint32_t imgs_cnt;
    Vulkan::vk_try(get_swch_imgs(l_dev.inner(), swch, &imgs_cnt, NULL),
                   "getting swapchain images count");
    log.indent();
    log.enter("images count", imgs_cnt);
    log.brk();

    imgs.resize(imgs_cnt);
    Vulkan::vk_try(get_swch_imgs(l_dev.inner(), swch, &imgs_cnt, imgs.data()),
                   "getting swapchain images");
    log.brk();
}

Swapchain::~Swapchain() noexcept
{
    log.attempt("Vulkan", "destroying swapchain");
    destroy_swch(dev, swch, NULL);
    log.finish();
    log.brk();
}
} // namespace cu
