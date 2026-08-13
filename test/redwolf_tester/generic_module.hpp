#pragma once

#include <redwolf/module.hpp>

namespace rwt {
    /**
     * @brief Module for generic testing.
     */
    class GenericModule : public rw::Module {
     public:
        GenericModule()           = default;
        ~GenericModule() override = default;

        GenericModule(const GenericModule&)            = delete;
        GenericModule& operator=(const GenericModule&) = delete;

        GenericModule(GenericModule&&)            = delete;
        GenericModule& operator=(GenericModule&&) = delete;

        [[nodiscard]] bool             init() override;
        [[nodiscard]] std::string_view name() override;
        [[nodiscard]] bool             on_update(f32 delta_time) override;
        [[nodiscard]] bool             on_render(f32 delta_time) override;

     private:
    };
} // namespace rwt