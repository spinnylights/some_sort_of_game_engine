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
#include <string>
#include <cstdint>

namespace cu {

/*!
 * \brief A general-purpose representation of a binary file on disk.
 */
class BinFile {
public:
    /*!
     * \brief (constructor) Reads in the data at fpath.
     *
     * \param fpath The path to the file you'd like to work with.
     */
    BinFile(std::filesystem::path fpath);

    BinFile(const BinFile&);
    BinFile& operator=(const BinFile&);

    BinFile(BinFile&&);
    BinFile& operator=(BinFile&&);

    /*!
     * \brief Returns the filepath.
     */
    std::filesystem::path path() const { return pth; }

    /*!
     * \brief The data contained within the file.
     */
    std::string data() const { return dta; }

    /*!
     * \brief A pointer to the data as unsigned 32-bit integers.
     */
    const std::uint32_t* u32()
    {
        return reinterpret_cast<const std::uint32_t*>(dta.data());
    }

    /*!
     * \brief The size of the data in bytes.
     */
    std::size_t size() const { return dta.size(); }

public:
    std::filesystem::path pth;

public:
    std::string dta;
};

} // namespace cu

#endif
