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

#include <chrono>
#include <fstream>

namespace cu {

std::vector<const char*> Engine::layers()
{
    if (dbg) {
        return {"VK_LAYER_KHRONOS_validation"};
    } else {
        return {};
    }
}

std::vector<const char*> Engine::extensions()
{
    auto exts = sdl.get_req_vulk_exts();

    if (dbg) {
        exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return exts;
}

Engine::Engine(bool debug)
    :dbg(debug),
     sdl{},
     vulk{extensions(),
          layers(),
          sdl,
          debug}
{}

void Engine::mode(Mode new_mode)
{
    mde = new_mode;
    log.enter("Engine: switching to " +  mode_str() + " mode");
    log.brk();
}

void Engine::add_shader(std::string name, BinFile f)
{
    vulk.add_shader(name, f);
}

void Engine::minicomp_mode(std::filesystem::path comp_spv_path)
{
    mode(minicomp);
    std::ifstream comp_spv_f(comp_spv_path, std::ios::binary | std::ios::ate);
    BinFile::container_t::size_type comp_spv_sz;
    if (comp_spv_f) {
        comp_spv_sz = comp_spv_f.tellg();
    } else {
        throw std::runtime_error("unable to open compiled shader at "
                                 + comp_spv_path.string());
    }

    add_shader(mode_str(), {comp_spv_f, comp_spv_sz});
    vulk.minicomp_setup();

    bool quit = false;
    while (!quit) {
        auto start = std::chrono::steady_clock::now();
        sdl.poll();

        if (sdl.quit()) {
            break;
        }

        // render
        vulk.minicomp_frame();
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> dur = end - start;

        log.enter("Engine", "** END OF FRAME ** ("
                  + std::to_string(dur.count())
                  + " s.)");
        log.brk();
    }
}

} // namespace cu
