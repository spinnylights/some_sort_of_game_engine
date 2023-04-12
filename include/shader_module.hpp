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
 * Copyright (c) 2022 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef yf4146aba35141379744cc68f4913e54
#define yf4146aba35141379744cc68f4913e54

#include <vulkan/vulkan.hpp>

#include "bin_data.hpp"
#include "deviced.hpp"

namespace cu {

/*!
 * \brief A VkShaderModule wrapper.
 */
class ShaderModule : public Deviced<PFN_vkCreateShaderModule,
                                    PFN_vkDestroyShaderModule,
                                    VkShaderModule> {
public:
    using ptr = std::shared_ptr<ShaderModule>;

    /*!
     * \brief (constructor)
     *
     * \param l_dev The current Device.
     * \param name  A name to identify the shader by.
     * \param file  The compiled (SPIR-V) shader in file form.
     */
    ShaderModule(Device::ptr l_dev, std::string name, const BinData& file);

    /*!
     * \brief (constructor)
     *
     * \param l_dev The current Device.
     * \param name  A name to identify the shader by.
     * \param bin   A pointer to the compiled (SPIR-V) shader data.
     * \param sz    The size of the data.
     */
    ShaderModule(Device::ptr l_dev,
                 std::string name,
                 const std::uint32_t* bin,
                 std::size_t sz);

    ShaderModule(const ShaderModule&) = delete;
    ShaderModule& operator=(const ShaderModule&) = delete;

    ShaderModule(ShaderModule&&);
    ShaderModule& operator=(ShaderModule&&);

    ~ShaderModule() noexcept;

    constexpr std::string name() const { return nme; }

public:
    bool free_inner = false;

private:
    std::string nme;
};

} // namespace cu

#endif
