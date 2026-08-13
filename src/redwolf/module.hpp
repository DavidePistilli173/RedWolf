#pragma once

#include "redwolf/common.hpp"

#include <string_view>

namespace rw {
    /**
     * @brief Base module class, meant to be inherited by users of RedWolf to build application-specific functionality.
     */
    class Module {
     public:
        Module()          = default;
        virtual ~Module() = default;

        Module(const Module&)            = delete;
        Module& operator=(const Module&) = delete;

        Module(Module&&)            = delete;
        Module& operator=(Module&&) = delete;

        /**
         * @brief Called at startup to initialise the module.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] virtual bool init() = 0;

        /**
         * @brief Get the name of the module.
         */
        [[nodiscard]] virtual std::string_view name() = 0;

        /**
         * @brief Called on each update tick.
         * @param delta_time Time since the last update. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] virtual bool on_update(f32 delta_time) = 0;

        /**
         * @brief Called on each render tick.
         * @param delat_time Time since the last render. [s]
         * @return true on success, false otherwise.
         */
        [[nodiscard]] virtual bool on_render(f32 delta_time) = 0;

     private:
    };
} // namespace rw