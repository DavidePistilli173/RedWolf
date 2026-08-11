#pragma once

#include "redwolf/memory/memory.hpp"
#include "redwolf/object.hpp"

#include <functional>

namespace rw {
    /**
     * @brief Base event class for events.
     */
    class EventBase {
     public:
        EventBase() = default;
        virtual ~EventBase();

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

     private:
    };
} // namespace rw