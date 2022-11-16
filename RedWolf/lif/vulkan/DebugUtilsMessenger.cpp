#include "DebugUtilsMessenger.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::lif::vlk;

DebugUtilsMessenger::DebugUtilsMessenger(RedWolfManager& manager) : BaseObject(manager) {}

DebugUtilsMessenger::~DebugUtilsMessenger()
{
   if (messenger_ != VK_NULL_HANDLE)
   {
      vulkanInterface_.destroyDebugUtilsMessenger(vulkanInstance_.handle(), messenger_);
   }
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessenger::debugCallback(
   VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
   VkDebugUtilsMessageTypeFlagsEXT             messageType,
   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
   void*                                       pUserData)
{
   auto* logger{ reinterpret_cast<rw::utils::Logger*>(pUserData) };

   std::string type;
   switch (messageType)
   {
   case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
      type = "GENERAL";
      break;
   case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
      type = "SPECIFICATION";
      break;
   case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
      type = "PERFORMANCE";
      break;
   default:
      type = "UNDEFINED";
      break;
   }

   switch (messageSeverity)
   {
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      logger->trace("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      logger->info("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      logger->warn("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      logger->err("|{}| {}", type, pCallbackData->pMessage);
      break;
   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
      logger->fatal("|{}| {}", type, pCallbackData->pMessage);
      break;
   }

   return VK_FALSE;
}

bool DebugUtilsMessenger::initialise(const VkDebugUtilsMessengerCreateInfoEXT& args)
{
   const std::scoped_lock lck{ debugCallbackMutex_ };

   if (messenger_ != VK_NULL_HANDLE)
   {
      logger_.warn("Vulkan debug messenger already initialised.");
      return false;
   }

   auto messenger = vulkanInterface_.createDebugUtilsMessenger(vulkanInstance_.handle(), args);
   if (!messenger.has_value()) return false;

   messenger_ = messenger.value();
   return true;
}
