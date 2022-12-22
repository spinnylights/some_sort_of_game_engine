#ifndef dca51d742abb4a1e937d117355829212
#define dca51d742abb4a1e937d117355829212

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <algorithm>

namespace cu {

struct VulkanUtil {

#define SWITCH_VK_BOTTOM \
            return "max enum; something is very wrong"; \
        default: \
            return "switch fell through; something is very wrong";

    static constexpr std::string format_str(VkFormat fmt) {
        switch(fmt) {
        case VK_FORMAT_UNDEFINED:
            return "UNDEFINED";
        case VK_FORMAT_R4G4_UNORM_PACK8:
            return "R4G4 UNORM PACK8";
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            return "R4G4B4A4 UNORM PACK16";
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            return "B4G4R4A4 UNORM PACK16";
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            return "R5G6B5 UNORM PACK16";
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            return "B5G6R5 UNORM PACK16";
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            return "R5G5B5A1 UNORM PACK16";
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            return "B5G5R5A1 UNORM PACK16";
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            return "A1R5G5B5 UNORM PACK16";
        case VK_FORMAT_R8_UNORM:
            return "R8 UNORM";
        case VK_FORMAT_R8_SNORM:
            return "R8 SNORM";
        case VK_FORMAT_R8_USCALED:
            return "R8 USCALED";
        case VK_FORMAT_R8_SSCALED:
            return "R8 SSCALED";
        case VK_FORMAT_R8_UINT:
            return "R8 UINT";
        case VK_FORMAT_R8_SINT:
            return "R8 SINT";
        case VK_FORMAT_R8_SRGB:
            return "R8 SRGB";
        case VK_FORMAT_R8G8_UNORM:
            return "R8G8 UNORM";
        case VK_FORMAT_R8G8_SNORM:
            return "R8G8 SNORM";
        case VK_FORMAT_R8G8_USCALED:
            return "R8G8 USCALED";
        case VK_FORMAT_R8G8_SSCALED:
            return "R8G8 SSCALED";
        case VK_FORMAT_R8G8_UINT:
            return "R8G8 UINT";
        case VK_FORMAT_R8G8_SINT:
            return "R8G8 SINT";
        case VK_FORMAT_R8G8_SRGB:
            return "R8G8 SRGB";
        case VK_FORMAT_R8G8B8_UNORM:
            return "R8G8B8 UNORM";
        case VK_FORMAT_R8G8B8_SNORM:
            return "R8G8B8 SNORM";
        case VK_FORMAT_R8G8B8_USCALED:
            return "R8G8B8 USCALED";
        case VK_FORMAT_R8G8B8_SSCALED:
            return "R8G8B8 SSCALED";
        case VK_FORMAT_R8G8B8_UINT:
            return "R8G8B8 UINT";
        case VK_FORMAT_R8G8B8_SINT:
            return "R8G8B8 SINT";
        case VK_FORMAT_R8G8B8_SRGB:
            return "R8G8B8 SRGB";
        case VK_FORMAT_B8G8R8_UNORM:
            return "B8G8R8 UNORM";
        case VK_FORMAT_B8G8R8_SNORM:
            return "B8G8R8 SNORM";
        case VK_FORMAT_B8G8R8_USCALED:
            return "B8G8R8 USCALED";
        case VK_FORMAT_B8G8R8_SSCALED:
            return "B8G8R8 SSCALED";
        case VK_FORMAT_B8G8R8_UINT:
            return "B8G8R8 UINT";
        case VK_FORMAT_B8G8R8_SINT:
            return "B8G8R8 SINT";
        case VK_FORMAT_B8G8R8_SRGB:
            return "B8G8R8 SRGB";
        case VK_FORMAT_R8G8B8A8_UNORM:
            return "R8G8B8A8 UNORM";
        case VK_FORMAT_R8G8B8A8_SNORM:
            return "R8G8B8A8 SNORM";
        case VK_FORMAT_R8G8B8A8_USCALED:
            return "R8G8B8A8 USCALED";
        case VK_FORMAT_R8G8B8A8_SSCALED:
            return "R8G8B8A8 SSCALED";
        case VK_FORMAT_R8G8B8A8_UINT:
            return "R8G8B8A8 UINT";
        case VK_FORMAT_R8G8B8A8_SINT:
            return "R8G8B8A8 SINT";
        case VK_FORMAT_R8G8B8A8_SRGB:
            return "R8G8B8A8 SRGB";
        case VK_FORMAT_B8G8R8A8_UNORM:
            return "B8G8R8A8 UNORM";
        case VK_FORMAT_B8G8R8A8_SNORM:
            return "B8G8R8A8 SNORM";
        case VK_FORMAT_B8G8R8A8_USCALED:
            return "B8G8R8A8 USCALED";
        case VK_FORMAT_B8G8R8A8_SSCALED:
            return "B8G8R8A8 SSCALED";
        case VK_FORMAT_B8G8R8A8_UINT:
            return "B8G8R8A8 UINT";
        case VK_FORMAT_B8G8R8A8_SINT:
            return "B8G8R8A8 SINT";
        case VK_FORMAT_B8G8R8A8_SRGB:
            return "B8G8R8A8 SRGB";
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return "A8B8G8R8 UNORM PACK32";
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return "A8B8G8R8 SNORM PACK32";
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            return "A8B8G8R8 USCALED PACK32";
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            return "A8B8G8R8 SSCALED PACK32";
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return "A8B8G8R8 UINT PACK32";
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return "A8B8G8R8 SINT PACK32";
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return "A8B8G8R8 SRGB PACK32";
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return "A2R10G10B10 UNORM PACK32";
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            return "A2R10G10B10 SNORM PACK32";
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            return "A2R10G10B10 USCALED PACK32";
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            return "A2R10G10B10 SSCALED PACK32";
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            return "A2R10G10B10 UINT PACK32";
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            return "A2R10G10B10 SINT PACK32";
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            return "A2B10G10R10 UNORM PACK32";
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            return "A2B10G10R10 SNORM PACK32";
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            return "A2B10G10R10 USCALED PACK32";
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            return "A2B10G10R10 SSCALED PACK32";
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            return "A2B10G10R10 UINT PACK32";
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            return "A2B10G10R10 SINT PACK32";
        case VK_FORMAT_R16_UNORM:
            return "R16 UNORM";
        case VK_FORMAT_R16_SNORM:
            return "R16 SNORM";
        case VK_FORMAT_R16_USCALED:
            return "R16 USCALED";
        case VK_FORMAT_R16_SSCALED:
            return "R16 SSCALED";
        case VK_FORMAT_R16_UINT:
            return "R16 UINT";
        case VK_FORMAT_R16_SINT:
            return "R16 SINT";
        case VK_FORMAT_R16_SFLOAT:
            return "R16 SFLOAT";
        case VK_FORMAT_R16G16_UNORM:
            return "R16G16 UNORM";
        case VK_FORMAT_R16G16_SNORM:
            return "R16G16 SNORM";
        case VK_FORMAT_R16G16_USCALED:
            return "R16G16 USCALED";
        case VK_FORMAT_R16G16_SSCALED:
            return "R16G16 SSCALED";
        case VK_FORMAT_R16G16_UINT:
            return "R16G16 UINT";
        case VK_FORMAT_R16G16_SINT:
            return "R16G16 SINT";
        case VK_FORMAT_R16G16_SFLOAT:
            return "R16G16 SFLOAT";
        case VK_FORMAT_R16G16B16_UNORM:
            return "R16G16B16 UNORM";
        case VK_FORMAT_R16G16B16_SNORM:
            return "R16G16B16 SNORM";
        case VK_FORMAT_R16G16B16_USCALED:
            return "R16G16B16 USCALED";
        case VK_FORMAT_R16G16B16_SSCALED:
            return "R16G16B16 SSCALED";
        case VK_FORMAT_R16G16B16_UINT:
            return "R16G16B16 UINT";
        case VK_FORMAT_R16G16B16_SINT:
            return "R16G16B16 SINT";
        case VK_FORMAT_R16G16B16_SFLOAT:
            return "R16G16B16 SFLOAT";
        case VK_FORMAT_R16G16B16A16_UNORM:
            return "R16G16B16A16 UNORM";
        case VK_FORMAT_R16G16B16A16_SNORM:
            return "R16G16B16A16 SNORM";
        case VK_FORMAT_R16G16B16A16_USCALED:
            return "R16G16B16A16 USCALED";
        case VK_FORMAT_R16G16B16A16_SSCALED:
            return "R16G16B16A16 SSCALED";
        case VK_FORMAT_R16G16B16A16_UINT:
            return "R16G16B16A16 UINT";
        case VK_FORMAT_R16G16B16A16_SINT:
            return "R16G16B16A16 SINT";
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return "R16G16B16A16 SFLOAT";
        case VK_FORMAT_R32_UINT:
            return "R32 UINT";
        case VK_FORMAT_R32_SINT:
            return "R32 SINT";
        case VK_FORMAT_R32_SFLOAT:
            return "R32 SFLOAT";
        case VK_FORMAT_R32G32_UINT:
            return "R32G32 UINT";
        case VK_FORMAT_R32G32_SINT:
            return "R32G32 SINT";
        case VK_FORMAT_R32G32_SFLOAT:
            return "R32G32 SFLOAT";
        case VK_FORMAT_R32G32B32_UINT:
            return "R32G32B32 UINT";
        case VK_FORMAT_R32G32B32_SINT:
            return "R32G32B32 SINT";
        case VK_FORMAT_R32G32B32_SFLOAT:
            return "R32G32B32 SFLOAT";
        case VK_FORMAT_R32G32B32A32_UINT:
            return "R32G32B32A32 UINT";
        case VK_FORMAT_R32G32B32A32_SINT:
            return "R32G32B32A32 SINT";
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return "R32G32B32A32 SFLOAT";
        case VK_FORMAT_R64_UINT:
            return "R64 UINT";
        case VK_FORMAT_R64_SINT:
            return "R64 SINT";
        case VK_FORMAT_R64_SFLOAT:
            return "R64 SFLOAT";
        case VK_FORMAT_R64G64_UINT:
            return "R64G64 UINT";
        case VK_FORMAT_R64G64_SINT:
            return "R64G64 SINT";
        case VK_FORMAT_R64G64_SFLOAT:
            return "R64G64 SFLOAT";
        case VK_FORMAT_R64G64B64_UINT:
            return "R64G64B64 UINT";
        case VK_FORMAT_R64G64B64_SINT:
            return "R64G64B64 SINT";
        case VK_FORMAT_R64G64B64_SFLOAT:
            return "R64G64B64 SFLOAT";
        case VK_FORMAT_R64G64B64A64_UINT:
            return "R64G64B64A64 UINT";
        case VK_FORMAT_R64G64B64A64_SINT:
            return "R64G64B64A64 SINT";
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return "R64G64B64A64 SFLOAT";
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return "B10G11R11 UFLOAT PACK32";
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            return "E5B9G9R9 UFLOAT PACK32";
        case VK_FORMAT_D16_UNORM:
            return "D16 UNORM";
        case VK_FORMAT_X8_D24_UNORM_PACK32:
            return "X8 D24 UNORM PACK32";
        case VK_FORMAT_D32_SFLOAT:
            return "D32 SFLOAT";
        case VK_FORMAT_S8_UINT:
            return "S8 UINT";
        case VK_FORMAT_D16_UNORM_S8_UINT:
            return "D16 UNORM S8 UINT";
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return "D24 UNORM S8 UINT";
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return "D32 SFLOAT S8 UINT";
        case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
            return "BC1 RGB UNORM BLOCK";
        case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
            return "BC1 RGB SRGB BLOCK";
        case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
            return "BC1 RGBA UNORM BLOCK";
        case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
            return "BC1 RGBA SRGB BLOCK";
        case VK_FORMAT_BC2_UNORM_BLOCK:
            return "BC2 UNORM BLOCK";
        case VK_FORMAT_BC2_SRGB_BLOCK:
            return "BC2 SRGB BLOCK";
        case VK_FORMAT_BC3_UNORM_BLOCK:
            return "BC3 UNORM BLOCK";
        case VK_FORMAT_BC3_SRGB_BLOCK:
            return "BC3 SRGB BLOCK";
        case VK_FORMAT_BC4_UNORM_BLOCK:
            return "BC4 UNORM BLOCK";
        case VK_FORMAT_BC4_SNORM_BLOCK:
            return "BC4 SNORM BLOCK";
        case VK_FORMAT_BC5_UNORM_BLOCK:
            return "BC5 UNORM BLOCK";
        case VK_FORMAT_BC5_SNORM_BLOCK:
            return "BC5 SNORM BLOCK";
        case VK_FORMAT_BC6H_UFLOAT_BLOCK:
            return "BC6H UFLOAT BLOCK";
        case VK_FORMAT_BC6H_SFLOAT_BLOCK:
            return "BC6H SFLOAT BLOCK";
        case VK_FORMAT_BC7_UNORM_BLOCK:
            return "BC7 UNORM BLOCK";
        case VK_FORMAT_BC7_SRGB_BLOCK:
            return "BC7 SRGB BLOCK";
        case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
            return "ETC2 R8G8B8 UNORM BLOCK";
        case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
            return "ETC2 R8G8B8 SRGB BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
            return "ETC2 R8G8B8A1 UNORM BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
            return "ETC2 R8G8B8A1 SRGB BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
            return "ETC2 R8G8B8A8 UNORM BLOCK";
        case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
            return "ETC2 R8G8B8A8 SRGB BLOCK";
        case VK_FORMAT_EAC_R11_UNORM_BLOCK:
            return "EAC R11 UNORM BLOCK";
        case VK_FORMAT_EAC_R11_SNORM_BLOCK:
            return "EAC R11 SNORM BLOCK";
        case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
            return "EAC R11G11 UNORM BLOCK";
        case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
            return "EAC R11G11 SNORM BLOCK";
        case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
            return "ASTC 4x4 UNORM BLOCK";
        case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
            return "ASTC 4x4 SRGB BLOCK";
        case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
            return "ASTC 5x4 UNORM BLOCK";
        case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
            return "ASTC 5x4 SRGB BLOCK";
        case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
            return "ASTC 5x5 UNORM BLOCK";
        case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
            return "ASTC 5x5 SRGB BLOCK";
        case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
            return "ASTC 6x5 UNORM BLOCK";
        case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
            return "ASTC 6x5 SRGB BLOCK";
        case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
            return "ASTC 6x6 UNORM BLOCK";
        case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
            return "ASTC 6x6 SRGB BLOCK";
        case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
            return "ASTC 8x5 UNORM BLOCK";
        case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
            return "ASTC 8x5 SRGB BLOCK";
        case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
            return "ASTC 8x6 UNORM BLOCK";
        case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
            return "ASTC 8x6 SRGB BLOCK";
        case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
            return "ASTC 8x8 UNORM BLOCK";
        case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
            return "ASTC 8x8 SRGB BLOCK";
        case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
            return "ASTC 10x5 UNORM BLOCK";
        case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
            return "ASTC 10x5 SRGB BLOCK";
        case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
            return "ASTC 10x6 UNORM BLOCK";
        case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
            return "ASTC 10x6 SRGB BLOCK";
        case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
            return "ASTC 10x8 UNORM BLOCK";
        case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
            return "ASTC 10x8 SRGB BLOCK";
        case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
            return "ASTC 10x10 UNORM BLOCK";
        case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
            return "ASTC 10x10 SRGB BLOCK";
        case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
            return "ASTC 12x10 UNORM BLOCK";
        case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
            return "ASTC 12x10 SRGB BLOCK";
        case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
            return "ASTC 12x12 UNORM BLOCK";
        case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
            return "ASTC 12x12 SRGB BLOCK";
        case VK_FORMAT_G8B8G8R8_422_UNORM:
            return "G8B8G8R8 422 UNORM";
        case VK_FORMAT_B8G8R8G8_422_UNORM:
            return "B8G8R8G8 422 UNORM";
        case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
            return "G8 B8 R8 3PLANE 420 UNORM";
        case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
            return "G8 B8R8 2PLANE 420 UNORM";
        case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
            return "G8 B8 R8 3PLANE 422 UNORM";
        case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
            return "G8 B8R8 2PLANE 422 UNORM";
        case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
            return "G8 B8 R8 3PLANE 444 UNORM";
        case VK_FORMAT_R10X6_UNORM_PACK16:
            return "R10X6 UNORM PACK16";
        case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
            return "R10X6G10X6 UNORM 2PACK16";
        case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
            return "R10X6G10X6B10X6A10X6 UNORM 4PACK16";
        case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
            return "G10X6B10X6G10X6R10X6 422 UNORM 4PACK16";
        case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
            return "B10X6G10X6R10X6G10X6 422 UNORM 4PACK16";
        case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
            return "G10X6 B10X6 R10X6 3PLANE 420 UNORM 3PACK16";
        case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
            return "G10X6 B10X6R10X6 2PLANE 420 UNORM 3PACK16";
        case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
            return "G10X6 B10X6 R10X6 3PLANE 422 UNORM 3PACK16";
        case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
            return "G10X6 B10X6R10X6 2PLANE 422 UNORM 3PACK16";
        case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
            return "G10X6 B10X6 R10X6 3PLANE 444 UNORM 3PACK16";
        case VK_FORMAT_R12X4_UNORM_PACK16:
            return "R12X4 UNORM PACK16";
        case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
            return "R12X4G12X4 UNORM 2PACK16";
        case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
            return "R12X4G12X4B12X4A12X4 UNORM 4PACK16";
        case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
            return "G12X4B12X4G12X4R12X4 422 UNORM 4PACK16";
        case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
            return "B12X4G12X4R12X4G12X4 422 UNORM 4PACK16";
        case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
            return "G12X4 B12X4 R12X4 3PLANE 420 UNORM 3PACK16";
        case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
            return "G12X4 B12X4R12X4 2PLANE 420 UNORM 3PACK16";
        case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
            return "G12X4 B12X4 R12X4 3PLANE 422 UNORM 3PACK16";
        case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
            return "G12X4 B12X4R12X4 2PLANE 422 UNORM 3PACK16";
        case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
            return "G12X4 B12X4 R12X4 3PLANE 444 UNORM 3PACK16";
        case VK_FORMAT_G16B16G16R16_422_UNORM:
            return "G16B16G16R16 422 UNORM";
        case VK_FORMAT_B16G16R16G16_422_UNORM:
            return "B16G16R16G16 422 UNORM";
        case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
            return "G16 B16 R16 3PLANE 420 UNORM";
        case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
            return "G16 B16R16 2PLANE 420 UNORM";
        case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
            return "G16 B16 R16 3PLANE 422 UNORM";
        case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
            return "G16 B16R16 2PLANE 422 UNORM";
        case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
            return "G16 B16 R16 3PLANE 444 UNORM";
        case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM:
            return "G8 B8R8 2PLANE 444 UNORM";
        case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:
            return "G10X6 B10X6R10X6 2PLANE 444 UNORM 3PACK16";
        case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:
            return "G12X4 B12X4R12X4 2PLANE 444 UNORM 3PACK16";
        case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM:
            return "G16 B16R16 2PLANE 444 UNORM";
        case VK_FORMAT_A4R4G4B4_UNORM_PACK16:
            return "A4R4G4B4 UNORM PACK16";
        case VK_FORMAT_A4B4G4R4_UNORM_PACK16:
            return "A4B4G4R4 UNORM PACK16";
        case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK:
            return "ASTC 4x4 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK:
            return "ASTC 5x4 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK:
            return "ASTC 5x5 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK:
            return "ASTC 6x5 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK:
            return "ASTC 6x6 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK:
            return "ASTC 8x5 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK:
            return "ASTC 8x6 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK:
            return "ASTC 8x8 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK:
            return "ASTC 10x5 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK:
            return "ASTC 10x6 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK:
            return "ASTC 10x8 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK:
            return "ASTC 10x10 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK:
            return "ASTC 12x10 SFLOAT BLOCK";
        case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK:
            return "ASTC 12x12 SFLOAT BLOCK";
        case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
            return "PVRTC1 2BPP UNORM BLOCK IMG";
        case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
            return "PVRTC1 4BPP UNORM BLOCK IMG";
        case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
            return "PVRTC2 2BPP UNORM BLOCK IMG";
        case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
            return "PVRTC2 4BPP UNORM BLOCK IMG";
        case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
            return "PVRTC1 2BPP SRGB BLOCK IMG";
        case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
            return "PVRTC1 4BPP SRGB BLOCK IMG";
        case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
            return "PVRTC2 2BPP SRGB BLOCK IMG";
        case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
            return "PVRTC2 4BPP SRGB BLOCK IMG";
        case VK_FORMAT_MAX_ENUM:
            return "MAX ENUM";
        default:
            return "switch fell through; something is very wrong";
        }
    }

