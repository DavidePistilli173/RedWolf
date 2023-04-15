#ifndef RW_LIF_VULKAN_VERTEX_HPP
#define RW_LIF_VULKAN_VERTEX_HPP

#include "RedWolf/common.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace rw::lif::vlk
{
   /**
    * @brief Vertex data.
    * @details This structure uses: 3D positions, 4D colours.
    */
   struct RW_API Vertex
   {
      /**
       * @brief Default constructor.
       */
      constexpr Vertex() = default;

      /**
       * @brief Constructor.
       * @param pPos Position data.
       * @param pColour Colour data.
       */
      constexpr Vertex(const glm::vec3& pPos, const glm::vec4& pColour);

      /**
       * @brief Get the binding description of the vertex.
       * @return Binding description of the vertex.
       */
      [[nodiscard]] static constexpr VkVertexInputBindingDescription bindingDescription();

      /**
       * @brief Get the attribute description of the vertex colour data.
       * @return Attribute description of the vertex colour data.
       */
      [[nodiscard]] static constexpr VkVertexInputAttributeDescription colourAttributeDescription();

      /**
       * @brief Get the attribute description of the vertex position data.
       * @return Attribute description of the vertex position data.
       */
      [[nodiscard]] static constexpr VkVertexInputAttributeDescription positionAttributeDescription();

      glm::vec3 pos;    /**< Position data. */
      glm::vec4 colour; /**< Colour data. */
   };
} // namespace rw::lif::vlk

#endif