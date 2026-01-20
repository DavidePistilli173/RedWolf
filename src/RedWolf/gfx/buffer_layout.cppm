module;

#include <cstdint>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

export module redwolf.gfx.buffer_layout;

import redwolf.gfx.shader_data;

export namespace rw::gfx {
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
        BufferLayoutElement(const ShaderDataType p_type, const std::string_view p_name, const bool p_normalized = false) :
            type{ p_type }, name{ p_name }, normalized{ p_normalized }, size{ shader_data_type_size(p_type) } {}

        /**
         * @brief Get the number of components of the element.
         * @return Number of components of the element.
         */
        [[nodiscard]] uint32_t component_count() const {
            return shader_data_element_count(type);
        }

        ShaderDataType type{ ShaderDataType::none }; /**< Data type of the element. */
        std::string    name;                         /**< Readable name of the element. */
        bool           normalized{ false };          /**< Whether the element is normalized. */
        uint64_t       offset{ 0U };                 /**< Offset from the start of the buffer. */
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
        BufferLayout(const std::initializer_list<BufferLayoutElement>& elements) : elements_(elements) {
            uint32_t offset{ 0U };
            for (auto& element : elements_) {
                element.offset = offset;
                offset += element.size;
            }
            stride_ = offset; // Set the stride to the total size of the elements
        }

        /**
         * @brief Get the begin iterator.
         * @return Begin iterator.
         */
        [[nodiscard]] iterator begin() {
            return elements_.begin();
        }

        /**
         * @brief Get the begin iterator (const).
         * @return Const begin iterator.
         */
        [[nodiscard]] const_iterator begin() const {
            return elements_.begin();
        }

        /**
         * @brief Get the begin iterator (const).
         * @return Const begin iterator.
         */
        [[nodiscard]] const_iterator cbegin() const {
            return elements_.cbegin();
        }

        /**
         * @brief Get the end iterator (const).
         * @return Const end iterator.
         */
        [[nodiscard]] const_iterator cend() const {
            return elements_.cend();
        }

        /**
         * @brief Get the actual layout elements.
         * @return Layout elements.
         */
        [[nodiscard]] const std::vector<BufferLayoutElement>& elements() const {
            return elements_;
        }

        /**
         * @brief Get the end iterator.
         * @return End iterator.
         */
        [[nodiscard]] iterator end() {
            return elements_.end();
        }

        /**
         * @brief Get the end iterator (const).
         * @return Const end iterator.
         */
        [[nodiscard]] const_iterator end() const {
            return elements_.end();
        }

        /**
         * @brief Get the stride of the buffer layout.
         * @return Stride in bytes.
         */
        [[nodiscard]] uint32_t stride() const {
            return stride_;
        }

     private:
        std::vector<BufferLayoutElement> elements_;     /**< Elements in the buffer layout. */
        uint32_t                         stride_{ 0U }; /**< Stride of the buffer layout in bytes. */
    };
} // namespace rw::gfx
