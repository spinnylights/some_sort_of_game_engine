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
#include "cli.hpp"

#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    cu::CLI cli {argc, argv};

    if (cli.help()) {
        std::cout << cli.output();
        return cli.status();
    }

    if (cli.log()) {
        cu::log.turn_on();
    }

    if (cli.async_log()) {
        cu::log.async_on();
    }

    cu::Engine e {cli.debug()};

    if (cli.minicomp()) {
        e.minicomp_mode(cli.comp_path());
    }

    return 0;
}
