module;

#include <optional>

export module redwolf.engine.entity;

import redwolf.core.math;
import redwolf.gfx.quad;
import redwolf.phy.physics_descriptor;

export namespace rw::engine {
    /**
     * @brief Data for an application object that can be managed by the engine.
     */
    struct Entity {
        rw::core::Transform                   transform;    /**< Entity transform. */
        std::optional<rw::gfx::Quad>          quad;         /**< Optional quad for rendering the entity. */
        std::optional<phy::PhysicsDescriptor> physics_desc; /**< Optional physics descriptor for simulating the entity. */
    };
} // namespace rw::engine