    static constexpr std::string present_mode_str(VkPresentModeKHR mode) {
        switch(mode) {
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
            return "immediate";
        case VK_PRESENT_MODE_MAILBOX_KHR:
            return "mailbox";
        case VK_PRESENT_MODE_FIFO_KHR:
            return "FIFO";
        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
            return "FIFO relaxed";
        case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
            return "shared demand refresh";
        case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
            return "shared continuous refresh";
        case VK_PRESENT_MODE_MAX_ENUM_KHR:
        SWITCH_VK_BOTTOM
        }
    }

    static constexpr std::string color_space_to_str(VkColorSpaceKHR cs)
    {
        if (cs == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
                return "SRGB nonlinear";
        } else {
            return std::to_string(cs);
        }
    }

    static constexpr std::vector<const char*>
    img_usage_to_cstrs(VkImageUsageFlags f)
    {
        std::vector<const char *> cstrs;

        if (f & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
            cstrs.push_back("transfer source");
        }

        if (f & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
            cstrs.push_back("transfer destination");
        }

        if (f & VK_IMAGE_USAGE_SAMPLED_BIT) {
            cstrs.push_back("sampled");
        }

        if (f & VK_IMAGE_USAGE_STORAGE_BIT) {
            cstrs.push_back("storage");
        }

        if (f & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
            cstrs.push_back("color attachment");
        }

        if (f & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            cstrs.push_back("depth/stencil");
        }

        if (f & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
            cstrs.push_back("transient attachment (lazily allocated)");
        }

        if (f & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) {
            cstrs.push_back("input attachment");
        }

        return cstrs;
    }

