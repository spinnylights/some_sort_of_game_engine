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

#ifndef T1ea57521dc545848febc06aae81ba12
#define T1ea57521dc545848febc06aae81ba12

#include "sdl.hpp"
#include "vulkan.hpp"

namespace cu {

/*!
 * \brief The entry point into the game engine; coordinates its
 * components. The tentative long-term plan is for this to take a
 * reference to a Game once a frame and handle IO based on it.
 */
class Engine {
public:
    enum class Mode { normal, minicomp };
    using enum Mode;

    /*!
     * \brief (constructor)
     *
     * \param debug Whether the Vulkan debug utils should be
     * enabled.
     */
    Engine(bool debug = false);

    Engine(Engine&&) = delete;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    ~Engine() = default;

    /*!
     * \brief Returns the Vulkan layers required.
     */
    std::vector<const char*> layers();

    /*!
     * \brief Returns the Vulkan extensions required.
     */
    std::vector<const char*> extensions();

    /*!
     * \brief Returns the current mode.
     */
    Mode mode() const { return mde; }

    /*!
     * \brief Returns the current mode as a string.
     */
    constexpr std::string mode_str()
    {
        switch(mde) {
        case normal:
            return "normal";
        case minicomp:
            return "minicomp";
        default:
            return "UNKNOWN";
        }
    }

    /*!
     * \brief Sets the current mode.
     */
    void mode(Mode new_mode);

    /*!
     * \brief Add a new compiled (SPIR-V) shader.
     */
    void add_shader(std::string name, BinFile compiled_shader);

private:
    bool dbg;

private:
    SDL sdl;

private:
    Vulkan vulk;

private:
    Mode mde = normal;
};

} // namespace cu

#endif
