#pragma once

#include "redwolf/containers/vec.hpp"
#include "redwolf/memory/memory.hpp"

#include <functional>

namespace rw {
    /**
     * @brief Base event class for events.
     */
    class EventBase {
     public:
        EventBase()          = default;
        virtual ~EventBase() = default;

        EventBase(EventBase&)                  = default;
        EventBase& operator=(const EventBase&) = default;

        EventBase(EventBase&&)            = default;
        EventBase& operator=(EventBase&&) = default;
    };

    /**
     * @brief Actual templated event class.
     */
    template<typename PayloadT>
    class Event : public EventBase {
     public:
        using Callback = std::function<bool(const PayloadT&)>;

        Event() = default;

        /**
         * @brief Fire the event.
         * @param payload Event data.
         */
        void fire(const PayloadT& payload) {
            for (auto& subscriber : subscribers_) {
                if (subscriber.first(payload)) {
                    // Event handled, nothing more to do.
                    return;
                }
            }
        }

        /**
         * @brief Subscriber to this event.
         * @param callback Callback that will be called when the event gets fired.
         * @return Subscriber ID.
         */
        [[nodiscard]] u32 subscribe(Callback callback) {
            const u32 sub_id{ next_subscriber_id_++ };
            (void) subscribers_.emplace_back(callback, sub_id);
            return sub_id;
        }

        /**
         * @brief Unsubscribe from the event.
         * @param id Subscriber ID returned during subscription.
         */
        void unsubscribe(u32 id) {
            subscribers_.erase_first([id](const std::pair<Callback, u32>& item) { return id == item.second; });
        }

     private:
        u32                           next_subscriber_id_{ 1U };              /**< Next subscriber ID to allocate. */
        Vec<std::pair<Callback, u32>> subscribers_{ MemoryCategory::events }; /**< List of all event subscribers. */
    };
} // namespace rw