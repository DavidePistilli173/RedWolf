#ifndef REDWOLF_UTILITY_HPP
#define REDWOLF_UTILITY_HPP

namespace rw
{
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
}

#endif