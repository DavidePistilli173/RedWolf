#ifndef RW_GFX_BASEENTITY_HPP
#define RW_GFX_BASEENTITY_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/BaseObject.hpp"
#include "RedWolf/gfx/Mesh.hpp"
#include "RedWolf/lif/vulkan/CommandBuffer.hpp"

#include <map>

namespace rw
{
   class RedWolfManager;
}

namespace rw::gfx
{
   class Renderer;
}

namespace rw::gfx
{
   class RW_API BaseEntity : public rw::core::BaseObject
   {
   public:
      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param renderer Renderer that will render the entity.
       */
      BaseEntity(RedWolfManager& manager, Renderer& renderer);

      /**
       * @brief Destructor.
       */
      ~BaseEntity() override;

      /**
       * @brief Copy constructor.
       */
      BaseEntity(const BaseEntity&) = delete;

      /**
       * @brief Move constructor.
       */
      BaseEntity(BaseEntity&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      BaseEntity& operator=(const BaseEntity&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      BaseEntity& operator=(BaseEntity&&) = delete;

      /**
       * @brief Add a mesh to the entity.
       * @param mesh Mesh to add.
       */
      void addMesh(Mesh& mesh);

      /**
       * @brief Draw the entity on the screen.
       * @details Method called on each frame. This method should not be overrided. To add user behaviour, override userDraw_,
       * @param commandBuffer Command buffer that is currently recording commands.
       * @param pipelineId ID of the pipeline currently being drawn.
       */
      void draw(rw::lif::vlk::CommandBuffer& commandBuffer, size_t pipelineId);

      /**
       * @brief Remove a mesh from the entity.
       * @param id ID of the mesh.
       */
      void removeMesh(u32 id);

      /**
       * @brief Synchronise data between the CPU and the GPU.
       * @details This method is scheduled by the library. It is not meant to be overridden.
       */
      void sync(rw::lif::vlk::CommandBuffer& cmdBuffer);

   protected:
      /**
       * @brief User draw method. To be implemented in derived classes.
       * @details The operations performed in this function must be protected by some kind of thread synchronisation mechanism, because the
       * rendering thread is different from the main thread.
       * @param commandBuffer Command buffer that is currently recording commands.
       * @param pipelineId ID of the pipeline currently being drawn.
       */
      virtual void userDraw_(rw::lif::vlk::CommandBuffer& commandBuffer, size_t pipelineId) = 0;

      /**
       * @brief User handle function for incoming events. To be implemented in derived classes.
       * @details The operations performed in this function must be protected by some kind of thread synchronisation mechanism, because
       * events could be generated from different threads (eg. rw::time::Timer).
       * @param evnt Event data.
       * @param sender Sender of the event.
       */
      virtual void userHandle_(const rw::events::BaseEvent& evnt, const BaseObject* sender) = 0;

      std::mutex mtx_; /**< Mutex used to protect concurrent access to the entity. */

      Renderer& renderer_; /**< Renderer this entity will be rendered by. */

   private:
      std::unordered_map<u32, Mesh&> meshes_; /**< List of meshes linked to this entity, ordered by ID. */
   };
} // namespace rw::gfx

#endif