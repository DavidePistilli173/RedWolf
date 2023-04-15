#ifndef RW_LIF_VULKAN_SHADERMODULE_HPP
#define RW_LIF_VULKAN_SHADERMODULE_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"

#include <string_view>

namespace rw
{
   class RedWolfManager;
} // namespace rw

namespace rw::lif::vlk
{
   class DeviceBase;
} // namespace rw::lif::vlk

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a Vulkan VkShaderModule.
    */
   class RW_API ShaderModule : public BaseObject
   {
   public:
      static constexpr std::string_view entry_point_name{ "main" }; /**< Name of the shader's entry point. */

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param device Logical device that will use the shader.
       * @param file Path to the compiled shader file.
       */
      ShaderModule(RedWolfManager& manager, const DeviceBase& device, std::string_view file);

      /**
       * @brief Destructor.
       */
      ~ShaderModule() override;

      /**
       * @brief Copy constructor.
       */
      ShaderModule(const ShaderModule&) = delete;

      /**
       * @brief Move constructor.
       */
      ShaderModule(ShaderModule&& other) noexcept;

      /**
       * @brief Copy-assignment operator.
       */
      ShaderModule& operator=(const ShaderModule&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      ShaderModule& operator=(ShaderModule&&) = delete;

      /**
       * @brief Get the raw shader handle.
       * @return Raw shader handle.
       */
      [[nodiscard]] VkShaderModule handle() const;

   private:
      VkShaderModule shader_{ VK_NULL_HANDLE }; /**< Raw shader handle. */

      const DeviceBase& device_; /**< Logical device that will use the shader. */
   };
} // namespace rw::lif::vlk

#endif