    static constexpr std::string sharing_mode_to_str(VkSharingMode sm)
    {
        switch (sm) {
        case VK_SHARING_MODE_EXCLUSIVE:
            return "exclusive";
        case VK_SHARING_MODE_CONCURRENT:
            return "concurrent";
        case VK_SHARING_MODE_MAX_ENUM:
            SWITCH_VK_BOTTOM
        }
    }

    static constexpr std::string smpl_count_to_str(VkSampleCountFlagBits f)
    {
        switch (f) {
        case VK_SAMPLE_COUNT_1_BIT:
            return "1";
        case VK_SAMPLE_COUNT_2_BIT:
            return "2";
        case VK_SAMPLE_COUNT_4_BIT:
            return "4";
        case VK_SAMPLE_COUNT_8_BIT:
            return "8";
        case VK_SAMPLE_COUNT_16_BIT:
            return "16";
        case VK_SAMPLE_COUNT_32_BIT:
            return "32";
        case VK_SAMPLE_COUNT_64_BIT:
            return "64";
        case VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM:
        SWITCH_VK_BOTTOM
        }
    }

    static constexpr std::string tiling_to_str(VkImageTiling it)
    {
        switch (it) {
        case VK_IMAGE_TILING_OPTIMAL:
            return "optimal";
        case VK_IMAGE_TILING_LINEAR:
            return "linear";
        case VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT:
            return "linux DRM format modifier";
        case VK_IMAGE_TILING_MAX_ENUM:
        SWITCH_VK_BOTTOM
        }
    }

