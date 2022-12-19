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

namespace cu {

CLI::CLI(int argc, char** argv)
{
    std::string help_txt = "Usage: " + std::string{argv[0]} + " [OPTION]...\n"
                           "Play a game called " + Game::name + ".\n"
                           "\n"
                           "    -l, --log       Enable log output\n"
                           "    -d, --debug     Enable Vulkan debug output\n"
                           "                    (silent without --log)\n"
                           "    -a, --async-log Log messages asynchronously\n"
                           "    -h, --help      Print this message and exit\n";

    constexpr struct option long_options[] = {
        {"log",       no_argument, NULL, 'l'},
        {"debug",     no_argument, NULL, 'd'},
        {"async-log", no_argument, NULL, 'a'},
        {"help",      no_argument, NULL, 'h'},
        {0, 0, 0, 0},
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "ldah", long_options, nullptr)) != -1) {
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
        default:
            outpt = "\n***\n\n" + help_txt;
            hlp = true;
            stat = EINVAL;
        }
    }
}

} // namespace cu
