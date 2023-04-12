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

#ifndef Y3a755e5d163414fbb1bb2aa3df497d7
#define Y3a755e5d163414fbb1bb2aa3df497d7

#include <filesystem>
#include <vector>
#include <cstdint>

namespace cu {

/*!
 * \brief A general-purpose representation of binary data read in from a stream,
 * motivated by Vulkan's interface for consuming shaders.
 */
class BinData {
public:
    using data_t = char;
    using stream_t = std::basic_istream<data_t>;
    using container_t = std::vector<data_t>;

    /*!
     * \brief (constructor) Reads in data from istream and copies it into an
     * internal buffer. If the data does not fit cleanly into 32-bit words, the
     * internal copy is padded out with nulls for safety (this should never be
     * the case if the data is SPIR-V).
     *
     * \param istream The stream to the data you'd like to work with.
     * \param size The amount of data to read in from the stream in bytes.
     */
    BinData(stream_t& istream, container_t::size_type size);

    BinData(const BinData&);
    BinData& operator=(const BinData&);

    BinData(BinData&&);
    BinData& operator=(BinData&&);

    /*!
     * \brief The inner data.
     */
    container_t inner() const { return dta; }

    /*!
     * \brief A pointer to the data.
     */
    const data_t* data() const { return dta.data(); }

    /*!
     * \brief A 32-bit pointer to the data. Vulkan requires this.
     */
    const std::uint32_t* u32() const
    {
        return reinterpret_cast<const std::uint32_t*>(dta.data());
    }

    /*!
     * \brief The size of the data in bytes.
     */
    container_t::size_type size() const { return dta.size(); }

public:
    container_t dta;
};

} // namespace cu

#endif
