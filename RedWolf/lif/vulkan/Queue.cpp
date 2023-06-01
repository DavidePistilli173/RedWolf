#include "Queue.hpp"

using namespace rw::lif::vulkan;

Queue::Queue(VkQueue queue) : queue_{ queue } {}

VkQueue Queue::handle() const
{
   return queue_;
}

bool Queue::isOperationSupported(OpType operationType) const
{
   return std::find(supportedOperations_.begin(), supportedOperations_.end(), operationType) != supportedOperations_.end();
}

const std::vector<Queue::OpType>& Queue::supportedOperations() const
{
   return supportedOperations_;
}