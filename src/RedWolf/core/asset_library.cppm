module;

#include <cstdint>
#include <memory>
#include <vector>

export module redwolf.core.asset_library;

import redwolf.common;

export namespace rw::core {
    /**
     * @brief Class for managing a collection of assets.
     * @tparam T Type of asset to manage.
     */
    template<typename T>
    class AssetLibrary {
     public:
        /**
         * @brief Constructor.
         */
        AssetLibrary() {
            assets_.emplace_back(); // Reserve ID 0 as invalid.
        }

        /**
         * @brief Create a new asset with its arguments.
         * @tparam Args Types of arguments to pass to the asset's constructor.
         * @param args Parameters for the asset's constructor.
         * @return Non-owning handle to the newly-constructed asset.
         */
        template<typename... Args>
        [[nodiscard]] rw::Handle<T> create(Args&&... args) {
            const auto id{ static_cast<Id>(assets_.size()) };
            auto&      new_asset{ assets_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)) };
            rw::Handle result{ .id = id, .ptr = new_asset.get() };
            return result;
        }

        /**
         * @brief Get the specified asset.
         * @param id ID of the asset to get.
         * @return Non-owning handle to the asset with the specified ID, if it exists. nullptr otherwise.
         */
        [[nodiscard]] rw::Handle<T> get(const uint64_t id) {
            if (id >= assets_.size()) {
                return {};
            }
            return rw::Handle{ .id = id, .ptr = assets_[id].get() };
        }

     private:
        std::vector<std::unique_ptr<T>> assets_; /**< Actual asset collection. */
    };
} // namespace rw::core
