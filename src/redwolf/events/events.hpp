#pragma once

#include "connection.hpp"
#include "event.hpp"
#include "redwolf/containers/hash_map.hpp"
#include "redwolf/memory/memory_pool.hpp"

namespace rw {
    /**
     * @brief Event system.
     */
    class Events {
     public:
        using TypeId = const void*;

        template<typename PayloadT>
        [[nodiscard]] TypeId type_id() {
            static char marker{};
            return &marker;
        }

        /**
         * @brief Initialise the event system.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Shutdown the event system.
         */
        static void shutdown();

        /**
         * @brief Fire a specific event.
         * @tparam PayloadT Type of payload that the event carries.
         * @param payload Payload to transmit with the event.
         */
        template<typename PayloadT>
        static void fire(const PayloadT& payload) {
            auto* g_events{ instance_() };

            const TypeId id{ g_events->type_id<PayloadT>() };
            auto         it{ g_events->events_.find(id) };

            // The event does not exist: there are surely no subscribers, no point in firing it.
            if (it == g_events->events_.end()) {
                return;
            }

            // Already existing event.
            static_ptr_cast<Event<PayloadT>>(it->value)->fire(payload);
        }

        /**
         * @brief Subscribe to an event.
         * @tparam PayloadT Type of payload the event carries.
         * @param callback Callback to call when the event gets fired.
         * @return RAII connection object.
         */
        template<typename PayloadT>
        [[nodiscard]] static Connection<PayloadT> subscribe(Event<PayloadT>::Callback callback) {
            auto* g_events{ instance_() };

            const TypeId id{ g_events->type_id<PayloadT>() };
            auto         it{ g_events->events_.find(id) };

            // New event.
            if (it == g_events->events_.end()) {
                auto& event{ g_events->events_.emplace(id, Memory::new_object<Event<PayloadT>>()) };
                return Connection<PayloadT>(event.subscribe(callback), event);
            }

            // Already existing event.
            return Connection<PayloadT>(static_ptr_cast<Event<PayloadT>>(it->value)->subscribe(callback), it->value);
        }

     private:
        Events() = default;

        /**
         * @brief Get the event manager instance.
         */
        [[nodiscard]] static Events* instance_();

        HashMap<TypeId, Ptr<EventBase>> events_{ MemoryType::engine }; /**< All enabled events. */
    };
} // namespace rw