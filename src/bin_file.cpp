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

#include "bin_file.hpp"

#include <fstream>
#include <iostream>
#include <utility>

namespace fs = std::filesystem;

namespace cu {

BinFile::BinFile(fs::path fpath)
    : pth {fpath}
{
    if (std::ifstream is {fpath, std::ios::binary | std::ios::ate}) {
        auto bytesize = is.tellg();
        dta.resize(bytesize, '\0');
        is.seekg(0);
        is.read(reinterpret_cast<char*>(dta.data()), bytesize);
        is.close();
    } else {
        throw std::runtime_error("failed to open file at "
                                 + fpath.string());
    }
}

BinFile::BinFile(const BinFile& f)
    : pth {f.path()},
      dta {f.data()}
{}

BinFile& BinFile::operator=(const BinFile& f)
{
    pth = f.path();
    dta = f.data();

    return *this;
}

BinFile::BinFile(BinFile&& f)
    : pth {std::move(f.pth)},
      dta {std::move(f.dta)}
{
    f.pth = "";
    f.dta = "";
}

BinFile& BinFile::operator=(BinFile&& f)
{
    std::swap(pth, f.pth);
    std::swap(dta, f.dta);

    return *this;
}

} // namespace cu
