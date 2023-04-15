#include "BaseEntity.hpp"

#include "RedWolf/RedWolfManager.hpp"
#include "RedWolf/gfx/Renderer.hpp"

using namespace rw::gfx;

BaseEntity::BaseEntity(RedWolfManager& manager, Renderer& renderer) : rw::core::BaseObject(manager), renderer_{ renderer }
{
   renderer_.registerEntity(this);
}

BaseEntity::~BaseEntity()
{
   renderer_.unregisterEntity(this);
}

void BaseEntity::addMesh(Mesh& mesh)
{
   std::scoped_lock lck_{ mtx_ };
   meshes_.emplace(mesh.id(), mesh);
}

void BaseEntity::draw(rw::lif::vlk::CommandBuffer& commandBuffer, size_t pipelineId)
{
   std::scoped_lock lck_{ mtx_ };

   for (auto& mesh : meshes_)
   {
      mesh.second.draw(commandBuffer);
   }

   userDraw_(commandBuffer, pipelineId);
}

void BaseEntity::removeMesh(u32 id)
{
   std::scoped_lock lck_{ mtx_ };
   meshes_.erase(id);
}

void BaseEntity::sync(rw::lif::vlk::CommandBuffer& cmdBuffer)
{
   std::scoped_lock lck_{ mtx_ };
   for (auto& mesh : meshes_)
   {
      mesh.second.sync(cmdBuffer);
   }
}