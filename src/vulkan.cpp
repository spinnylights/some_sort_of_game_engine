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

#include "vulkan.hpp"

#include "log.hpp"

#include "sdl.hpp"
#include "descriptor_set_layout_binding.hpp"
#include "descriptor_set_layout.hpp"
#include "pipeline_layout.hpp"
#include "compute_pipeline.hpp"
#include "descriptor_pool.hpp"
#include "command_pool.hpp"
#include "command_buffer.hpp"
#include "fence.hpp"
#include "heap.hpp"

#include <stdexcept>
#include <array>
#include <iostream>

namespace cu {

std::string map_vk_result(VkResult res)
{
    std::string msg;
    switch (res) {
        case VK_SUCCESS:
            msg = "success";
            break;
        case VK_NOT_READY:
            msg = "not ready";
            break;
        case VK_TIMEOUT:
            msg = "timeout";
            break;
        case VK_EVENT_SET:
            msg = "event set";
            break;
        case VK_EVENT_RESET:
            msg = "event reset";
            break;
        case VK_INCOMPLETE:
            msg = "incomplete";
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            msg = "error! out of host memory";
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            msg = "error! out of device memory";
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            msg = "error! initialization failed";
            break;
        case VK_ERROR_DEVICE_LOST:
            msg = "error! device lost";
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            msg = "error! memory map failed";
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            msg = "error! layer not present";
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            msg = "error! extension not present";
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            msg = "error! feature not present";
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            msg = "error! incompatible driver";
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            msg = "error! too many objects";
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            msg = "error! format not supported";
            break;
        case VK_ERROR_FRAGMENTED_POOL:
            msg = "error! fragmented pool";
            break;
        case VK_ERROR_UNKNOWN:
            msg = "error! unknown";
            break;
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            msg = "error! out of pool memory";
            break;
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            msg = "error! invalid external handle";
            break;
        case VK_ERROR_FRAGMENTATION:
            msg = "error! fragmentation";
            break;
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            msg = "error! invalid opaque capture address";
            break;
        case VK_ERROR_SURFACE_LOST_KHR:
            msg = "error! surface lost";
            break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            msg = "error! native window in use";
            break;
        case VK_SUBOPTIMAL_KHR:
            msg = "suboptimal";
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            msg = "error! out of date";
            break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            msg = "error! incompatible display";
            break;
        case VK_ERROR_VALIDATION_FAILED_EXT:
            msg = "error! validation failed";
            break;
        case VK_ERROR_INVALID_SHADER_NV:
            msg = "error! invalid shader nv";
            break;
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            msg = "error! invalid drm format modifier plane layout";
            break;
        case VK_ERROR_NOT_PERMITTED_EXT:
            msg = "error! not permitted";
            break;
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            msg = "error! full screen exclusive mode lost";
            break;
        case VK_THREAD_IDLE_KHR:
            msg = "thread idle";
            break;
        case VK_THREAD_DONE_KHR:
            msg = "thread done";
            break;
        case VK_OPERATION_DEFERRED_KHR:
            msg = "operation deferred";
            break;
        case VK_OPERATION_NOT_DEFERRED_KHR:
            msg = "operation not deferred";
            break;
        case VK_PIPELINE_COMPILE_REQUIRED_EXT:
            msg = "pipeline compile required";
            break;
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            msg = "compression exhausted";
            break;
        case VK_RESULT_MAX_ENUM:
            msg = "max result enum value (something is very wrong)";
            break;
    }

    return msg;
}

const std::array<VkResult, 26> vk_res_errs = {
    VK_ERROR_OUT_OF_HOST_MEMORY,
    VK_ERROR_OUT_OF_DEVICE_MEMORY,
    VK_ERROR_INITIALIZATION_FAILED,
    VK_ERROR_DEVICE_LOST,
    VK_ERROR_MEMORY_MAP_FAILED,
    VK_ERROR_LAYER_NOT_PRESENT,
    VK_ERROR_EXTENSION_NOT_PRESENT,
    VK_ERROR_FEATURE_NOT_PRESENT,
    VK_ERROR_INCOMPATIBLE_DRIVER,
    VK_ERROR_TOO_MANY_OBJECTS,
    VK_ERROR_FORMAT_NOT_SUPPORTED,
    VK_ERROR_FRAGMENTED_POOL,
    VK_ERROR_UNKNOWN,
    VK_ERROR_OUT_OF_POOL_MEMORY,
    VK_ERROR_INVALID_EXTERNAL_HANDLE,
    VK_ERROR_FRAGMENTATION,
    VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
    VK_ERROR_SURFACE_LOST_KHR,
    VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    VK_ERROR_OUT_OF_DATE_KHR,
    VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    VK_ERROR_VALIDATION_FAILED_EXT,
    VK_ERROR_INVALID_SHADER_NV,
    VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT,
    VK_ERROR_NOT_PERMITTED_EXT,
    VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,
};

void Vulkan::vk_try(VkResult res, std::string oper)
{
    log.attempt("Vulkan", oper);
    for (auto res_err : vk_res_errs) {
        if (res == res_err) {
            throw std::runtime_error("could not "
                                     + oper
                                     + "; Vulkan says: "
                                     + map_vk_result(res));
        }
    }
    log.enter(map_vk_result(res));
}

bool Vulkan::vkbool_to_bool(VkBool32 b)
{
    if (b == VK_TRUE) {
        return true;
    } else {
        return false;
    }
}

std::string Vulkan::api_ver_to_str(uint32_t ver)
{
    return std::to_string(VK_VERSION_MAJOR(ver)) + "." +
           std::to_string(VK_VERSION_MINOR(ver)) + "." +
           std::to_string(VK_VERSION_PATCH(ver));
}

Vulkan::Vulkan(std::vector<const char*> exts,
               std::vector<const char*> layers,
               SDL& sdl,
               bool debug)
    : inst{std::make_shared<Instance>(exts, layers)},
      dbg_msgr{inst, debug},
      surf{sdl, inst},
      phys_devs{inst, surf},
      logi_dev {
          std::make_shared<Device>(phys_devs.default_device(), inst)
      },
      swch{phys_devs.default_device(), logi_dev, surf}
{}

void Vulkan::minicomp_setup()
{
    using namespace vk;

    DescriptorSetLayoutBinding swapimg ({
        .binding_ndx   = 0,
        .type          = DescriptorType::strge_img,
        .count         = 1,
        .shader_stages = flgs(ShaderStageFlag::cmpte),
    });

    std::vector<DescriptorSetLayoutBinding> bndgs = {swapimg};

    auto d_layt = std::make_shared<DescriptorSetLayout>(logi_dev,
                                                        "scratch image",
                                                        bndgs);

    std::vector<DescriptorSetLayout::ptr> d_layts = {d_layt};

    auto p_layt = std::make_shared<PipelineLayout>(logi_dev, d_layts);

    ShaderModule::ptr minicomp_shdr;

    if (auto search = shdrs.find("minicomp"); search != shdrs.end()) {
        minicomp_shdr = search->second;
    } else {
        throw std::runtime_error("failed to find shader 'minicomp'");
    }

    ComputePipeline pipel {logi_dev, minicomp_shdr, p_layt};

    DescriptorPool descpool {logi_dev, d_layts};

    Heap heap {logi_dev, phys_devs.default_device()};

    Image scratch {logi_dev, {
        .extent = {
            .width  = surf.width(),
            .height = surf.height(),
            .depth  = 1,
        },
        .usage  = flgs(ImageUsageFlag::strge)
                  | flgs(ImageUsageFlag::trnsfr_src),
        .format = v(Format::r8g8b8a8_uint),
    }};

    heap.alloc_on_dev(scratch);

    ImageView scratch_v {scratch};

    auto cmd_pool = std::make_shared<CommandPool>(logi_dev,
                                                  Device::compute_queue);

    CommandBuffer cmd_buff {logi_dev, cmd_pool};

    descpool.write()
            .storage_image("scratch image", 0, 0, &scratch_v)
            .submit();

    cmd_buff.begin()
            .bind(pipel, {descpool["scratch image"]})
            .barrier(scratch,
                     PipelineStageFlag::top_of_pipe,
                     PipelineStageFlag::cmpte_shader,
                     AccessFlag::none,
                     AccessFlag::shader_write,
                     ImageLayout::undfnd,
                     ImageLayout::gnrl,
                     ImageAspectFlag::color)
            .dispatch(scratch.extent().width, scratch.extent().height)
            .end();

    Fence fnce {logi_dev};

    logi_dev->submit(Device::compute_queue, cmd_buff, fnce);

    cmd_pool->reset();

    swch.next(fnce);

    cmd_buff.begin()
            .barrier(scratch,
                     PipelineStageFlag::top_of_pipe,
                     PipelineStageFlag::trnsfr,
                     AccessFlag::none,
                     AccessFlag::trnsfr_read,
                     ImageLayout::gnrl,
                     ImageLayout::trnsfr_src_optml,
                     ImageAspectFlag::color)
            .barrier(swch.img(),
                     PipelineStageFlag::top_of_pipe,
                     PipelineStageFlag::trnsfr,
                     AccessFlag::none,
                     AccessFlag::trnsfr_write,
                     ImageLayout::undfnd,
                     ImageLayout::trnsfr_dst_optml,
                     ImageAspectFlag::color)
            .copy(scratch, swch.img())
            .barrier(swch.img(),
                     PipelineStageFlag::trnsfr,
                     PipelineStageFlag::bottom_of_pipe,
                     AccessFlag::trnsfr_write,
                     AccessFlag::none,
                     ImageLayout::trnsfr_dst_optml,
                     ImageLayout::prsnt_src,
                     ImageAspectFlag::color)
            .end();

    logi_dev->submit(Device::compute_queue, cmd_buff, fnce);

    while (!logi_dev->present(swch)) swch.recreate();
}

void Vulkan::add_shader(std::string name, BinFile f)
{
    log.enter("Vulkan",  "adding shader " + name);
    log.indent();
    log.enter("path", std::string(f.path()));
    log.brk();

    bool ok = shdrs.insert({name, std::make_shared<ShaderModule>(logi_dev,
                                                                 name,
                                                                 f)})
                   .second;

    if (!ok) {
        throw std::runtime_error("failed to add shader " + name);
    }
}

} // namespace cu
