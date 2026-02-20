module;

#include <vector>

export module redwolf.phy.physics_engine;

import redwolf.core.asset_library;
import redwolf.core.geometry;
import redwolf.engine.entity;

export namespace rw::phy {
    /**
     * @brief Class responsible for managing the physics simulation.
     */
    class PhysicsEngine {
     public:
        PhysicsEngine() = default;

        /**
         * @brief Update the physics simulation.
         * @param delta_time Time elapsed since the last update, in seconds.
         * @param entities List of all entities. Only the ones with physics descriptors will be updated.
         */
        void update(float delta_time, core::AssetLibrary<engine::Entity>& entities) {
            std::vector<core::Line> collision_lines;
            collision_lines.reserve(entities.size());

            entities.visit_all([&collision_lines](engine::Entity& entity) {
                if (!entity.physics_desc.has_value()) {
                    return;
                }
                auto& desc{ entity.physics_desc.value() };

                if (desc.is_static) {
                }
            });
        }
    };
} // namespace rw::phy
