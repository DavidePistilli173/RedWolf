module;

#include <cstdint>
#include <memory>
#include <unordered_map>

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
        AssetLibrary() = default;

        /**
         * @brief Create a new asset with the given ID and arguments.
         * @details If an asset with the given ID already exists, it is replaced.
         * @tparam Args Types of arguments to pass to the asset's constructor.
         * @param id ID of the asset.
         * @param args Parameters for the asset's constructor.
         * @return Non-owning pointer to the newly-constructed asset.
         */
        template<typename... Args>
        [[nodiscard]] rw::Handle<T> create(const uint64_t id, Args&&... args) {
            auto       new_asset{ std::make_unique<T>(std::forward<Args>(args)...) };
            rw::Handle result{ .id = id, .ptr = new_asset.get() };
            assets_[id] = std::move(new_asset);
            return result;
        }

        /**
         * @brief Get the specified asset.
         * @param id ID of the asset to get.
         * @return Non-owning pointer to the asset with the specified ID, if it exists. nullptr otherwise.
         */
        [[nodiscard]] rw::Handle<T> get(const uint64_t id) {
            const auto it{ assets_.find(id) };
            if (assets_.end() == it) {
                return {};
            }
            return rw::Handle{ .id = id, .ptr = it->second.get() };
        }

     private:
        std::unordered_map<uint64_t, std::unique_ptr<T>> assets_; /**< Actual asset collection. */
    };
} // namespace rw::core
