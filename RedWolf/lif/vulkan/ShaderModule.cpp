#include "ShaderModule.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/io/File.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"

using namespace rw::lif::vlk;

ShaderModule::ShaderModule(RedWolfManager& manager, const DeviceBase& device, std::string_view file) :
   BaseObject(manager), device_{ device }
{
   const rw::io::File inputFile{ manager, file };
   if (!inputFile.isOpen())
   {
      logger_.relFatal("Failed to open shader {}.", file);
   }

   std::vector<std::byte> code{ inputFile.readAllByteArray() };

   VkShaderModuleCreateInfo createInfo{};
   createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
   createInfo.codeSize = code.size();
   createInfo.pCode = reinterpret_cast<const u32*>(code.data());

   shader_ = device_.createShaderModule(createInfo);
   if (shader_ == VK_NULL_HANDLE)
   {
      logger_.relFatal("Failed to create shader module.");
   }
}

ShaderModule::~ShaderModule()
{
   device_.destroyShaderModule(shader_);
}

ShaderModule::ShaderModule(ShaderModule&& other) noexcept : BaseObject(other.manager_), device_{ other.device_ }
{
   shader_ = other.shader_;
   other.shader_ = VK_NULL_HANDLE;
}

VkShaderModule ShaderModule::handle() const
{
   return shader_;
}