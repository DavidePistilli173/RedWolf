module;

#include <array>
#include <chrono>
#include <memory>
#include <ranges>

export module redwolf.engine.engine;

import redwolf.core.asset_library;
import redwolf.core.common;
import redwolf.core.handle;
import redwolf.engine.camera_controller;
import redwolf.engine.entity;
import redwolf.evt.application_event;
import redwolf.evt.event;
import redwolf.gfx.renderer_2_d;
import redwolf.ui.window;
import redwolf.util.logger;

export namespace rw::engine {
    class Engine;

    template<typename T>
    concept UserAppType = requires(T t, float delta_time, const rw::evt::Event& event) {
        //{ T(Engine * engine) } -> std::same_as<T>;
        { t.update(delta_time) } -> std::same_as<void>;
        { t.on_event(event) } -> std::same_as<void>;
    };

    /**
     * @brief Main engine class responsible for initializing and running the application.
     * @tparam UserApp Type of application to run.
     */
    template<UserAppType UserApp>
    class Engine {
     public:
        /**
         * @brief Construct a new Engine object.
         * @param window_data Data for creating the application window.
         * @param argc Argument count from the command line.
         * @param argv Argument vector from the command line.
         */
        Engine(const rw::ui::WindowDescriptor& window_data, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
            // Initialise the logger as first instruction.
            rw::info("Constructing application");

            // Create the main application window.
            window_ = std::make_unique<rw::ui::Window>(window_data);
            window_->set_event_callback([this](const rw::evt::Event& event) { return on_event_(event); });

            // Instantiate the user-defined application class.
            user_app_ = std::make_unique<UserApp>(this);
        }

        ~Engine() = default;

        Engine(const Engine&)            = delete;
        Engine& operator=(const Engine&) = delete;

        Engine(Engine&&)            = delete;
        Engine& operator=(Engine&&) = delete;

        /**
         * @brief Move an entity by a specified translation vector.
         * @param entity_handle Handle of the entity to move.
         * @param translation Translation vector to apply to the entity's position.
         */
        void move_entity(const Handle<Entity> entity_handle, const rw::core::Vec3 translation) {
            entities_.visit<void>(entity_handle, [&translation](Entity& entity) { entity.transform.position += translation; });
        }

        /**
         * @brief Constructor.
         * @param width Width of the viewport.
         * @param height Height of the viewport.
         * @param movement_enabled If true, enables movement input control. Otherwise, the camera position will be fixed.
         * @param rotation_enabled If true, enables rotation input control. Otherwise, the camera orientation will be fixed.
         * @param zoom_enabled If true, enables zoom input control. Otherwise, the camera zoom level will be fixed.
         */
        [[nodiscard]] std::shared_ptr<CameraController> new_camera_controller(
            const uint32_t width,
            const uint32_t height,
            const bool     movement_enabled,
            const bool     rotation_enabled,
            const bool     zoom_enabled) {
            return camera_controllers_.emplace_back(
                std::make_shared<CameraController>(width, height, movement_enabled, rotation_enabled, zoom_enabled));
        }

        /**
         * @brief Create a new entity empty in the engine.
         * @return Handle to the newly created entity.
         */
        [[nodiscard]] Handle<Entity> new_entity() {
            return entities_.create().second;
        }

        /**
         * @brief Create a new entity with initial data.
         * @param entity Initial data for the entity.
         * @return Handle to the newly created entity.
         */
        [[nodiscard]] Handle<Entity> new_entity(const Entity& entity) {
            auto [new_entity, entity_handle] = entities_.create();
            new_entity                       = entity;
            return entity_handle;
        }

        /**
         * @brief Run the main loop of the engine with the specified application type.
         */
        void run() {
            auto last_ts{ std::chrono::high_resolution_clock::now() };

            // Main application loop.
            while (running_) {
                auto       current_ts{ std::chrono::high_resolution_clock::now() };
                const auto delta_time{ static_cast<float>((current_ts - last_ts).count()) * nanoseconds_to_seconds };

                user_app_.update(delta_time);
                window_->update();

                last_ts = current_ts;
            }
        }

     private:
        /**
         * @brief Event handler for the application.
         */
        [[nodiscard]] bool on_event_(const rw::evt::Event& event) {
            bool result{ false };

            switch (event.type()) {
            case rw::evt::EventType::window_resize:
                on_window_resize_(dynamic_cast<const rw::evt::WindowResizedEvent&>(event));
                result = false;
                break;
            case rw::evt::EventType::window_close:
                running_ = false;
                result   = true;
                break;
            default:
                break;
            }

            user_app_.on_event(event);

            return result;
        }

        /**
         * @brief Handle a window resized event.
         * @param event Event to handle.
         */
        void on_window_resize_(const rw::evt::WindowResizedEvent& event) {
            if (0 == event.width && 0 == event.height) {
                minimized_ = true;
                return;
            }
            minimized_ = false;

            window_->renderer()->set_viewport(0, 0, event.width, event.height);
        }

        std::unique_ptr<UserApp> user_app_; /**< User application. */

        std::unique_ptr<rw::ui::Window> window_;             /**< Application window. */
        bool                            running_{ true };    /**< Flag to indicate if the application is running. */
        bool                            minimized_{ false }; /**< If true, the window is minimised. */

        core::AssetLibrary<Entity>                     entities_;           /**< All entities in the application. */
        std::vector<std::shared_ptr<CameraController>> camera_controllers_; /**< List of all camera controllers in the application. */
    };
} // namespace rw::engine
