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
 * Copyright (c) 2023 Zoë Sparks <zoe@milky.flowers>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <bin_file.hpp>

#include <string>
#include <sstream>

void test_binfile_against_sstream(cu::BinFile bf, std::string bin_data)
{
    SUBCASE("size matches") {
        CHECK(bf.size() == bin_data.size());
    }

    SUBCASE("data matches through data pointer") {
        for (std::size_t i = 0; i < bin_data.size(); ++i) {
            CHECK(bf.data()[i] == bin_data.data()[i]);
        }
    }
}

void test_binfile_against_binfile(cu::BinFile bf, cu::BinFile other)
{
    SUBCASE("size matches") {
        CHECK(bf.size() == other.size());
    }

    SUBCASE("container matches") {
        CHECK(bf.inner() == other.inner());
    }

    SUBCASE("data matches through data pointer") {
        for (std::size_t i = 0; i < bf.size(); ++i) {
            CHECK(bf.data()[i] == other.data()[i]);
        }
    }

    SUBCASE("data matches through u32 pointer") {
        for (std::size_t i = 0;
             i < bf.size() / (sizeof(uint32_t) / sizeof(cu::BinFile::data_t));
             ++i) {
            CHECK(bf.u32()[i] == other.u32()[i]);
        }
    }
}

TEST_CASE("BinFile") {
    std::string bin_data = { 0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf };
    std::istringstream raw(bin_data);
    cu::BinFile bf(raw, bin_data.size());

    SUBCASE("construction from an istream") {
        test_binfile_against_sstream(bf, bin_data);
    }

    SUBCASE("copy construction") {
        cu::BinFile bf_cp(bf);

        test_binfile_against_binfile(bf_cp, bf);
    }

    SUBCASE("move construction") {
        cu::BinFile bf_mv({raw, bin_data.size()});

        test_binfile_against_sstream(bf_mv, bin_data);
    }

    SUBCASE("pads out non-32-bit-word data") {
        std::string non_32_data = { 0xf, 0xa, 0xc, 0xa, 0xd, 0xe };
        std::istringstream raw_non32(non_32_data);
        cu::BinFile fix32_bf(raw_non32, non_32_data.size());
        constexpr std::size_t size_diff =
            sizeof(uint32_t) / sizeof(cu::BinFile::data_t);
        CHECK(fix32_bf.size()
              == ((non_32_data.size() / size_diff) + 1) * size_diff);
    }
}
