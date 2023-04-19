#ifndef RW_REDWOLFMANAGER_HPP
#define RW_REDWOLFMANAGER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/core/Object.hpp"
#include "RedWolf/core/concepts.hpp"

#include <memory>
#include <mutex>
#include <unordered_map>

namespace rw
{
   /**
    * @brief RedWolf library manager.
    */
   class RW_API RedWolfManager
   {
   public:
      /**
       * @brief Constructor.
       */
      explicit RedWolfManager() = default;

      /**
       * @brief Create a new RedWolf object.
       * @tparam ...Args Types of arguments that will be passed to the object's constructor.
       * @tparam T Type of object to create.
       * @param ...args Arguments passed to the object's constructor.
       * @return Non-owning pointer to the object.
       */
      template<rw::core::IsDerivedFrom<rw::core::Object> T, typename... Args>
      [[nodiscard]] T* createObject(Args&&... args);

      /**
       * @brief Destroy a RedWolf object.
       * @param obj Pointer to the object to destroy.
       */
      void destroyObject(rw::core::Object* obj);

   private:
      std::mutex mtx_; /**< Mutex for protecting private variables. */

      std::unordered_map<rw::core::ObjectID, std::unique_ptr<rw::core::Object>> objects_; /**< All library-managed objects. */
   };
} // namespace rw

// IMPLEMENTATION
using namespace rw;

template<rw::core::IsDerivedFrom<rw::core::Object> T, typename... Args>
[[nodiscard]] T* RedWolfManager::createObject(Args&&... args)
{
   auto obj = std::make_unique<T>(std::forward<Args>(args)...);
   auto result = obj.get();
   objects_.emplace(result->id(), std::move(obj));
   return result;
}

#endif