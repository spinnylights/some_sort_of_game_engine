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

#ifndef o38ceb660d354959bcf20e39afdb11e9
#define o38ceb660d354959bcf20e39afdb11e9

#include "deviced.hpp"

namespace cu {

class Semaphore : public Deviced<PFN_vkCreateSemaphore,
                                 PFN_vkDestroySemaphore,
                                 VkSemaphore> {
public:
    /*!
     * /brief Binary semaphore constructor.
     */
    explicit Semaphore(Device::ptr l_dev);

    /*!
     * /brief Timeline semaphore constructor.
     */
    Semaphore(Device::ptr l_dev, uint64_t initial_val);

private:
    void cnstrct(const VkSemaphoreCreateInfo*);
};

} // namespace cu

#endif
