//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_BUFFER_LAYOUT_HPP
#define SRC_REDWOLF_BUFFER_LAYOUT_HPP

#include "shader_data.hpp"

#include <cstdint>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

namespace rw::gfx {
    /**
     * @brief Describes a single element in a buffer layout.
     */
    struct BufferLayoutElement {
        /**
         * @brief Default constructor.
         */
        BufferLayoutElement() = default;

        /**
         * @brief Constructor.
         * @param p_name Readable name of the element.
         * @param p_type Data type of the element.
         * @param p_normalized Whether the element is normalized.
         */
        BufferLayoutElement(const ShaderDataType p_type, const std::string_view p_name, const bool p_normalized = false);

        /**
         * @brief Get the number of components of the element.
         * @return Number of components of the element.
         */
        [[nodiscard]] uint32_t component_count() const;

        ShaderDataType type{ ShaderDataType::none }; /**< Data type of the element. */
        std::string    name;                         /**< Readable name of the element. */
        bool           normalized{ false };          /**< Whether the element is normalized. */
        uint32_t       offset{ 0U };                 /**< Offset from the start of the buffer. */
        uint32_t       size{ 0U };                   /**< Size of the element in bytes. */
    };

    /**
     * @brief Data layout descriptor for vertex buffers.
     */
    class BufferLayout {
     public:
        /**
         * @brief Iterator type for the layer stack.
         */
        using iterator = std::vector<BufferLayoutElement>::iterator;

        /**
         * @brief Const iterator type for the layer stack.
         */
        using const_iterator = std::vector<BufferLayoutElement>::const_iterator;

        /**
         * @brief Default constructor.
         */
        BufferLayout() = default;

        /**
         * @brief Constructor.
         * @param elements Layout elements.
         */
        BufferLayout(const std::initializer_list<BufferLayoutElement>& elements);

        /**
         * @brief Get the begin iterator.
         * @return Begin iterator.
         */
        [[nodiscard]] iterator begin();

        /**
         * @brief Get the begin iterator (const).
         * @return Const begin iterator.
         */
        [[nodiscard]] const_iterator begin() const;

        /**
         * @brief Get the begin iterator (const).
         * @return Const begin iterator.
         */
        [[nodiscard]] const_iterator cbegin() const;

        /**
         * @brief Get the end iterator (const).
         * @return Const end iterator.
         */
        [[nodiscard]] const_iterator cend() const;

        /**
         * @brief Get the actual layout elements.
         * @return Layout elements.
         */
        [[nodiscard]] const std::vector<BufferLayoutElement>& elements() const;

        /**
         * @brief Get the end iterator.
         * @return End iterator.
         */
        [[nodiscard]] iterator end();

        /**
         * @brief Get the end iterator (const).
         * @return Const end iterator.
         */
        [[nodiscard]] const_iterator end() const;

        /**
         * @brief Get the stride of the buffer layout.
         * @return Stride in bytes.
         */
        [[nodiscard]] uint32_t stride() const;

     private:
        std::vector<BufferLayoutElement> elements_;     /**< Elements in the buffer layout. */
        uint32_t                         stride_{ 0U }; /**< Stride of the buffer layout in bytes. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_BUFFER_LAYOUT_HPP
