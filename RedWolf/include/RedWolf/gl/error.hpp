/** 
    \file error.hpp 
    \brief OpenGL message logging. 
*/

#ifndef RW_GL_ERROR_HPP
#define RW_GL_ERROR_HPP

#include "RedWolf/core.hpp"
#include "RedWolf/utility.hpp"

#include <glad/glad.h>

#include <array>
#include <string>
#include <vector>

namespace rw::gl
{
    /** \brief Levels of severity for an OpenGL message. */
    enum class ErrLvl
    {
        notification, /**< Notifications, can be safely ignored. */
        info, /**< Simple debug information.*/
        warn, /**< Warning.*/
        error, /**< Serious error. */
        tot /**< Total number of levels. */
    };
    /** \brief Message weights. */
    enum class ErrVal
    {
        info = 10, /**< Information messages.*/
        warn = 25, /**< Warning messages. */
        error = 50 /**< Error messages. */
    };
    /** \brief Weight thresholds used to set the message error level. */
    enum class ErrThresholds
    {
        info = 10, /**< Information messages. */
        warn = 40, /**< Warning messages. */
        error = 60 /**< Error messages. */
    };

    /** \brief Single OpenGL message. */
    struct Msg
    {
        /** \brief Default constructor. */
        constexpr Msg() = default;
        /** 
            \brief Construct a custom Msg. 
            \param nid Message ID.
            \param nlvl Message severity level.
        */
        constexpr Msg(GLuint nid, ErrLvl nlvl) :
            id{ nid },
            lvl{ nlvl }
        {}
        
        GLuint id{ 0 }; /**< Message ID. */
        ErrLvl lvl{ ErrLvl::notification }; /**< Message severity. */
    };

    /** \brief Log and store OpenGL messages. */
    class RW_API Messages
    {
    public:
        /** \brief Maximum size of a single message queue. */
        static constexpr size_t queue_size{ 1024 };
        /** \brief Add an error to the list. */
        static void add(GLuint id, ErrLvl level);
        
        /** \brief Get all messages with a specific level. Clears the log for the specific level. */
        static rw::FixedQueue<Msg, Messages::queue_size> checkOut(ErrLvl level);

    private:
        /** \brief Different message queues, one for each severity level. */
        static std::array<rw::FixedQueue<Msg, queue_size>, static_cast<int>(ErrLvl::tot)> msgs_;
    };

    /**
        \brief Callback for OpenGL message handling.
        Only active in debug builds (see core.hpp).
    */
    void RW_API GLAPIENTRY openGLError(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam
    );
}

#endif