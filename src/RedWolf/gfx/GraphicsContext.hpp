//
// Created by cjm036653 on 18/08/2025.
//

#ifndef SRC_REDWOLF_CONTEXT_HPP
#define SRC_REDWOLF_CONTEXT_HPP

namespace rw::gfx {
    /**
     * @brief Base class for all graphics contexts.
     */
    class GraphicsContext {
     public:
        /**
         * @brief Constructor.
         */
        GraphicsContext() = default;

        /**
         * @brief Destructor.
         */
        virtual ~GraphicsContext() = default;

        /**
         * @brief Copy constructor.
         */
        GraphicsContext(const GraphicsContext&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        GraphicsContext& operator=(const GraphicsContext&) = delete;

        /**
         * @brief Move constructor.
         */
        GraphicsContext(GraphicsContext&&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        GraphicsContext& operator=(GraphicsContext&&) = delete;

        /**
         * @brief Initialise the context.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] virtual bool init() = 0;

        /**
         * @brief Swap frame buffers.
         */
        virtual void swap_buffers() = 0;

     private:
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_CONTEXT_HPP
