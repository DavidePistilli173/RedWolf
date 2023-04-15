#include "Vertex.hpp"

using namespace rw::lif::vlk;

constexpr Vertex::Vertex(const glm::vec3& pPos, const glm::vec4& pColour) : pos{ pPos }, colour{ pColour } {}

constexpr VkVertexInputBindingDescription Vertex::bindingDescription()
{
   VkVertexInputBindingDescription result{};

   result.binding = 0U;
   result.stride = sizeof(Vertex);
   result.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

   return result;
}

constexpr VkVertexInputAttributeDescription Vertex::colourAttributeDescription()
{
   VkVertexInputAttributeDescription result{};

   result.binding = 0U;
   result.location = 1U;
   result.format = VK_FORMAT_R32G32B32A32_SFLOAT;
   result.offset = offsetof(Vertex, colour);

   return result;
}

constexpr VkVertexInputAttributeDescription Vertex::positionAttributeDescription()
{
   VkVertexInputAttributeDescription result{};

   result.binding = 0U;
   result.location = 0U;
   result.format = VK_FORMAT_R32G32B32_SFLOAT;
   result.offset = offsetof(Vertex, pos);

   return result;
}