module;

#include <cstdint>

export module redwolf.core.handle;

export namespace rw {
    /**
     * @brief Generic handle for RedWolf resources, cheap enough to be copied around.
     * @details This handle does not own the resource it points to, and is only valid as long as the resource exists.
     * @tparam T Type of resource contained in the handle.
     */
    template<typename T>
    struct Handle {
        static constexpr uint64_t invalid_uid{ 0U }; /**< Constant representing an invalid resource ID. */

        /**
         * @brief Check if two handles are equal.
         * @param other Handle to compare against.
         * @return True if the handles point to the same resource, false otherwise.
         */
        [[nodiscard]] bool operator==(const Handle<T>& other) const noexcept {
            return index == other.index;
        }

        /**
         * @brief Check if two handles are different.
         * @param other Handle to compare against.
         * @return True if the handles DO NOT point to the same resource, false otherwise.
         */
        [[nodiscard]] bool operator!=(const Handle<T>& other) const noexcept {
            return index != other.index;
        }

        /**
         * @brief Check if the handle is invalid (i.e., does not point to a resource).
         * @return True if the handle is invalid, false otherwise.
         */
        [[nodiscard]] bool invalid() const noexcept {
            return invalid_uid == index;
        }

        /**
         * @brief Check if the handle is valid (i.e., points to a resource).
         * @return True if the handle is valid, false otherwise.
         */
        [[nodiscard]] bool valid() const noexcept {
            return invalid_uid != index;
        }

        uint64_t index{ invalid_uid }; /**< Unique identifier for the resource. */
    };
} // namespace rw
