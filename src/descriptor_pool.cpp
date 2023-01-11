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

#ifndef Qb50852edc8a428cae6c2f42e3fe5a89
#define Qb50852edc8a428cae6c2f42e3fe5a89

#include "descriptor_pool.hpp"
#include "vulkan.hpp"
#include "descriptor_set_layout.hpp"

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <functional>

namespace cu {

// from https://en.cppreference.com/w/cpp/algorithm/ranges/fold_left, coming in
// the stdlib in C++23
struct fold_left_fn
{
    template<
        std::input_iterator I, std::sentinel_for<I> S, class T, typename F
    >
    constexpr auto operator()( I first, S last, T init, F f ) const
    {
        using U = std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>;
        if (first == last)
            return U(std::move(init));
        U accum = std::invoke(f, std::move(init), *first);
        for (++first; first != last; ++first)
            accum = std::invoke(f, std::move(accum), *first);
        return std::move(accum);
    }
    template<
        std::ranges::input_range R, class T, typename F >
    constexpr auto operator()( R&& r, T init, F f ) const
    {
        return (*this)(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::ref(f));
    }
};

inline constexpr fold_left_fn fold_left;

DescriptorPool::DescriptorPool(Device::ptr l_dev,
                               std::vector<DescriptorSetLayout::ptr>
                                   layts)
    : Deviced(l_dev, "descriptor pool", "DescriptorPool"),
      desc_sets(layts.size()),
      alloc_desc_sets {
          reinterpret_cast<PFN_vkAllocateDescriptorSets>(
              dev->get_proc_addr("vkAllocateDescriptorSets")
          )
      }
{
    uint32_t max_descs = layts.size();

    std::unordered_map<VkDescriptorType, uint32_t> dt_allocs;
    for (auto l : layts) {
        for (auto b : l->innerbindings()) {
            if (dt_allocs.contains(b.descriptorType)) {
                dt_allocs[b.descriptorType] += b.descriptorCount;
            } else {
                dt_allocs[b.descriptorType] = b.descriptorCount;
            }
        }
    }

    std::vector<VkDescriptorPoolSize> dp_sizes(dt_allocs.size());
    std::transform(dt_allocs.cbegin(), dt_allocs.cend(), dp_sizes.begin(),
                   [](const auto& a)
                   {
                       VkDescriptorPoolSize out = {
                          .type            = a.first,
                          .descriptorCount = a.second
                       };

                       return out;
                   });

    VkDescriptorPoolCreateInfo inf {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .maxSets = max_descs,
                         // safe bc VkDescriptorType has far less than
                         // UINT32_MAX elements
        .poolSizeCount = static_cast<uint32_t>(dp_sizes.size()),
        .pPoolSizes    = dp_sizes.data(),
    };

    Vulkan::vk_try(create(dev->inner(), &inf, NULL, &nner),
                   "creating " + descrptn());

    log.indent();
    if (inf.flags) {
        log.enter("flags", vk::dscrpt_pool_create_flags_cstrs(inf.flags));
    }
    log.enter("max sets", inf.maxSets);
    log.enter("pool sizes:");
    for (const auto& s : dp_sizes) {
        log.indent(2);
        log.enter("type", vk::dscrpt_type_str(s.type));
        log.indent(3);
        log.enter("count", s.descriptorCount);
    }
    log.brk();

    std::vector<VkDescriptorSetLayout> layts_hs(layts.size());
    std::transform(layts.begin(), layts.end(), layts_hs.begin(),
                   [](auto a){ return a->inner(); });

    VkDescriptorSetAllocateInfo ds_alloc_inf {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext              = NULL,
        .descriptorPool     = nner,
        .descriptorSetCount = max_descs,
        .pSetLayouts        = layts_hs.data(),
    };

    Vulkan::vk_try(alloc_desc_sets(l_dev->inner(),
                                   &ds_alloc_inf,
                                   desc_sets.data()),
                   "allocating descriptor sets from pool");

    for (std::size_t i = 0; i < layts.size(); ++i) {
        desc_set_map[layts.at(i)->name()] = &desc_sets.at(i);

        log.indent(1);
        log.enter("set " + std::to_string(i) + ":");
        layts.at(i)->log_attrs(2);
    }
}

VkDescriptorSet DescriptorPool::operator[](std::string name)
{
    return *desc_set_map.at(name);
}

} // namespace cu

#endif
