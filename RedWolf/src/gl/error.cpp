#include "RedWolf/gl/error.hpp"

#include "RedWolf/Log.hpp"

namespace rw::gl
{
    std::array<rw::FixedQueue<Msg, Messages::queue_size>, static_cast<int>(ErrLvl::tot)> Messages::msgs_;

    /**
        \param id Message ID.
        \param level Message severity.
    */
    void Messages::add(GLuint id, ErrLvl level)
    {
        msgs_[static_cast<int>(level)].push({ id, level });
    }

    /**
        \param level of the messages to be returned.
        \return All messages with the specified severity since the last checkOut.
    */
    rw::FixedQueue<Msg, Messages::queue_size> Messages::checkOut(ErrLvl level)
    {
        return std::move(msgs_[static_cast<int>(level)]);
    }

    /**
        \param source Source of the message.
        \param type Type of error message.
        \param id Message identifier.
        \param severity Severity of the error.
        \param length Length of the message.
        \param message Actual message.
        \param userParam Optional user data. Unused.
    */
    void RW_API GLAPIENTRY openGLError(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam
    )
    {
        static long msgId{ 1 }; // Custom ID for messages.

        /* Set the error source. */
        std::string errSrc; // Error source.
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            errSrc = "OPENGL API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            errSrc = "WINDOW SYSTEM API";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            errSrc = "SHADER COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            errSrc = "THIRD PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            errSrc = "CURRENT APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            errSrc = "OTHER";
            break;
        default:
            errSrc = "NO SOURCE";
            break;
        }

        /* Set the error type. */
        unsigned int severityValue{ 0 }; // Current level of severity.
        std::string errType; // Error type.
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            errType = "API ERROR";
            severityValue += static_cast<int>(ErrVal::warn);
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            errType = "DEPRECATED BEHAVIOUR";
            severityValue += static_cast<int>(ErrVal::info);
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            errType = "UNDEFINED BEHAVIOUR";
            severityValue += static_cast<int>(ErrVal::warn);
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            errType = "PORTABILITY ISSUE";
            severityValue += static_cast<int>(ErrVal::warn);
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            errType = "PERFORMANCE ISSUE";
            severityValue += static_cast<int>(ErrVal::warn);
            break;
        case GL_DEBUG_TYPE_MARKER:
            errType = "STREAM ANNOTATION";
            severityValue += static_cast<int>(ErrVal::info);
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            errType = "GROUP PUSHING";
            severityValue += static_cast<int>(ErrVal::info);
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            errType = "GROUP POPPING";
            severityValue += static_cast<int>(ErrVal::info);
            break;
        case GL_DEBUG_TYPE_OTHER:
            errType = "OTHER";
            severityValue += static_cast<int>(ErrVal::info);
            break;
        default:
            errType = "NO TYPE";
            break;
        }

        /* Set the error severity. */
        std::string errSev; // Error severity.
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            errSev = "HIGH";
            severityValue += static_cast<int>(ErrVal::error);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            errSev = "MEDIUM";
            severityValue += static_cast<int>(ErrVal::warn);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            errSev = "LOW";
            severityValue += static_cast<int>(ErrVal::info);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            errSev = "NOTIFICATION";
            severityValue = 0;
            break;
        default:
            errSev = "NO SEVERITY";
            severityValue = 0;
            break;
        }

        /* Set the appropriate level error based on the accumulated severity score. */
        Msg msg; //Message data.
        msg.id = msgId++; // Set and increase the message ID.
        if (severityValue > static_cast<int>(ErrThresholds::info)) msg.lvl = ErrLvl::info;
        else if (severityValue > static_cast<int>(ErrThresholds::warn)) msg.lvl = ErrLvl::warn;
        else if (severityValue > static_cast<int>(ErrThresholds::error)) msg.lvl = ErrLvl::error;

        /* Log the error and add it to memory. */
        switch (msg.lvl)
        {
        case ErrLvl::info:
            Log::gl_info(
                "Message #{0}:\n        Source: {1}\n        Type: {2}\n        Severity: {3}\n        Message: {4}",
                msg.id,
                errSrc.c_str(),
                errType.c_str(),
                errSev.c_str(),
                message
            );
            break;
        case ErrLvl::warn:
            Log::gl_warn(
                "Message #{0}:\n        Source: {1}\n        Type: {2}\n        Severity: {3}\n        Message: {4}",
                msg.id,
                errSrc.c_str(),
                errType.c_str(),
                errSev.c_str(),
                message
            );
            break;
        case ErrLvl::error:
            Log::gl_err(
                "Message #{0}:\n        Source: {1}\n        Type: {2}\n        Severity: {3}\n        Message: {4}",
                msg.id,
                errSrc.c_str(),
                errType.c_str(),
                errSev.c_str(),
                message
            );
            break;
        case ErrLvl::notification:
            [[fallthrough]];
        case ErrLvl::tot:
            break;
        }
    }
}