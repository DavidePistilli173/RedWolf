#ifndef RW_APP_HPP
#define RW_APP_HPP

#include "RedWolf/core.hpp"

#include <memory>

namespace rw
{
    /** \brief Base application class for all RedWolf programs. */
    class RW_API App
    {
    public:
        /** \brief Default constructor. */
        App() = default;
        /** \brief Default destructor. */
        virtual ~App() = default;
        /** \brief Applications cannot be copied. */
        App(const App&) = delete;
        /** \brief Default move constructor. */
        App(App&& app) = default;

        /** \brief Applications cannot be copied. */
        App& operator=(const App&) = delete;
        /** \brief Default move assignment operator. */
        App& operator=(App&& app) = default;

        /** \brief Run the application. */
        void run();
    };

    /**
        \brief To be defined in the user program. 
        \return Pointer to the application instance.
    */
    std::unique_ptr<App> createApp();
}

#endif