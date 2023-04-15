#include "Mesh.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::gfx;

Mesh::Mesh(RedWolfManager& manager, const Renderer& renderer, u32 id, size_t vertexNum) :
   id_{ id }, stagingVertexBuffer_{ manager,
                                    renderer.graphicsDevice(),
                                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                    VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
                                    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
                                    vertexNum },
   actualVertexBuffer_{ manager,
                        renderer.graphicsDevice(),
                        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
                        0,
                        vertexNum },
   logger_{ manager.logger() }
{
   if (!stagingVertexBuffer_.map())
   {
      logger_.relFatal("Failed to map staging buffer.");
   }
}

void Mesh::draw(rw::lif::vlk::CommandBuffer& cmdBuffer)
{
   cmdBuffer.bindVertexBuffer(actualVertexBuffer_.buffer());
   cmdBuffer.draw(actualVertexBuffer_.size(), 1U, 0U, 0U);
}

u32 Mesh::id() const
{
   return id_;
}

void Mesh::sync(rw::lif::vlk::CommandBuffer& cmdBuffer)
{
   actualVertexBuffer_.copyFrom(cmdBuffer, stagingVertexBuffer_);
}