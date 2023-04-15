#ifndef RW_GFX_MESH_HPP
#define RW_GFX_MESH_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/gfx/Renderer.hpp"
#include "RedWolf/lif/vulkan/Buffer.hpp"
#include "RedWolf/lif/vulkan/CommandBuffer.hpp"
#include "RedWolf/lif/vulkan/Vertex.hpp"

namespace rw
{
   class RedWolfManager;
}

namespace rw::gfx
{
   /**
    * @brief Polygonal mesh.
    */
   class RW_API Mesh
   {
   public:
      /**
       * @brief Constructor.
       * @param renderer Renderer that will draw the mesh.
       * @param id Numerical ID of the mesh.
       * @param vertexNum Number of vertices of the mesh.
       */
      Mesh(RedWolfManager& manager, const Renderer& renderer, u32 id, size_t vertexNum = 4U);

      /**
       * @brief Draw the mesh.
       * @param cmdBuffer Command buffer that will record the draw commands.
       */
      void draw(rw::lif::vlk::CommandBuffer& cmdBuffer);

      /**
       * @brief Get the ID of the mesh.
       * @return ID of the mesh.
       */
      [[nodiscard]] u32 id() const;

      /**
       * @brief Set the vertex data of the mesh.
       * @param data Vertex data to set.
       */
      template<rw::cont::IsContiguousContainerOf<rw::lif::vlk::Vertex> ListT>
      void setVertexData(const ListT& data)
      {
         stagingVertexBuffer_.setData(data);
      }

      /**
       * @brief Synchronise data between CPU and GPU.
       * @param cmdBuffer Buffer to use for the copy.
       */
      void sync(rw::lif::vlk::CommandBuffer& cmdBuffer);

   private:
      const u32                                  id_{ 0U };            /**< ID of the mesh. */
      rw::lif::vlk::Buffer<rw::lif::vlk::Vertex> stagingVertexBuffer_; /**< Temporary vertex buffer for storing local changes. */
      rw::lif::vlk::Buffer<rw::lif::vlk::Vertex> actualVertexBuffer_;  /**< Actual vertex buffer used by the GPU for rendering. */

      rw::utils::Logger& logger_; /**< Library logger. */
   };
} // namespace rw::gfx

#endif