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

#ifndef p2d6304ab76845cf93d63f5a13f7a524
#define p2d6304ab76845cf93d63f5a13f7a524

#include <vulkan/vulkan.h>

namespace cu {

/*!
 * \brief A convenience wrapper for Vulkan image format values.
 */
class ImageFormat {
public:
    /*!
     * \brief (constructor) Wrap a Vulkan format value.
    */
    ImageFormat(VkFormat);

    /*!
     * \brief The underlying Vulkan format.
     */
    VkFormat format() const { return _fmt; }

    /*!
     * \brief Whether the format has a depth component.
     */
    bool has_depth() const;

    /*!
     * \brief Whether the format has a stencil component.
     */
    bool has_stencil() const;

private:
    VkFormat _fmt;
};

} // namespace cu

#endif
