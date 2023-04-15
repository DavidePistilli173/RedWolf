#include "TestEntity.hpp"

TestEntity::TestEntity(rw::RedWolfManager& rw, rw::gfx::Renderer& renderer) : BaseEntity(rw, renderer), mesh_{ rw, renderer, 1U, 3U }
{
   vertexData_[0].pos = { 0.0F, -0.5F, 0.0F };
   vertexData_[1].pos = { 0.5F, 0.5F, 0.0F };
   vertexData_[2].pos = { -0.5F, 0.5F, 0.0F };

   vertexData_[0].colour = { 1.0F, 0.0F, 0.0F, 1.0F };
   vertexData_[1].colour = { 0.0F, 1.0F, 0.0F, 1.0F };
   vertexData_[2].colour = { 0.0F, 0.0F, 1.0F, 1.0F };

   mesh_.setVertexData(vertexData_);

   addMesh(mesh_);
}

void TestEntity::userDraw_(rw::lif::vlk::CommandBuffer& commandBuffer, size_t pipelineId) {}

void TestEntity::userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) {}
