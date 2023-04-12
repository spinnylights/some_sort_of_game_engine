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

#include "bin_data.hpp"

#include <fstream>
#include <iostream>
#include <utility>

namespace cu {

BinData::BinData(BinData::stream_t& f, container_t::size_type size)
{
    if (f) {
        auto posmemo = f.tellg();
        bool non_32 = size & 3;
        auto readsize = non_32 ? ((size >> 2) + 1) << 2 : size;
        dta.resize(readsize, '\0');
        f.seekg(0);
        f.read(dta.data(), size);
        f.seekg(posmemo);
    } else {
        throw std::runtime_error("invalid stream");
    }
}

BinData::BinData(const BinData& f)
    : dta {f.inner()}
{}

BinData& BinData::operator=(const BinData& f)
{
    dta = f.inner();

    return *this;
}

BinData::BinData(BinData&& f)
    : dta {std::move(f.dta)}
{
    f.dta = {};
}

BinData& BinData::operator=(BinData&& f)
{
    std::swap(dta, f.dta);

    return *this;
}

} // namespace cu
