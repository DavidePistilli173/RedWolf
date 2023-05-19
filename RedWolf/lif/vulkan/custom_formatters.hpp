#ifndef RW_LIF_VULKAN_CUSTOM_FORMATTERS_HPP
#define RW_LIF_VULKAN_CUSTOM_FORMATTERS_HPP

#include <format>
#include <vulkan/vulkan.h>

/**
 * @brief std::formatter specialization for VkPhysicalDeviceType.
 */
template<>
struct std::formatter<VkPhysicalDeviceType> : std::formatter<std::string>
{
   auto format(VkPhysicalDeviceType type, std::format_context& ctx)
   {
      switch (type)
      {
      case VK_PHYSICAL_DEVICE_TYPE_OTHER:
         return std::formatter<std::string>::format("OTHER DEVICE", ctx);
         break;
      case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
         return std::formatter<std::string>::format("INTEGRATED GPU", ctx);
         break;
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
         return std::formatter<std::string>::format("DISCRETE GPU", ctx);
         break;
      case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
         return std::formatter<std::string>::format("VIRTUAL GPU", ctx);
         break;
      case VK_PHYSICAL_DEVICE_TYPE_CPU:
         return std::formatter<std::string>::format("CPU", ctx);
         break;
      default:
         return std::formatter<std::string>::format("UNKNOWN DEVICE", ctx);
      }
   }
};

/**
 * @brief std::formatter specialization for VkResult.
 */
template<>
struct std::formatter<VkResult> : std::formatter<std::string>
{
   auto format(const VkResult& data, std::format_context& ctx)
   {
      switch (data)
      {
      case VkResult::VK_SUCCESS:
         return std::formatter<std::string>::format("VK_SUCCESS", ctx);
         break;
      case VkResult::VK_NOT_READY:
         return std::formatter<std::string>::format("VK_NOT_READY", ctx);
         break;
      case VkResult::VK_TIMEOUT:
         return std::formatter<std::string>::format("VK_TIMEOUT", ctx);
         break;
      case VkResult::VK_EVENT_SET:
         return std::formatter<std::string>::format("VK_EVENT_SET", ctx);
         break;
      case VkResult::VK_EVENT_RESET:
         return std::formatter<std::string>::format("VK_EVENT_RESET", ctx);
         break;
      case VkResult::VK_INCOMPLETE:
         return std::formatter<std::string>::format("VK_INCOMPLETE", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_HOST_MEMORY:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_HOST_MEMORY", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_DEVICE_MEMORY", ctx);
         break;
      case VkResult::VK_ERROR_INITIALIZATION_FAILED:
         return std::formatter<std::string>::format("VK_ERROR_INITIALIZATION_FAILED", ctx);
         break;
      case VkResult::VK_ERROR_DEVICE_LOST:
         return std::formatter<std::string>::format("VK_ERROR_DEVICE_LOST", ctx);
         break;
      case VkResult::VK_ERROR_MEMORY_MAP_FAILED:
         return std::formatter<std::string>::format("VK_ERROR_MEMORY_MAP_FAILED", ctx);
         break;
      case VkResult::VK_ERROR_LAYER_NOT_PRESENT:
         return std::formatter<std::string>::format("VK_ERROR_LAYER_NOT_PRESENT", ctx);
         break;
      case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT:
         return std::formatter<std::string>::format("VK_ERROR_EXTENSION_NOT_PRESENT", ctx);
         break;
      case VkResult::VK_ERROR_FEATURE_NOT_PRESENT:
         return std::formatter<std::string>::format("VK_ERROR_FEATURE_NOT_PRESENT", ctx);
         break;
      case VkResult::VK_ERROR_INCOMPATIBLE_DRIVER:
         return std::formatter<std::string>::format("VK_ERROR_INCOMPATIBLE_DRIVER", ctx);
         break;
      case VkResult::VK_ERROR_TOO_MANY_OBJECTS:
         return std::formatter<std::string>::format("VK_ERROR_TOO_MANY_OBJECTS", ctx);
         break;
      case VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED:
         return std::formatter<std::string>::format("VK_ERROR_FORMAT_NOT_SUPPORTED", ctx);
         break;
      case VkResult::VK_ERROR_FRAGMENTED_POOL:
         return std::formatter<std::string>::format("VK_ERROR_FRAGMENTED_POOL", ctx);
         break;
      case VkResult::VK_ERROR_UNKNOWN:
         return std::formatter<std::string>::format("VK_ERROR_UNKNOWN", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_POOL_MEMORY:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_POOL_MEMORY", ctx);
         break;
      case VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_EXTERNAL_HANDLE", ctx);
         break;
      case VkResult::VK_ERROR_FRAGMENTATION:
         return std::formatter<std::string>::format("VK_ERROR_FRAGMENTATION", ctx);
         break;
      case VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS", ctx);
         break;
      case VkResult::VK_PIPELINE_COMPILE_REQUIRED:
         return std::formatter<std::string>::format("VK_PIPELINE_COMPILE_REQUIRED", ctx);
         break;
      case VkResult::VK_ERROR_SURFACE_LOST_KHR:
         return std::formatter<std::string>::format("VK_ERROR_SURFACE_LOST_KHR", ctx);
         break;
      case VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
         return std::formatter<std::string>::format("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR", ctx);
         break;
      case VkResult::VK_SUBOPTIMAL_KHR:
         return std::formatter<std::string>::format("VK_SUBOPTIMAL_KHR", ctx);
         break;
      case VkResult::VK_ERROR_OUT_OF_DATE_KHR:
         return std::formatter<std::string>::format("VK_ERROR_OUT_OF_DATE_KHR", ctx);
         break;
      case VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
         return std::formatter<std::string>::format("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR", ctx);
         break;
      case VkResult::VK_ERROR_VALIDATION_FAILED_EXT:
         return std::formatter<std::string>::format("VK_ERROR_VALIDATION_FAILED_EXT", ctx);
         break;
      case VkResult::VK_ERROR_INVALID_SHADER_NV:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_SHADER_NV", ctx);
         break;
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
      case VkResult::VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR", ctx);
         break;
#endif
      case VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
         return std::formatter<std::string>::format("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT", ctx);
         break;
      case VkResult::VK_ERROR_NOT_PERMITTED_KHR:
         return std::formatter<std::string>::format("VK_ERROR_NOT_PERMITTED_KHR", ctx);
         break;
      case VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
         return std::formatter<std::string>::format("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT", ctx);
         break;
      case VkResult::VK_THREAD_IDLE_KHR:
         return std::formatter<std::string>::format("VK_THREAD_IDLE_KHR", ctx);
         break;
      case VkResult::VK_THREAD_DONE_KHR:
         return std::formatter<std::string>::format("VK_THREAD_DONE_KHR", ctx);
         break;
      case VkResult::VK_OPERATION_DEFERRED_KHR:
         return std::formatter<std::string>::format("VK_OPERATION_DEFERRED_KHR", ctx);
         break;
      case VkResult::VK_OPERATION_NOT_DEFERRED_KHR:
         return std::formatter<std::string>::format("VK_OPERATION_NOT_DEFERRED_KHR", ctx);
         break;
      case VkResult::VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
         return std::formatter<std::string>::format("VK_ERROR_COMPRESSION_EXHAUSTED_EXT", ctx);
         break;
      case VkResult::VK_RESULT_MAX_ENUM:
         return std::formatter<std::string>::format("VK_RESULT_MAX_ENUM", ctx);
         break;
      }

      return std::formatter<std::string>::format("UNDEFINED", ctx);
   }
};

#endif