module;

export module redwolf.gfx.context;

import redwolf.ui.common;

export namespace rw::gfx {
    /**
     * @brief OpenGL graphics context.
     */
    class Context {
     public:
        /**
         * @brief Constructor.
         * @param window Raw handle of the window to render to.
         */
        explicit Context(rw::ui::WindowHandle window);

        /**
         * @brief Destructor.
         */
        ~Context() = default;

        /**
         * @brief Copy constructor.
         */
        Context(const Context&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        Context& operator=(const Context&) = delete;

        /**
         * @brief Move constructor.
         */
        Context(Context&&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        Context& operator=(Context&&) = delete;

        /**
         * @brief Initialise the context.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init();

        /**
         * @brief Swap frame buffers.
         */
        void swap_buffers();

     private:
        rw::ui::WindowHandle window_{ rw::ui::invalid_window_handle }; /**< Raw handle of the window to render to. */
    };
} // namespace rw::gfx
