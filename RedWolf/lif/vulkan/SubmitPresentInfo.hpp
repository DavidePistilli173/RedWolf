#ifndef RW_LIF_VULKAN_SUBMITPRESENTINFO_HPP
#define RW_LIF_VULKAN_SUBMITPRESENTINFO_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/cont/concepts.hpp"
#include "RedWolf/lif/vulkan/BaseObject.hpp"

#include <array>

namespace rw
{
   class RedWolfManager;
}

namespace rw::lif::vlk
{
   /**
    * @brief Wrapper for a VkSubmitInfo and a VkPresentInfo struct.
    * @tparam WaitStageNum Number of stages to wait on.
    * @tparam CmdBuffNum Number of command buffers to submit.
    * @tparam SignalSemNum Number of semaphores to signal the end operations.
    * @tparam SwapChainsNum Number of swap chains to present the images to.
    */
   template<size_t WaitStageNum, size_t CmdBuffNum, size_t SignalSemNum, size_t SwapChainsNum>
   class RW_API SubmitPresentInfo : public BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       */
      SubmitPresentInfo(RedWolfManager& manager) : BaseObject(manager)
      {
         waitStages_.fill(0);
         waitSemaphores_.fill(VK_NULL_HANDLE);
         commandBuffers_.fill(VK_NULL_HANDLE);
         signalSemaphores_.fill(VK_NULL_HANDLE);

         // Initialise the submit information.
         submitInfo_.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
         submitInfo_.waitSemaphoreCount = WaitStageNum;
         submitInfo_.pWaitSemaphores = waitSemaphores_.data();
         submitInfo_.pWaitDstStageMask = waitStages_.data();
         submitInfo_.commandBufferCount = CmdBuffNum;
         submitInfo_.pCommandBuffers = commandBuffers_.data();
         submitInfo_.signalSemaphoreCount = SignalSemNum;
         submitInfo_.pSignalSemaphores = signalSemaphores_.data();

         // Initialise the presentation information.
         presentInfo_.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
         presentInfo_.waitSemaphoreCount = static_cast<u32>(signalSemaphores_.size());
         presentInfo_.pWaitSemaphores = signalSemaphores_.data(); // Wait for image rendering.
         presentInfo_.swapchainCount = static_cast<u32>(swapChains_.size());
         presentInfo_.pSwapchains = swapChains_.data();
      }

      /**
       * @brief Get the raw presentation information.
       * @return Raw presentation information.
       */
      [[nodiscard]] VkPresentInfoKHR& presentInfo()
      {
         return presentInfo_;
      }

      /**
       * @brief Set the command buffers for the submit info.
       * @tparam T Contiguous list of raw command buffer handles.
       * @param cmdBuffers Raw handles to the command buffers to set.
       */
      template<rw::cont::IsContiguousContainerOf<VkCommandBuffer> T>
      void setCommandBuffers(const T& cmdBuffers)
      {
         if (cmdBuffers.size() != commandBuffers_.size())
         {
            logger_.err("Wrong number of command buffers.");
         }

         for (size_t i = 0U; i < cmdBuffers.size() && i < commandBuffers_.size(); ++i)
         {
            commandBuffers_[i] = cmdBuffers[i];
         }
      }

      /**
       * @brief Set the signal semaphores for the submit info.
       * @tparam T Contiguous list of raw semaphore handles.
       * @param signalSems Raw handles to the command buffers to set.
       */
      template<rw::cont::IsContiguousContainerOf<VkSemaphore> T>
      void setSignalSemaphores(const T& signalSems)
      {
         if (signalSems.size() != signalSemaphores_.size())
         {
            logger_.err("Wrong number of signal semaphores.");
         }

         for (size_t i = 0U; i < signalSems.size() && i < signalSemaphores_.size(); ++i)
         {
            signalSemaphores_[i] = signalSems[i];
         }
      }

      template<rw::cont::IsContiguousContainerOf<VkSwapchainKHR> T>
      void setSwapChains(const T& swapChains)
      {
         if (swapChains.size() != swapChains_.size())
         {
            logger_.err("Wrong number of swap chains.");
         }

         for (size_t i = 0U; i < swapChains.size() && i < swapChains_.size(); ++i)
         {
            swapChains_[i] = swapChains[i];
         }
      }

      /**
       * @brief Set the wait semaphores and wait stages for the submit info.
       * @tparam T Contiguous list of raw semaphore handles.
       * @tparam U Contiguous list of pipeline stage flags.
       * @param waitSems Raw wait semaphore handles.
       * @param waitStages Wait stages flags.
       */
      template<rw::cont::IsContiguousContainerOf<VkSemaphore> T, rw::cont::IsContiguousContainerOf<VkPipelineStageFlags> U>
      void setWaitSemaphores(const T& waitSems, const U& waitStages)
      {
         if (waitSems.size() != waitSemaphores_.size() || waitStages.size() != waitStages_.size() || waitSems.size() != waitStages.size())
         {
            logger_.err("Wrong number of wait semaphores and/or wait stages.");
         }

         for (size_t i = 0U; i < waitSems.size() && i < waitStages.size() && i < waitSemaphores_.size() && i < waitStages_.size(); ++i)
         {
            waitStages_[i] = waitStages[i];
            waitSemaphores_[i] = waitSems[i];
         }
      }

      /**
       * @brief Get the raw submit information.
       * @return Raw submit information.
       */
      [[nodiscard]] VkSubmitInfo& submitInfo()
      {
         return submitInfo_;
      }

   private:
      VkSubmitInfo     submitInfo_{};  /**< Queue submit information. */
      VkPresentInfoKHR presentInfo_{}; /**< Queue presentation information. */

      std::array<VkPipelineStageFlags, WaitStageNum> waitStages_;     /**< Stages in which command buffer execution will wait. */
      std::array<VkSemaphore, WaitStageNum>          waitSemaphores_; /**< Semaphores to wait for in each stage of wait_stages. */
      std::array<VkCommandBuffer, CmdBuffNum>        commandBuffers_; /**< Command buffers that will be submitted to the graphics queue. */
      std::array<VkSemaphore, SignalSemNum>     signalSemaphores_; /**< Semaphores to signal when the graphics queue finishes execution. */
      std::array<VkSwapchainKHR, SwapChainsNum> swapChains_;       /**< Swap chains to present the images to. */
   };
} // namespace rw::lif::vlk

#endif