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

#ifndef f80db72cb4a245559828275b30e652b5
#define f80db72cb4a245559828275b30e652b5

#include <cstdint>
#include <stdexcept>

namespace byte_conv {
    constexpr unsigned long long byte_conv(unsigned long long ibytes,
                                           unsigned int       deg,
                                           const char*        suffix)
    {
        unsigned long coef = 1;
        for (unsigned int d = 0; d < deg; ++d) {
            coef *= 1024;
        }

        if (ibytes > UINT64_MAX / coef) {
            throw std::length_error("too many " + std::string(suffix));
        }

        return coef * ibytes;
    }
}

constexpr auto operator""_KiB(unsigned long long kib)
    -> unsigned long long
{
    return byte_conv::byte_conv(kib, 1, "KiB");
}

constexpr auto operator""_MiB(unsigned long long mib)
    -> unsigned long long
{
    return byte_conv::byte_conv(mib, 2, "MiB");
}

constexpr auto operator""_GiB(unsigned long long gib)
    -> unsigned long long
{
    return byte_conv::byte_conv(gib, 3, "GiB");
}

constexpr auto operator""_TiB(unsigned long long tib)
    -> unsigned long long
{
    return byte_conv::byte_conv(tib, 4, "TiB");
}

constexpr auto operator""_PiB(unsigned long long pib)
    -> unsigned long long
{
    return byte_conv::byte_conv(pib, 5, "PiB");
}

#endif
