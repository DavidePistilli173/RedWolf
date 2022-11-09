#ifndef RW_LIF_VULKAN_SHADERMODULE_HPP
#define RW_LIF_VULKAN_SHADERMODULE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"

#include <string_view>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   class GraphicsDevice;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkShaderModule.
    */
   class RW_API ShaderModule : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Graphics device that will use the shader.
       * @param file Path to the compiled shader file.
       */
      ShaderModule(RedWolfManager& manager, GraphicsDevice& device, std::string_view file);

      /**
       * @brief Destructor.
       */
      ~ShaderModule();

      /**
       * @brief Copy constructor.
       */
      ShaderModule(const ShaderModule&) = delete;

      /**
       * @brief Move constructor.
       */
      ShaderModule(ShaderModule&& other);

      /**
       * @brief Copy-assignment operator.
       */
      ShaderModule& operator=(const ShaderModule&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      ShaderModule& operator=(ShaderModule&&) = delete;

   private:
      VkShaderModule shader_{ VK_NULL_HANDLE }; /**< Raw shader handle. */

      VkDevice device_{ VK_NULL_HANDLE }; /**< Raw handle to the device that will use the shader. */
   };
} // namespace rw::lif::vlk

#endif