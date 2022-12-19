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

#include "cli.hpp"

#include "game.hpp"

#include <getopt.h>
#include <filesystem>
#include <iostream>

namespace cu {

// Obviously this could be DRYer, but it's so small right now that I don't feel
// like it's worth working over yet. Once it really starts to get on my nerves
// I'll make an Options class or the like.

CLI::CLI(int argc, char** argv)
{
    std::string help_txt =
        "Usage: \n"
        "    " + std::string{argv[0]} + " [OPTION...]\n"
        "    " + std::string{argv[0]} + " [OPTION...] -m COMPUTE_SHADER\n"
        "\n"
        "Play a game called " + Game::name + ".\n"
        "\n"
        "    -l, --log                         Enable log output\n"
        "    -d, --debug                       Enable Vulkan debug output\n"
        "                                      (silent without --log)\n"
        "    -a, --async-log                   Log messages asynchronously\n"
        "    -m, --minicomp=COMPUTE_SHADER     Run COMPUTE_SHADER in minicomp mode\n"
        "    -h, --help                        Print this message and exit\n";

    constexpr struct option long_options[] = {
        {"log",       no_argument,       NULL, 'l'},
        {"debug",     no_argument,       NULL, 'd'},
        {"async-log", no_argument,       NULL, 'a'},
        {"minicomp",  required_argument, NULL, 'm'},
        {"help",      no_argument,       NULL, 'h'},
        {0, 0, 0, 0},
    };

    std::filesystem::path compute_shdr_path;

    int opt;
    while ((opt = getopt_long(argc, argv, "ldam:h", long_options, nullptr))
            != -1) {
        switch(opt) {
        case 'h':
            outpt = help_txt;
            hlp = true;
            break;
        case 'd':
            debg = true;
            break;
        case 'l':
            lg = true;
            break;
        case 'a':
            async_lg = true;
            break;
        case 'm':
            compute_shdr_path = {std::string(optarg)};
            std::cout << compute_shdr_path;
            break;
        default:
            outpt = "\n***\n\n" + help_txt;
            hlp = true;
            stat = EINVAL;
        }
    }
}

} // namespace cu
