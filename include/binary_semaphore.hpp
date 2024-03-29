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

#ifndef s0b1d78c61674773890d9ef685cf948d
#define s0b1d78c61674773890d9ef685cf948d

#include "semaphore.hpp"

namespace cu {

class BinarySemaphore : public Semaphore {
public:
    explicit BinarySemaphore(Device::ptr l_dev);

    ~BinarySemaphore() noexcept { Deviced::dstrct(); };
};

} // namespace cu

#endif
