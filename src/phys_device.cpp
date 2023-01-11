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

#include "phys_device.hpp"
#include "surface.hpp"

#include "log.hpp"

namespace cu {

VkDeviceSize calc_total_mem(const VkPhysicalDeviceMemoryProperties& mem_props)
{
    VkDeviceSize total_mem = 0;
    for (std::size_t i = 0;
         i < mem_props.memoryHeapCount;
         ++i) {
        VkMemoryHeap heap = mem_props.memoryHeaps[i];
        if ((heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
            == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            total_mem += heap.size;
        }
    }

    return total_mem;
}

void PhysDevice::get_queue_fams(std::vector<VkQueueFamilyProperties>&
                                   queue_fam_props,
                               Surface& surf,
                               Instance::ptr inst)
{
        for (uint32_t i = 0; i < queue_fam_props.size(); ++i) {
            QueueFamily fam = {
                queue_fam_props.at(i),
                i,
                inner(),
                surf,
                inst
            };

            queue_families.push_back(fam);
        }

}

void PhysDevice::populate_mem_props(const VkPhysicalDeviceMemoryProperties&
                                        props)
{
    for (std::size_t i = 0; i < props.memoryTypeCount; ++i) {
        mem_types.push_back({
            .inner = props.memoryTypes[i],
            ._ndx  = i,
        });
    }

    for (std::size_t i = 0; i < props.memoryHeapCount; ++i) {
        mem_heaps.push_back({
            .inner = props.memoryHeaps[i],
            ._ndx  = i,
        });

        for (const auto& type : mem_types) {
            if (type.heap_ndx() == i) {
                mem_heaps.back().mem_types.push_back(type);
            }
        }
    }
}

PhysDevice::PhysDevice(VkPhysicalDevice                     device,
                       Surface&                             surf,
                       Instance::ptr                        inst,
                       PhysDeviceProps                      device_props,
                       VkPhysicalDeviceMemoryProperties     vk_memory_props,
                       std::vector<VkQueueFamilyProperties> vk_queue_props,
                       std::vector<std::string>
                           extensions_supported)
    : dev                    {device},
      name                   {device_props.props.properties.deviceName},
      type                   {device_props.props.properties.deviceType},
      raw_vk_ver             {device_props.props.properties.apiVersion},
      raw_vk_driver_ver      {device_props.props.properties.driverVersion},
      vk_vend_id             {device_props.props.properties.vendorID},
      vk_vend_dev_id         {device_props.props.properties.deviceID},
      max_timel_sem_val_diff {
          device_props.timel_props.maxTimelineSemaphoreValueDifference
      },
      mem                    {calc_total_mem(vk_memory_props)},
      extensions             {extensions_supported},
      get_phys_dev_ftrs {
          reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures2>(
              inst->get_proc_addr("vkGetPhysicalDeviceFeatures2")
          )
      }
{
    get_queue_fams(vk_queue_props, surf, inst);
    populate_mem_props(vk_memory_props);
    maintenance4.pNext = &timel_sem;
    features.pNext = &maintenance4;
    get_phys_dev_ftrs(dev, &features);

    if (!timel_sem.timelineSemaphore) {
        throw std::runtime_error("gpu does not support timeline semaphores");
    }

    if (!maintenance4.maintenance4) {
        throw std::runtime_error("gpu does not support maintenance4");
    }
}

void PhysDevice::fix_pnext_chain()
{
    maintenance4.pNext = &timel_sem;
    features.pNext     = &maintenance4;
}

PhysDevice::PhysDevice(const PhysDevice& other)
    : dev                    {other.dev},
      name                   {other.name},
      type                   {other.type},
      raw_vk_ver             {other.raw_vk_ver},
      raw_vk_driver_ver      {other.raw_vk_driver_ver},
      vk_vend_id             {other.vk_vend_id},
      vk_vend_dev_id         {other.vk_vend_id},
      max_timel_sem_val_diff {other.max_timel_sem_val_diff},
      mem                    {other.mem},
      extensions             {other.extensions},
      get_phys_dev_ftrs      {other.get_phys_dev_ftrs},
      timel_sem              {other.timel_sem},
      maintenance4           {other.maintenance4},
      features               {other.features}
{
    fix_pnext_chain();
}

PhysDevice& PhysDevice::operator=(const PhysDevice& other)
{
    PhysDevice tmp {other};
    std::swap(*this, tmp);
    return *this;
}

PhysDevice::PhysDevice(PhysDevice&& other)
    : dev                    {other.dev},
      name                   {other.name},
      type                   {other.type},
      raw_vk_ver             {other.raw_vk_ver},
      raw_vk_driver_ver      {other.raw_vk_driver_ver},
      vk_vend_id             {other.vk_vend_id},
      vk_vend_dev_id         {other.vk_vend_id},
      max_timel_sem_val_diff {other.max_timel_sem_val_diff},
      mem                    {other.mem},
      extensions             {other.extensions},
      get_phys_dev_ftrs      {other.get_phys_dev_ftrs},
      timel_sem              {other.timel_sem},
      maintenance4           {other.maintenance4},
      features               {other.features}
{
    fix_pnext_chain();

    other.dev = nullptr;
    other.name = "";
    other.mem_types = {};
    other.mem_heaps = {};
    other.extensions = {};
    other.queue_families = {};
    other.get_phys_dev_ftrs = nullptr;
    other.timel_sem = {};
    other.maintenance4 = {};
    other.features = {};
}

PhysDevice& PhysDevice::operator=(PhysDevice&& other)
{
    std::swap(*this, other);
    fix_pnext_chain();
    return *this;
}

PhysDevice::VkVersionNumber PhysDevice::vk_ver_spprtd() const
{
        VkVersionNumber ver_n = {
                .major = VK_VERSION_MAJOR(raw_vk_ver),
                .minor = VK_VERSION_MINOR(raw_vk_ver),
                .patch = VK_VERSION_PATCH(raw_vk_ver),
        };

        return ver_n;
}

std::string PhysDevice::VkVersionNumber::str() const
{
    return std::to_string(major) + "." +
           std::to_string(minor) + "." +
           std::to_string(patch);
}

std::string PhysDevice::mem_str() const
{
    return std::to_string(mem) + " bytes";
}


void PhysDevice::log() const
{
    cu::log.enter({
        .name = name,
        .members {
            {"api version",
                vk_ver_spprtd().str()},
            {"driver version",
                raw_vk_driver_ver},
            {"vendor ID", vk_vend_id},
            {"device ID", vk_vend_dev_id},
            {"device type", type},
            {"video memory", mem_str()},
            {"max timel. sem. val. diff.", max_timel_sem_val_diff},
            {"extensions", extensions}
        }
    });

    cu::log.brk();

    for (const auto& fam : queue_families) {
        cu::log.indent(2);
        fam.log_info();
    }

    for (const auto& heap : mem_heaps) {
        cu::log.indent(2);
        cu::log.enter({
            .name = "heap " + std::to_string(heap.ndx()),
            .members = {
                {"size", std::to_string(heap.size()) + " bytes"},
                {"device local", heap.device_local()},
            }
        });
        cu::log.brk();

        for (const auto& type : mem_types) {
            if (type.heap_ndx() == heap.ndx()) {
                cu::log.indent(3);
                cu::log.enter({
                    .name = "type " + std::to_string(type.ndx()),
                    .members = {
                        {"heap index",    type.heap_ndx()},
                        {"device local",  type.device_local()},
                        {"host visible",  type.host_visible()},
                        {"host coherent", type.host_coherent()},
                        {"host cached",   type.host_cached()},
                    }
                });
                cu::log.brk();
            }
        }
    }
}

} // namespace cu
