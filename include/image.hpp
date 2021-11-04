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

#include <vulkan/vulkan.h>

namespace cu {

class LogiDevice;

/*!
 * \brief A Vulkan image wrapper.
 */
class Image {
public:
    /*!
     * \brief (constructor) Wrap an existing VkImage, in order to
     * handle `vkGetSwapchainImagesKHR` and the like.
     *
     * \param existing An already-initialized VkImage.
     * \param l_dev    The logical device used to create the
     *                 image.
     * \param destroy  Whether the VkImage should be destroyed in
     *                 the Image's destructor (Swapchain images
     *                 are destroyed along with the Swapchain).
     */
    Image(VkImage existing, LogiDevice& l_dev, bool destroy = false);

    ~Image() noexcept;
private:
    VkImage img;
    VkDevice dev;

    PFN_vkDestroyImage destroy_img;

    bool should_destroy;
};

} // namespace cu

#endif
