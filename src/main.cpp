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

#include "engine.hpp"

#include "log.hpp"

#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    bool debug = false;

    std::string help_txt = "Usage: crypt_underworld [OPTION]...\n"
                           "Play a game called Crypt Underworld.\n"
                           "\n"
                           "    -d, --debug     Enable debug output\n"
                           "    -l, --log       Enable log output\n"
                           "    -a, --async-log Log messages asynchronously\n"
                           "    -h, --help      Print this message and exit\n";

    for (int i = 1; i < argc; ++i) {
        std::string arg {argv[i]};

        if (arg == "-h" || arg == "--help") {
            std::cout << help_txt;
            return 0;
        } else if (arg == "-d" || arg == "--debug") {
            debug = true;
        } else if (arg == "-l" || arg == "--log") {
            cu::log.turn_on();
        } else if (arg == "-a" || arg == "--async-log") {
            cu::log.async_on();
        } else {
            std::cout << "*** Option " << arg << " not recognized.\n\n"
                      << help_txt;
            return EINVAL;
        }
    }

    cu::Engine e {debug};

    return 0;
}