    static constexpr std::string img_layout_to_str(VkImageLayout il)
    {
        switch(il) {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            return "undefined";
        case VK_IMAGE_LAYOUT_GENERAL:
            return "general";
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            return "color attachment optimal";
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            return "depth stencil attachment optimal";
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
            return "depth stencil read only optimal";
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            return "shader read only optimal";
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            return "transfer src optimal";
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            return "transfer dst optimal";
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            return "preinitialized";
        case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
            return "depth read only stencil attachment optimal";
        case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
            return "depth attachment stencil read only optimal";
        case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
            return "depth attachment optimal";
        case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
            return "depth read only optimal";
        case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
            return "stencil attachment optimal";
        case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
            return "stencil read only optimal";
        case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:
            return "read only optimal";
        case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:
            return "attachment optimal";
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            return "present src (KHR)";
        case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
            return "shared present (KHR)";
        case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
            return "fragment density map optimal (EXT)";
        case VK_IMAGE_LAYOUT_MAX_ENUM:
        SWITCH_VK_BOTTOM
        }
    }

    static constexpr std::string img_view_type_str(VkImageViewType vt)
    {
        switch(vt) {
        case VK_IMAGE_VIEW_TYPE_1D:
            return "1D";
        case VK_IMAGE_VIEW_TYPE_2D:
            return "2D";
        case VK_IMAGE_VIEW_TYPE_3D:
            return "3D";
        case VK_IMAGE_VIEW_TYPE_CUBE:
            return "cube";
        case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
            return "1D array";
        case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
            return "2D array";
        case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
            return "cube array";
        case VK_IMAGE_VIEW_TYPE_MAX_ENUM:
        SWITCH_VK_BOTTOM
        }
    }

    static constexpr std::string img_type_str(VkImageType it)
    {
        switch(it) {
        case VK_IMAGE_TYPE_1D:
            return "1D";
        case VK_IMAGE_TYPE_2D:
            return "2D";
        case VK_IMAGE_TYPE_3D:
            return "3D";
        case VK_IMAGE_TYPE_MAX_ENUM:
        SWITCH_VK_BOTTOM
        }
    }
};

} // namespace cu 

#endif
