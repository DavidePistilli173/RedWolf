//
// Created by david on 11/09/2025.
//

#ifndef SRC_REDWOLF_SHADER_LIBRARY_HPP
#define SRC_REDWOLF_SHADER_LIBRARY_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace rw::core {
    /**
     * @brief Concept for types that have a constructor taking a single std::string parameter (e.g. file path).
     */
    template<typename T>
    concept HasFileConstructor = requires(T a, std::string b) {
        { T(b) } -> std::same_as<T>;
    };

    /**
     * @brief Class for managing a collection of assets.
     */
    template<HasFileConstructor T>
    class AssetLibrary {
     public:
        /**
         * @brief Constructor.
         */
        AssetLibrary() = default;

        /**
         * @brief Create a new asset with the given ID and arguments.
         * @tparam Args Types of arguments to pass to the asset's constructor.
         * @param id ID of the asset.
         * @param args Parameters for the asset's constructor.
         * @return Non-owning pointer to the newly-constructed asset.
         */
        template<typename... Args>
        [[nodiscard]] T* create(const uint64_t id, Args&&... args) {
            auto  new_asset{ std::unique_ptr<T>(new T(std::forward<Args>(args)...)) };
            auto* result{ new_asset.get() };
            assets_[id] = std::move(new_asset);
            return result;
        }

        /**
         * @brief Get the specified asset.
         * @param id ID of the asset to get.
         * @return Asset with the specified ID, if it exists. nullptr otherwise.
         */
        [[nodiscard]] T* get(const uint64_t id) {
            const auto it{ assets_.find(id) };
            if (assets_.end() == it) {
                return nullptr;
            }
            return it->second.get();
        }

     private:
        std::unordered_map<uint64_t, std::unique_ptr<T>> assets_; /**< Actual asset collection. */
    };
} // namespace rw::core

#endif // SRC_REDWOLF_SHADER_LIBRARY_HPP
