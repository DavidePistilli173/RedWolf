#ifndef RW_EVT_EVENTHANDLER_HPP
#define RW_EVT_EVENTHANDLER_HPP

#include "RedWolf/alg/search.hpp"
#include "RedWolf/alg/sort.hpp"
#include "RedWolf/common.hpp"
#include "RedWolf/core/Object.hpp"
#include "RedWolf/evt/IEventHandler.hpp"
#include "RedWolf/evt/aliases.hpp"
#include "RedWolf/thread/ThreadPool.hpp"
#include "RedWolf/util/Logger.hpp"

#include <map>
#include <shared_mutex>
#include <vector>

namespace rw::evt
{
   /**
    * @brief Class for handling event subscribing, unsubscribing and dispatching.
    */
   class RW_API EventHandler : public IEventHandler
   {
   public:
      /**
       * @brief Default constructor.
       * @param logger Logger to use.
       */
      explicit EventHandler(rw::util::Logger& logger);

      /**
       * @brief Add a new object to the event system.
       * @param object Object to add to the event system.
       */
      void addObject(rw::core::Object* object);

      /**
       * @brief Generate an event and propagate it to all subscribers using a dedicated thread.
       * @tparam Event Event type to generate.
       * @param generator Object that generated the event.
       * @param evnt Event data to send.
       * @param policy Policy for the event generation.
       */
      void generateEvent(
         rw::core::IObject* generator, const Event& evt, GenerationPolicy policy = GenerationPolicy::asynchronous) const override;

      /**
       * @brief Remove an object from the event system.
       * @param object Object to remove from the event system.
       */
      void removeObject(rw::core::BaseObject* object);

      /**
       * @brief Subscribe an object to an event generated by another object.
       * @tparam Event Event type that the object wants to receive.
       * @param generator Object that will generate the event.
       * @param suscriber Object that wants to subscribe to the event.
       * @return true on success, false otherwise.
       */
      template<rw::events::HasEventID Event>
      bool subscribe(rw::core::BaseObject* generator, rw::core::BaseObject* subscriber)
      {
         std::scoped_lock lck_{ mtx_ };
         if (!objects_.contains(generator))
         {
            logger_.warn("Pointer {} does not refer to a valid RedWolf object.", generator);
            return false;
         }

         if (rw::alg::binarySearch(objects_[generator][Event::event_id], subscriber) == objects_[generator][Event::event_id].cend())
         {
            objects_[generator][Event::event_id].emplace_back(subscriber);
            rw::alg::sortLastElement(objects_[generator][Event::event_id].begin(), objects_[generator][Event::event_id].end());
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
         auto  subscriberIterator = rw::alg::binarySearch(eventMap, subscriber);
         if (subscriberIterator != eventMap.cend())
         {
            eventMap.erase(subscriberIterator);
         }
      }

   private:
      rw::util::Logger&      logger_;     /**< RedWolf library logger. */
      rw::thread::ThreadPool threadPool_; /**< RedWolf library thread pool. */

      std::shared_mutex mtx_; /**< Mutex for protecting write access to the subscribers map. */

      /**
       * @brief Map of all RedWolf objects with all their subscribers for each event ID.
       * @details This is a map containing all RedWolf objects.
       *          For each object, there is a map of all subscribers, divided by event ID.
       *          For each event ID, there is a vector of all objects subscribed to that event.
       */
      std::unordered_map<rw::core::Object*, std::unordered_map<EventID, std::vector<rw::core::Object*>>> objects_;
   };
} // namespace rw::evt

#endif