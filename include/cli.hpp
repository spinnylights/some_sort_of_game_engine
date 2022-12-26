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
#include <filesystem>

namespace cu {

/*!
 * \brief Parses the command line arguments.
 *
 * Pass in the parameters from main() and this class will
 * determine which options the user wants enabled, what to print
 * to stdout if anything (e.g. help or error text), and an exit
 * status code to return if exiting right away instead of
 * starting the game (e.g. if the user requests help).
 */
class CLI {
public:
    /*!
     * \brief (constructor)
     *
     * Parses the command line.
     *
     * \param argc the length of argv.
     * \param argv the arguments array.
     */
    CLI(int argc, char** argv);

    /*!
     * \brief Text to write to stdout, if any.
     */
    std::string output() const { return outpt; }

    /*!
     * \brief An exit status code to return if immediately exiting
     * (e.g. if the user has requested help text).
     */
    int status() const { return stat; }

    /*!
     * \brief Whether to enable logging in general.
     */
    bool log() const { return lg; };

    /*!
     * \brief Whether to log debug messages from Vulkan.
     */
    bool debug() const { return debg; }

    /*!
     * \brief Whether logging should be done asynchronously.
     */
    bool async_log() const { return async_lg; }

    /*!
     * \brief Whether to print the help text.
     */
    bool help() const { return hlp; }

    /*!
     * \brief Whether minicomp mode is on.
     */
    bool minicomp() const;

    /*!
     * \brief The path to the compute shader, if any.
     */
    std::filesystem::path comp_path() const { return compute_shdr_path; }

private:
    std::string outpt;

private:
    std::filesystem::path compute_shdr_path;

private:
    int stat = 0;
    bool lg = false;
    bool debg = false;
    bool async_lg = false;
    bool hlp = false;
};

} // namespace cu

#endif
