#pragma once

#include "redwolf/events/event.hpp"

namespace rw {
    /**
     * @brief Object representing a connection between an event and a subscriber.
     */
    template<typename PayloadT>
    class Connection {
     public:
        Connection() = default;
        Connection(u32 id, Ptr<Event<PayloadT>> event) : id_{ id }, event_{ std::move(event) } {}

        ~Connection() {
            if (!event_.is_null()) {
                event_->unsubscribe(id_);
            }
        }

        Connection(const Connection&)            = delete;
        Connection& operator=(const Connection&) = delete;

        /**
         * @brief Move constructor.
         */
        Connection(Connection&& other) noexcept {
            move_(other);
        }

        /**
         * @brief Move-assignment operator.
         */
        Connection& operator=(Connection&& other) noexcept {
            if (this != &other) {
                move_(other);
            }
            return *this;
        }

     private:
        /**
         * @brief Move from another connection object.
         */
        void move_(Connection& other) {
            if (!event_.is_null()) {
                event_->unsubscribe(id_);
            }

            id_       = other.id_;
            other.id_ = 0U;

            event_ = other.event_;
            other.event_.reset();
        }

        u32                  id_{ 0U }; /**< Subscriber ID. */
        Ptr<Event<PayloadT>> event_;    /**< Pointer to the event. */
    };
} // namespace rw