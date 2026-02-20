//
// Created by david on 20/02/2026.
//

export module redwolf.phy.physics_descriptor;

import redwolf.core.math;

export namespace rw::phy {
    /**
     * @brief Descriptor for configuring physics properties of an entity.
     */
    struct PhysicsDescriptor {
        bool       is_static{ true }; /**< Whether the entity is movable through the physics simulation or only through manual commands. */
        core::Vec2 velocity{ 0.0F, 0.0F }; /**< Velocity of the entity. Ignored if is_static is true. */
    };
} // namespace rw::phy
