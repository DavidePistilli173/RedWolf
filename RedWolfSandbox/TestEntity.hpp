#ifndef RWSANDBOX_TESTENTITY_HPP
#define RWSANDBOX_TESTENTITY_HPP

#include <RedWolf/gfx/BaseEntity.hpp>
#include <RedWolf/gfx/Renderer.hpp>
#include <array>

class TestEntity : public rw::gfx::BaseEntity
{
public:
   TestEntity(rw::RedWolfManager& rw, rw::gfx::Renderer& renderer);

protected:
   /**
    * @brief User draw method.
    * @details The operations performed in this function must be protected by some kind of thread synchronisation mechanism, because the
    * rendering thread is different from the main thread.
    * @param pipelineId ID of the pipeline currently being drawn.
    */
   void userDraw_(rw::lif::vlk::CommandBuffer& commandBuffer, size_t pipelineId) override;

   /**
    * @brief User handle function for incoming events. To be implemented in derived classes.
    * @details The operations performed in this function must be protected by some kind of thread synchronisation mechanism, because
    * events could be generated from different threads (eg. rw::time::Timer).
    * @param evnt Event data.
    * @param sender Sender of the event.
    */
   void userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) override;

private:
   std::array<rw::lif::vlk::Vertex, 3U> vertexData_; /**< Test vertex data. */
   rw::gfx::Mesh                        mesh_;       /**< Test mesh. */
};

#endif