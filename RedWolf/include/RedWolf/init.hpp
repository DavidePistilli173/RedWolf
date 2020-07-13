#ifndef RW_INIT_HPP
#define RW_INIT_HPP

#include "RedWolf/core.hpp"

namespace rw
{
   /**
       \brief Handler for application-wide subsystems like SDL.
   */
   class RW_API SubsystemsHandler
   {
   public:
      /**
          \brief Initialise all subsystems.
          \detail Throws an std::exception on error.
      */
      SubsystemsHandler();

      /** \brief Quit all subsystems. */
      ~SubsystemsHandler();

      /** \brief This object cannot be copied. */
      SubsystemsHandler(const SubsystemsHandler&) = delete;
      /** \brief This object cannot be moved. */
      SubsystemsHandler(SubsystemsHandler&&) = delete;

      /** \brief This object cannot be copied. */
      SubsystemsHandler& operator=(const SubsystemsHandler&) = delete;
      /** \brief This object cannot be moved. */
      SubsystemsHandler& operator=(SubsystemsHandler&&) = delete;

   private:
      static bool initialised_; /**< true if the subsystems are already initialised. false otherwise. */
   };
} // namespace rw

#endif