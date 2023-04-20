#ifndef RW_CORE_BASEOBJECT_HPP
#define RW_CORE_BASEOBJECT_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/evt/Event.hpp"

namespace rw::core
{
   /**
    * @brief Base object for all objects managed by RedWolf.
    */
   class RW_API BaseObject
   {
   public:
      /**
       * @brief Constructor.
       */
      BaseObject() = default;

      /**
       * @brief Destructor.
       */
      virtual ~BaseObject() = default;

      /**
       * @brief Copy constructor.
       */
      BaseObject(const BaseObject&) = default;

      /**
       * @brief Move constructor.
       */
      BaseObject(BaseObject&&) = default;

      /**
       * @brief Copy-assignment operator.
       */
      BaseObject& operator=(const BaseObject&) = default;

      /**
       * @brief Move-assignment operator.
       */
      BaseObject& operator=(BaseObject&&) = default;

      /**
       * @brief Handle incoming events.
       * @details Do not override this function. To add user behaviour, override the userHandle_ function.
       * @param generator Event generator.
       * @param evt Event data.
       */
      void handle(const BaseObject* generator, const rw::evt::Event& evt);

   protected:
      /**
       * @brief Function to override in child classes to add custom event handling.
       * @param generator Event generator. Will always be nullptr.
       * @param evt Event data.
       */
      virtual void userHandle_(const BaseObject* generator, const rw::evt::Event& evt) = 0;
   };
} // namespace rw::core

#endif