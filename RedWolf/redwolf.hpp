/** @file redwolf.hpp File that includes all library headers. Use this only when you are REALLY lazy. */

#ifndef RW_REDWOLF_HPP
#define RW_REDWOLF_HPP

#include "RedWolfManager.hpp"
#include "common.hpp"

// alg
#include "RedWolf/alg/search.hpp"
#include "RedWolf/alg/sort.hpp"

// cont
#include "RedWolf/cont/ConstexprMap.hpp"
#include "RedWolf/cont/concepts.hpp"

// core
#include "RedWolf/core/BaseApplication.hpp"
#include "RedWolf/core/BaseGUIApplication.hpp"
#include "RedWolf/core/BaseObject.hpp"

// dat
#include "RedWolf/dat/KVPair.hpp"
#include "RedWolf/dat/LogMsg.hpp"
#include "RedWolf/dat/MVPair.hpp"
#include "RedWolf/dat/SettingsNode.hpp"
#include "RedWolf/dat/VersionInfo.hpp"

// events
#include "RedWolf/events/BaseEvent.hpp"
#include "RedWolf/events/ButtonPressEvent.hpp"
#include "RedWolf/events/DataReadyEvent.hpp"
#include "RedWolf/events/EventHandler.hpp"
#include "RedWolf/events/TimeoutEvent.hpp"
#include "RedWolf/events/WindowCloseEvent.hpp"
#include "RedWolf/events/aliases.hpp"
#include "RedWolf/events/concepts.hpp"

// io
#include "RedWolf/io/File.hpp"
#include "RedWolf/io/concepts.hpp"
#include "RedWolf/io/custom_formatters.hpp"

// lif
#include "RedWolf/lif/glfw/GlfwManager.hpp"
#include "RedWolf/lif/glfw/Window.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"
#include "RedWolf/lif/vulkan/DebugUtilsMessenger.hpp"
#include "RedWolf/lif/vulkan/DeviceBase.hpp"
#include "RedWolf/lif/vulkan/FunctionPointers.hpp"
#include "RedWolf/lif/vulkan/GraphicsDevice.hpp"
#include "RedWolf/lif/vulkan/GraphicsPipeline.hpp"
#include "RedWolf/lif/vulkan/ImageView.hpp"
#include "RedWolf/lif/vulkan/Instance.hpp"
#include "RedWolf/lif/vulkan/InstanceBase.hpp"
#include "RedWolf/lif/vulkan/Interface.hpp"
#include "RedWolf/lif/vulkan/PhysicalDevice.hpp"
#include "RedWolf/lif/vulkan/PipelineLayout.hpp"
#include "RedWolf/lif/vulkan/QueueFamilies.hpp"
#include "RedWolf/lif/vulkan/RenderPass.hpp"
#include "RedWolf/lif/vulkan/ShaderModule.hpp"
#include "RedWolf/lif/vulkan/Surface.hpp"
#include "RedWolf/lif/vulkan/SwapChain.hpp"
#include "RedWolf/lif/vulkan/concepts.hpp"

// net
#include "RedWolf/net/BaseSocket.hpp"
#include "RedWolf/net/UdpSocket.hpp"

// text
#include "RedWolf/text/string_utils.hpp"

// thread
#include "RedWolf/thread/ThreadPool.hpp"

// time
#include "RedWolf/time/DateTime.hpp"
#include "RedWolf/time/Timer.hpp"
#include "RedWolf/time/concepts.hpp"
#include "RedWolf/time/constants.hpp"

// ui
#include "RedWolf/ui/BaseWindow.hpp"

// utils
#include "RedWolf/utils/Logger.hpp"
#include "RedWolf/utils/Settings.hpp"

#endif