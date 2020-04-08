#ifndef REDWOLFTESTS_UTILITY_HPP
#define REDWOLFTESTS_UTILITY_HPP

#include <glad/glad.h>
#include <RedWolf/utility.hpp>
#include <SDL2.0.9/SDL.h>

#include <cstdio>
#include <memory>
#include <string_view>

namespace rwt
{
    /********** TYPE ALIASES **********/
    using window_t = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    /********** ENUMS **********/
    /* OpenGL error severities. */
    enum class ErrorSeverity
    {
        NOTIFICATION,
        LOW,
        MEDIUM,
        HIGH
    };

    /********** CONSTANTS **********/
    constexpr rw::Size<int> win_size{ 640, 480 };

    /********** GLOBAL VARIABLES **********/
    bool g_glError{ false }; // True if an OpenGL error occurs.

    /********** FUNCTIONS **********/
    /* Print all OpenGL errors. */
    void GLAPIENTRY openGLError(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        /* Keep track of the number of the current error. */
        static int errorNum = 0;
        ++errorNum;

        const ErrorSeverity MIN_SEVERITY = ErrorSeverity::LOW;  // Desired minimum error level to be printed.
        ErrorSeverity currentSeverity = ErrorSeverity::NOTIFICATION; // Severity of the current error.

        /* Set the error strings. */
        std::string errorType, errorSeverity;

        /* Set the error type string. */
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            errorType = "API ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            errorType = "DEPRECATED BEHAVIOUR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            errorType = "UNDEFINED BEHAVIOUR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            errorType = "PORTABILITY ISSUE";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            errorType = "PERFORMANCE ISSUE";
            break;
        case GL_DEBUG_TYPE_MARKER:
            errorType = "STREAM ANNOTATION";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            errorType = "GROUP PUSHING";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            errorType = "GROUP POPPING";
            break;
        case GL_DEBUG_TYPE_OTHER:
            errorType = "OTHER";
            break;
        }

        /* Set the error severity string. */
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            errorSeverity = "HIGH";
            currentSeverity = ErrorSeverity::HIGH;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            errorSeverity = "MEDIUM";
            currentSeverity = ErrorSeverity::MEDIUM;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            errorSeverity = "LOW";
            currentSeverity = ErrorSeverity::LOW;
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            errorSeverity = "NOTIFICATION";
            currentSeverity = ErrorSeverity::NOTIFICATION;
            break;
        }

        /* If the error is severe enough print it and flag the error. */
        if (currentSeverity >= MIN_SEVERITY)
        {
            g_glError = true;
            std::printf("[OPENGL #%d]\n   type = %s,\n   severity = %s,\n   message = %s\n\n",
                errorNum,
                errorType.c_str(),
                errorSeverity.c_str(),
                message);
        }
    }

    /* Set up a basic OpenGL window and context. */
    window_t setUpGLContext(std::string_view winName)
    {
        /* Set OpenGL version. */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        /* Set core profile for OpenGL. */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        
        /* Create the window. */
        window_t win{ window_t(
            SDL_CreateWindow(winName.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win_size.w, win_size.h, SDL_WINDOW_OPENGL),
            [](SDL_Window* win) {SDL_DestroyWindow(win); }
        ) };
        if (win == nullptr)
        {
            std::printf("Could not create game window.\n");
            std::printf("%s", SDL_GetError());
            return win;
        }

        SDL_GLContext context{ SDL_GL_CreateContext(win.get()) };
        gladLoadGLLoader(SDL_GL_GetProcAddress);

        /* Set debug context. */
        #ifdef _DEBUG
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openGLError, nullptr);
        #endif

        return win;
    }
}

#endif