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

#ifndef oe57235954a14256abd94ca26648c94e
#define oe57235954a14256abd94ca26648c94e

#include <vector>
#include <string>

namespace cu {

class CLI {
public:
    CLI(int argc, char** argv);

    std::string output() const { return outpt; }
    int status() const { return stat; }
    bool log() const { return lg; };
    bool debug() const { return debg; }
    bool async_log() const { return async_lg; }
    bool help() const { return hlp; }

private:
    std::string outpt;
    int stat = 0;
    bool lg = false;
    bool debg = false;
    bool async_lg = false;
    bool hlp = false;
};

} // namespace cu

#endif
