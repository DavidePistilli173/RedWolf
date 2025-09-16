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

        /**
         * @brief Load an asset from file and store it.
         * @param id Id of the asset. If the ID already exists, the asset is replaced.
         * @param file_path Path to the asset source file.
         * @return Pointer to the newly created asset.
         */
        [[nodiscard]] T* load(const uint64_t id, const std::string& file_path) {
            auto  new_shader{ std::unique_ptr<T>(new T(file_path)) };
            auto* result{ new_shader.get() };
            assets_[id] = std::move(new_shader);
            return result;
        }

     private:
        std::unordered_map<uint64_t, std::unique_ptr<T>> assets_; /**< Actual asset collection. */
    };
} // namespace rw::core

#endif // SRC_REDWOLF_SHADER_LIBRARY_HPP
