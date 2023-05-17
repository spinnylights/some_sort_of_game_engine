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
 * Copyright (c) 2023 Zoë Sparks <zoe@milky.flowers>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "sdl.hpp"
#include "instance.hpp"
#include "debug_msgr.hpp"
#include "phys_devices.hpp"
#include "phys_device.hpp"
#include "device.hpp"

static cu::SDL sdl {};

std::vector<const char*> extensions()
{
    auto exts = sdl.get_req_vulk_exts();

    exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return exts;
}

static std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};
static std::vector<const char*> exts   = extensions();

static cu::Instance::ptr inst {
    std::make_shared<cu::Instance>(exts, layers)
};

TEST_CASE("VkInstance is not null") {
    CHECK(inst->inner() != VK_NULL_HANDLE);
}

static cu::DebugMsgr dbg {inst, true};

static cu::PhysDevices phys_devs {inst};

static cu::PhysDevice phys_dev = phys_devs.default_device();

TEST_CASE("VkPhysicalDevice is not null") {
    CHECK(phys_dev.inner() != VK_NULL_HANDLE);
}

TEST_CASE("Physical device supports graphics") {
    CHECK(phys_dev.graphics());
}

static cu::Device::ptr dev {
    std::make_shared<cu::Device>(phys_dev, inst)
};

TEST_CASE("VkDevice is not null") {
    CHECK(dev->inner() != VK_NULL_HANDLE);
}
