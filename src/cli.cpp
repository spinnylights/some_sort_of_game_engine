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

namespace cu {

bool grouped_single_char_args(std::string arg)
{
    return arg.length() > 2 && arg.at(1) != '-';
}

std::vector<std::string> parse_argv(int argc, char** argv)
{
    std::vector<std::string> args;

    for (int i = 1; i < argc; ++i) {
        std::string arg {argv[i]};

        if (grouped_single_char_args(arg)) {
            for (auto c : arg) {
                if (c != '-') {
                    args.push_back("-" + std::string{c});
                }
            }
        } else {
            args.push_back(arg);
        }
    }

    return args;
}

CLI::CLI(int argc, char** argv)
{
    auto args = parse_argv(argc, argv);

    std::string help_txt = "Usage: " + std::string{argv[0]} + " [OPTION]...\n"
                           "Play a game called " + Game::name + ".\n"
                           "\n"
                           "    -l, --log       Enable log output\n"
                           "    -d, --debug     Enable Vulkan debug output\n"
                           "                    (silent without --log)\n"
                           "    -a, --async-log Log messages asynchronously\n"
                           "    -h, --help      Print this message and exit\n";

    for (const auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            outpt = help_txt;
            hlp = true;
        } else if (arg == "-d" || arg == "--debug") {
            debg = true;
        } else if (arg == "-l" || arg == "--log") {
            lg = true;
        } else if (arg == "-a" || arg == "--async-log") {
            async_lg = true;
        } else {
            outpt = "*** Option " + arg + " not recognized.\n\n" + help_txt;
            hlp = true;
            stat = EINVAL;
        }
    }
}

} // namespace cu
