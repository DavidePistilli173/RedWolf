#ifndef RW_EVENTS_EVENTHANDLER_HPP
#define RW_EVENTS_EVENTHANDLER_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/events/aliases.hpp"
#include "RedWolf/events/concepts.hpp"
#include "RedWolf/thread/ThreadPool.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <map>
#include <set>
#include <shared_mutex>

namespace rw
{
   class RedWolfManager;
}

namespace rw::core
{
   class BaseObject;
}

namespace rw::events
{
   /**
    * @brief Class for handling event subscribing, unsubscribing and dispatching.
    */
   class RW_API EventHandler
   {
   public:
      /**
       * @brief Default constructor.
       * @param manager RedWolf library manager.
       */
      explicit EventHandler(RedWolfManager& manager);

      /**
       * @brief Add a new object to the event system.
       * @param object Object to add to the event system.
       */
      void addObject(rw::core::BaseObject* object);

      /**
       * @brief Generate an event and propagate it to all subscribers using a dedicated thread.
       * @tparam Event Event type to generate.
       * @param generator Object that generated the event.
       * @param evnt Event data to send.
       */
      template<rw::events::HasEventID Event>
      void generateEvent(rw::core::BaseObject* generator, const Event& evnt)
      {
         threadPool_.addTask(
            [this, generator, evnt]()
            {
               std::shared_lock lck_{ mtx_ };

               if (!objects_.contains(generator))
               {
                  logger_.warn("Event generator {} already destroyed.", reinterpret_cast<void*>(generator));
                  return;
               }

               for (auto& subscriber : objects_[generator][Event::event_id])
               {
                  subscriber->handle(evnt, generator);
               }
            });
      }

      /**
       * @brief Remove an object from the event system.
       * @param object Object to remove from the event system.
       */
      void removeObject(rw::core::BaseObject* object);

      /**
       * @brief Subscribe an object to an event generated by this object.
       * @tparam Event Event type that the object wants to receive.
       * @param generator Object that will generate the events.
       * @param suscriber Object that wants to subscribe to the event.
       * @return true on success, false otherwise.
       */
      template<rw::events::HasEventID Event>
      bool subscribe(rw::core::BaseObject* generator, rw::core::BaseObject* subscriber)
      {
         std::scoped_lock lck_{ mtx_ };
         if (!objects_.contains(generator))
         {
            logger_.warn("Pointer {} does not refer to a valid RedWolf object.", reinterpret_cast<void*>(generator));
            return false;
         }

         if (!objects_[generator][Event::event_id].contains(subscriber))
         {
            objects_[generator][Event::event_id].insert(subscriber);
         }

         return true;
      }

      /**
       * @brief Unsubscribe a previously subscribed object from a specific event.
       * @tparam Event Event type that the object wants to unsubscribe from.
       * @param generator Object that will generate the events.
       * @param suscriber Object that wants to unsubscribe from the event.
       */
      template<rw::events::HasEventID Event>
      void unsubscribe(rw::core::BaseObject* generator, rw::core::BaseObject* subscriber)
      {
         std::scoped_lock lck_{ mtx_ };
         if (!objects_.contains(generator))
         {
            return;
         }

         auto& eventMap = objects_[generator][Event::event_id];
         auto  subscriberIterator = eventMap.find(subscriber);
         if (subscriberIterator != eventMap.end())
         {
            eventMap.erase(subscriber);
         }
      }

   private:
      rw::utils::Logger&      logger_;     /**< RedWolf library logger. */
      rw::thread::ThreadPool& threadPool_; /**< RedWolf library thread pool. */

      std::shared_mutex mtx_; /**< Mutex for protecting write access to the subscribers map. */

      /**
       * @brief Map of all RedWolf objects with all their subscribers for each event ID.
       */
      std::map<rw::core::BaseObject*, std::map<EventID, std::set<rw::core::BaseObject*>>> objects_;
   };
} // namespace rw::events

#endif