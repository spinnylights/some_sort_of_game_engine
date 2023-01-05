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
 * Copyright (c) 2022 Zoë Sparks <zoe@milky.flowers>
 */

#ifndef k83658a286d44c8db607933afed9c583
#define k83658a286d44c8db607933afed9c583

#include <string>
#include <vulkan/vulkan.h>

#include "log.hpp"
#include "device.hpp"

namespace cu {

using descr_fn = std::string(void);

/*!
 * \brief A base class for Device-created Vulkan objects.
 *
 * Gets their function pointers, sets up the Device pointer, supplies a default
 * destructor.
 *
 * \param Fc The Vulkan constructor function
 * \param Fd The Vulkan destructor function
 * \param Inner The wrapped Vulkan type
 */
template<typename Fc, typename Fd, typename Inner>
class Deviced {
public:
    /*!
     * \brief (constructor)
     *
     * \param l_dev A pointer to the Device
     *
     * \param dscr  A short description of the Vulkan object, e.g. "compute
     * pipeline"
     *
     * \param create_fn_suff The suffix attached to the Vulkan constructor, e.g.
     * "Swapchain" for `vkCreateSwapchain` or "ComputePipelines" for
     * `vkCreateComputePipelines`.
     *
     * \param dest_fn_suff The suffix attached to the Vulkan destructor, e.g.
     * "Pipeline" for `vkDestroyPipeline`. You can leave it blank if the
     * dest_fn_suff should be the same as the create_fn_suff, as with
     * "Swapchain" (`vkCreateSwapchain` and `vkDestroySwapchain`).
     */
    Deviced(Device::ptr l_dev,
            std::string dscr,
            std::string create_fn_suff,
            std::string dest_fn_suff = "")
        : dev {l_dev},
          descr {dscr},
          create {
              reinterpret_cast<Fc>(
                  dev->get_proc_addr(("vkCreate" + create_fn_suff).c_str())
              )
          },
          destroy {
              reinterpret_cast<Fd>(
                  dev->get_proc_addr(("vkDestroy" +
                                      (dest_fn_suff.empty() ? create_fn_suff
                                                              : dest_fn_suff))
                                     .c_str())
              )
          }
    {}

    /*!
     * \brief A destructor you can use.
     *
     * Assumes the Vulkan destructor can be called as `destroy(dev->inner(),
     * vk_handle, NULL)`. If logging is enabled, sends a message to the log
     * about the destruction using descr.
     */
    void dstrct() noexcept
    {
        log.attempt("Vulkan", "destroying " + descr);
        destroy(dev->inner(), nner, NULL);
        log.finish();
        log.brk();
    }

protected:
    Device::ptr dev;

private:
    std::string descr;

protected:
    Fc create;
    Fd destroy;

protected:
    Inner nner;
};

} // namespace cu

#endif
