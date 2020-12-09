#ifndef RW_UI_ENTITY_HPP
#define RW_UI_ENTITY_HPP

#include "RedWolf/core.hpp"
#include "RedWolf/data/Event.hpp"
#include "RedWolf/gl/Sprite.hpp"
#include "RedWolf/gl/Texture.hpp"

namespace rw::ui
{
   /** \brief Base class for all visual objects. */
   class RW_API Entity
   {
   public:
      /** \brief Default constructor. */
      Entity() = default;

      /** \brief Create an entity with a specific ID. */
      explicit Entity(size_t id);

      /** \brief Destructor. */
      virtual ~Entity() = default;

      /** 
         \brief Handle incoming events. 
         \param evnt Event to be handled.
         \return Result of the event handling.
      */
      virtual rw::data::Event handleEvent(const rw::data::Event& evnt) = 0;

      /** \brief Get the entity's ID. */
      [[nodiscard]] size_t id() const;

      /** \brief Base rendering method for the entity. */
      virtual void render() = 0;

   private:
      const size_t id_{ 0 }; /**< Numerical ID. */
   };
} // namespace rw::ui

#endif