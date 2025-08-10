//
// Created by cjm036653 on 10/08/25.
//

#ifndef APP_HPP
#define APP_HPP

#include <memory>

namespace rw::core {
    /**
     * @brief Base class for a RedWolf applications.
     */
    class App {
     public:
        /**
         * @brief Default constructor.
         */
        App() = default;

        /**
         * @brief Destructor.
         */
        virtual ~App() = default;

        /**
         * @brief Copy-constructor.
         */
        App(const App&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        App& operator=(const App&) = delete;

        /**
         * @brief Move-constructor.
         */
        App(App&&) = default;

        /**
         * @brief Move-assignment operator.
         */
        App& operator=(App&&) = default;

        void run();

     private:
    };

    /**
     * @brief Factory function to create RedWolf application instances.
     * @details This is required to be defined by the user application.
     * @return New RedWolf application instance.
     */
    [[nodiscard]] std::unique_ptr<App> create_app();

} // namespace rw::core

#endif // APP_HPP
