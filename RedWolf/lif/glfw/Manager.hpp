#ifndef RW_LIF_GLFW_MANAGER_HPP
#define RW_LIF_GLFW_MANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/util/Logger.hpp"

namespace rw::lif::glfw
{
   /**
    * @brief GLFW library manager.
    */
   class RW_API Manager
   {
   public:
      /**
       * @brief Constructor.
       * @param logger Logger.
       */
      explicit Manager(rw::util::Logger& logger);

      /**
       * @brief Destructor.
       */
      ~Manager();

      /**
       * @brief Copy constructor.
       */
      Manager(const Manager&) = delete;

      /**
       * @brief Move constructor.
       */
      Manager(Manager&&) = delete;

      /**
       * @brief Copy-assignment operator.
       */
      Manager& operator=(const Manager&) = delete;

      /**
       * @brief Move-assignment operator.
       */
      Manager& operator=(Manager&&) = delete;

   private:
      rw::util::Logger& logger_; /**< Logger. */
   };
} // namespace rw::lif::glfw

#endif