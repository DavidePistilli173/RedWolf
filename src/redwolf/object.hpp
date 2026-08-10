#pragma once

namespace rw {
    /**
     * @brief Base class for most engine objects.
     */
    class Object {
     public:
        Object()          = default;
        virtual ~Object() = default;

        Object(const Object&)            = default;
        Object& operator=(const Object&) = default;

        Object(Object&&)            = default;
        Object& operator=(Object&&) = default;

     private:
    };
} // namespace rw