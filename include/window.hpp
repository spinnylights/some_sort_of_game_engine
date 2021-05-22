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

#ifndef p331cb00feda44159c4965f40d76fdfa
#define p331cb00feda44159c4965f40d76fdfa

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

namespace cu {

class Window {
public:
    Window();

    Window(Window&&) = delete;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    ~Window();

    SDL_Window* inner() { return win; }

private:
    SDL_Window* win;
};

} // namespace cu

#endif
