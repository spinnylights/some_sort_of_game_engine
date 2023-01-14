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

#ifndef q765e99c966249c6aa35adad25b72458
#define q765e99c966249c6aa35adad25b72458

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "instance.hpp"
#include "debug_msgr.hpp"
#include "surface.hpp"
#include "phys_devices.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "shader_module.hpp"
#include "heap.hpp"
#include "command_pool.hpp"
#include "descriptor_pool.hpp"
#include "compute_pipeline.hpp"

namespace cu {

class SDL;

#define STRINGIFY(arg) #arg

#define GET_VK_FN_PTR(member_name,vk_fn)\
    member_name {\
        reinterpret_cast<PFN_vk##vk_fn>(\
            dev->get_proc_addr(STRINGIFY(vk##vk_fn))\
        )\
    }

#define GET_VK_FN_PTR_INST(member_name,vk_fn)\
    member_name {\
        reinterpret_cast<PFN_vk##vk_fn>(\
            inst->get_proc_addr(STRINGIFY(vk##vk_fn))\
        )\
    }

/*!
 * \brief The entry point into the Vulkan code; coordinates the
 * activity of the Vulkan classes.
 */
class Vulkan {
public:
    /*!
     * \brief A convenient way to check the return code of a
     * Vulkan operation. Throws in the event of a VK_ERROR* code
     * (see the
     * [VkResult](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkResult.html)
     * entry in the Vulkan manual). Behaves similarly to
     * SDL::sdl_try().
     *
     * \copydetails SDL::sdl_try()
     */
    static void vk_try(VkResult result, std::string oper);

    /*!
     * \brief Converts a Vulkan boolean to a C++ boolean.
     */
    static bool vkbool_to_bool(VkBool32);

    /*!
     * \brief Converts a Vulkan API version value to a
     * human-readable string.
     */
    static std::string api_ver_to_str(uint32_t ver);

    /*!
     * \brief (constructor) Initializes Vulkan, including the
     * various components needed.
     *
     * \param exts The extensions required.
     * \param layers The layers to enable.
     * \param sdl The SDL instance in use.
     * \param debug Whether to enable the debug utils.
     */
    Vulkan(std::vector<const char*> exts,
           std::vector<const char*> layers,
           SDL& sdl,
           bool debug = false);

    Vulkan(Vulkan&&) = delete;
    Vulkan(const Vulkan&) = delete;
    Vulkan& operator=(const Vulkan&) = delete;

    ~Vulkan() = default;

    /*!
     * \brief Add a new compiled (SPIR-V) shader.
     */
    void add_shader(std::string name, BinFile compiled_shader);

    // TODO: replace with something more general-purpose
    void minicomp_setup();

    void minicomp_frame();

private:
    Instance::ptr inst;
    DebugMsgr dbg_msgr;

private:
    Surface surf;
    PhysDevices phys_devs;
    Device::ptr logi_dev;

private:
    Heap heap;

private:
    Swapchain swch;

private:
    std::unordered_map<std::string, ShaderModule::ptr> shdrs;

private:
    struct minicomp_state {
        std::vector<DescriptorSetLayoutBinding> bns;
        std::vector<DescriptorSetLayout::ptr> dls;
        PipelineLayout::ptr pl;
        ShaderModule::ptr shdr;
        ComputePipeline* ppl;
        DescriptorPool* descpl;
        Image* scrtch;
        Heap::handle_t scrtch_h;
        ImageView* scrtch_v;
        CommandPool::ptr cmdp;
        CommandBuffer* cmdb;
        Fence* f;

        std::vector<DescriptorSetLayoutBinding>& bndgs() { return bns; }
        void bndgs(std::vector<DescriptorSetLayoutBinding> b) { bns = b; }

        std::vector<DescriptorSetLayout::ptr>& d_layts() { return dls; }
        void d_layts(std::vector<DescriptorSetLayout::ptr> d) { dls = d; }

        PipelineLayout::ptr p_layt() { return pl; }
        void p_layt(PipelineLayout::ptr p) { pl = p; }

        ShaderModule::ptr minicomp_shdr() { return shdr; }
        void minicomp_shdr(ShaderModule::ptr p ) { shdr = p; }

        ComputePipeline& pipel() { return *ppl; }
        void pipel(ComputePipeline* newp) { ppl = newp; }

        DescriptorPool& descpool() { return *descpl; }
        void descpool(DescriptorPool* newp) { descpl = newp; }

        Image& scratch() { return *scrtch; }
        void scratch(Image* img) { scrtch = img; }

        ImageView& scratch_v() { return *scrtch_v; }
        void scratch_v(ImageView* imgv) { scrtch_v = imgv; }

        CommandPool::ptr cmd_pool() { return cmdp; }
        void cmd_pool(CommandPool::ptr newp) { cmdp = newp; }

        CommandBuffer& cmd_buff() { return *cmdb; }
        void cmd_buff(CommandBuffer* newp) { cmdb = newp; }

        Fence& fnce() { return *f; }
        void fnce(Fence* newp) { f = newp; }

        ~minicomp_state() noexcept;
    };

    minicomp_state minist = {};

    void minicomp_recreate_swch();
};

} // namespace cu

#endif
