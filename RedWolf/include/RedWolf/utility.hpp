/** 
    \file utility.hpp 
    \brief Engine utilities. 
*/

#ifndef REDWOLF_UTILITY_HPP
#define REDWOLF_UTILITY_HPP

#include <SDL2.0.9/SDL.h>
#include <SDL2_ttf2.0.15/SDL_ttf.h>

#include <memory>
#include <queue>

namespace rw
{
    /********** TYPE ALIASES *********/
    /** \brief File handle. */
    using file_ptr_t = std::unique_ptr<SDL_RWops, void(*)(SDL_RWops*)>;
    /** \brief Text font to be used in textures. */
    using font_ptr_t = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>;
    /** \brief Pointer to an SDL surface. */
    using surf_ptr_t = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;
    /** \brief Pointer to a GUI window. */
    using win_ptr_t = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    /********** STRUCTS *********/
    /**
        \brief Struct containing information about width and height.
    */
    template <typename T>
    struct Size
    {
        /**
            \brief Initialises a struct with default values.
        */
        constexpr Size() = default;
        /**
            \brief Initialises a struct with given values.
            \param a Width.
            \param b Height.
        */
        constexpr Size(T a, T b) :
            w{ a }, h{ b }
        {}

        T w{}; /**< Width. */
        T h{}; /**< Height. */
    };

    /********** CLASSES *********/
    /** \brief Fixed size queue. */
    template <typename T, size_t max_size>
    class FixedQueue : public std::queue<T>
    {
    public:
        /**
            \brief Copy a value into the queue.
            \param value The value to be copied.
        */
        void push(const T& value)
        {
            /* Pop a message if the maximum size was reached. */
            if (size() >= max_size) pop();
            std::queue<T>::push(value);
        }

        /**
            \brief Move a value into the queue.
            \param value The value to be moved.
        */
        void push(T&& value)
        {
            if (size() >= max_size) pop();
            std::queue<T>::push(value);
        }
    };
}

#endif