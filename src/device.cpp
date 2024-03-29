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

#include "device.hpp"

#include "log.hpp"

#include "vulkan_util.hpp"
#include "vulkan.hpp"
#include "command_buffer.hpp"
#include "fence.hpp"

#include <iostream>
#include <functional>
#include <algorithm>
#include <set>

namespace cu {

uint32_t pick_queue_ndx(PhysDevice& phys_dev,
                        std::function<bool(QueueFamily&)> criterion)
{
    uint32_t ndx = 0;
    uint32_t flag_cnt = UINT32_MAX;

    for (auto& fam : phys_dev.queue_families) {
        uint32_t this_flag_cnt = fam.flag_count();

        if (criterion(fam) && this_flag_cnt < flag_cnt) {
            ndx = fam.index();
            flag_cnt = this_flag_cnt;
        }
    }

    return ndx;
}

#define GET_QUEUE_DAT(name) \
    {\
        name##_queue,\
        {\
            pick_queue_ndx(phys_dev, [](QueueFamily& q) { return q.name(); }),\
            {}\
        }\
    }

Device::Device(PhysDevice physi_dev, Instance::ptr inst)
    : phys_dev {physi_dev},
      GET_VK_FN_PTR_INST(create_dev, CreateDevice),
      GET_VK_FN_PTR_INST(get_dev_proc_addr, GetDeviceProcAddr),
      queue_map {
        GET_QUEUE_DAT(graphics),
        GET_QUEUE_DAT(compute),
        GET_QUEUE_DAT(present),
        GET_QUEUE_DAT(transfer),
      }
{
    std::set<uint32_t> indices_to_create;
    for (const auto& [_, t] : queue_map) {
        indices_to_create.insert(std::get<uint32_t>(t));
    }

    std::vector<VkDeviceQueueCreateInfo> queue_infos;
    const float queue_priority = 1.0;
    for (auto& i : indices_to_create) {
        queue_infos.push_back({
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .queueFamilyIndex = i,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
        });
    }

    const char* const ext_names[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    for (auto&& n : ext_names) {
        std::string ext {n};
        if (std::find(begin(phys_dev.extensions),
                      end(phys_dev.extensions),
                      ext) == std::end(phys_dev.extensions)) {
            throw std::runtime_error("device extension "
                                     + ext
                                     + " is not available!");
        }
    }

    VkPhysicalDeviceFeatures2 dev_ftrs = phys_dev.features;
    VkPhysicalDeviceMaintenance4Features maint4_ftrs = phys_dev.maintenance4;
    VkPhysicalDeviceTimelineSemaphoreFeatures timel_sem_ftrs
        = phys_dev.timel_sem;
    maint4_ftrs.pNext = &timel_sem_ftrs;
    dev_ftrs.pNext = &maint4_ftrs;

    VkDeviceCreateInfo dev_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &dev_ftrs,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
        .pQueueCreateInfos = queue_infos.data(),
        .enabledLayerCount = 0, // deprecated + ignored
        .ppEnabledLayerNames = NULL, // deprecated + ignored
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = ext_names,
        .pEnabledFeatures = NULL,
    };

    Vulkan::vk_try(create_dev(phys_dev.inner(), &dev_info, NULL, &dev),
                   "create logical device");

#define GET_VK_FN_PTR_INNER(member_name,vk_fn)\
    member_name = {\
        reinterpret_cast<PFN_vk##vk_fn>(\
            get_proc_addr(STRINGIFY(vk##vk_fn))\
        )\
    }

    GET_VK_FN_PTR_INNER(get_dev_queue, GetDeviceQueue);
    GET_VK_FN_PTR_INNER(queue_submit, QueueSubmit);
    GET_VK_FN_PTR_INNER(destroy_dev, DestroyDevice);
    GET_VK_FN_PTR_INNER(queue_present, QueuePresentKHR);

    for (const auto& [_, t] : queue_map) {
        get_dev_queue(dev,
                      std::get<uint32_t>(t),
                      0,
                      const_cast<VkQueue*>(&std::get<VkQueue>(t)));
    }

    log.indent();
    for (const auto& [flavor, t] : queue_map) {
        log.enter(qflav_str(flavor) + " queue index", std::get<uint32_t>(t));
    }
    log.brk();

    heap.construct(*this, phys_dev);
}

Device::~Device() noexcept
{
    log.attempt("Vulkan", "destroying logical device");
    heap.free_self(*this);
    destroy_dev(dev, NULL);
    log.finish();
    log.brk();
}

PFN_vkVoidFunction Device::get_proc_addr(const char* name)
{
    auto fn = get_dev_proc_addr(dev, name);

    if (fn == NULL) {
        throw std::runtime_error("device function "
                                 + std::string{name}
                                 + " is not available!");
    }

    return fn;
}

uint32_t Device::queue_ndx(QueueFlavor f) const
{
    auto ndx = std::get<uint32_t>(queue_map.at(f));
    log.enter("Vulkan",
              "queue " + qflav_str(f) + " is at " + std::to_string(ndx));
    log.brk();
    return ndx;
}

VkQueue Device::queue(QueueFlavor f)
{
    log.enter("Vulkan", "getting handle of " + qflav_str(f) + " queue");
    log.brk();
    return std::get<VkQueue>(queue_map.at(f));
}

void Device::submit(QueueFlavor f, CommandBuffer& buff, Fence& fnce)
{
    VkSubmitInfo inf {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = NULL,
        .pWaitDstStageMask = NULL,
        .commandBufferCount = 1,
        .pCommandBuffers = buff.inner(),
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = NULL,
    };

    Vulkan::vk_try(queue_submit(queue(f), 1, &inf, fnce.inner()),
                   "submitting to " + qflav_str(f) + " queue");
    log.brk();

    fnce.wait();
}

bool Device::present(Swapchain& swch)
{
    // TODO: check for need to transfer image to present queue

    VkPresentInfoKHR inf {
        .sType          = v(vk::StructureType::prsnt_info),
        .swapchainCount = 1,
        .pSwapchains    = swch.inner(),
        .pImageIndices  = swch.ndx(),
    };

    VkResult res = queue_present(queue(present_queue), &inf);
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
        log.enter("Vulkan", std::string("swapchain needs recreating"));
        log.brk();
        return false;
    } else {
        Vulkan::vk_try(res, "presenting swapchain image");
        log.brk();
    }

    return true;
}

Heap::handle_t Device::alloc(Image& img)
{
    return heap.alloc_on_dev(*this, img);
}

void Device::release(Heap::handle_t h)
{
    heap.release(h);
}

} // namespace cu
