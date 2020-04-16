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