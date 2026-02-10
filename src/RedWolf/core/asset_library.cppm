module;

#include <cstdint>
#include <memory>
#include <optional>
#include <utility>
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
            assets_.emplace_back(); // Create the invalid asset.
        }

        /**
         * @brief Create a new asset with its arguments.
         * @tparam Args Types of arguments to pass to the asset's constructor.
         * @param args Parameters for the asset's constructor.
         * @return Reference to the newly constructed asset and a non-owning handle to the newly-constructed asset. Note that the reference
         * is only valid until a new asset is created.
         */
        template<typename... Args>
        [[nodiscard]] std::pair<T&, rw::Handle<T>> create(Args&&... args) {
            // If there are free slots, use the last one.
            if (!free_slots_.empty()) {
                const auto index{ static_cast<uint64_t>(free_slots_[free_slots_.size() - 1]) };
                assets_[index].emplace(std::forward<Args>(args)...);
                return std::pair<T&, rw::Handle<T>>(assets_[index].value(), rw::Handle<T>{ .index = index });
            }

            // Otherwise, add a new slot.
            const auto index{ static_cast<uint64_t>(assets_.size()) };
            auto&      new_asset{ assets_.emplace_back(std::forward<Args>(args)...) };
            return std::pair<T&, rw::Handle<T>>(new_asset.value(), rw::Handle<T>{ .index = index });
        }

        /**
         * @brief Specialization for creating an empty but valid asset.
         * @return
         */
        template<>
        [[nodiscard]] std::pair<T&, rw::Handle<T>> create() {
            // If there are free slots, use the last one.
            if (!free_slots_.empty()) {
                const auto index{ static_cast<uint64_t>(free_slots_[free_slots_.size() - 1]) };
                assets_[index].emplace(T{});
                return std::pair<T&, rw::Handle<T>>(assets_[index].value(), rw::Handle<T>{ .index = index });
            }

            // Otherwise, add a new slot.
            const auto index{ static_cast<uint64_t>(assets_.size()) };
            auto&      new_asset{ assets_.emplace_back(T{}) };
            return std::pair<T&, rw::Handle<T>>(new_asset.value(), rw::Handle<T>{ .index = index });
        }

        /**
         * @brief Remove a given asset.
         * @param handle Handle to the asset to remove.
         * @param func Function to call before deleting the asset.
         */
        template<typename FuncT>
        void remove(const rw::Handle<T> handle, FuncT func) {
            if (handle.index >= assets_.size()) {
                return;
            }

            func(assets_[handle.index].value());
            assets_[handle.index].reset();
            free_slots_.emplace_back(handle.index);
        }

        /**
         * @brief Remove all loaded assets.
         * @tparam FuncT Type of function object.
         * @param func Function to call before deleting the asset.
         */
        template<typename FuncT>
        void remove_all(FuncT func) {
            for (auto& asset : assets_) {
                if (asset.has_value()) {
                    func(asset.value());
                }
            }

            assets_.clear();
            free_slots_.clear();
        }

        /**
         * @brief Get a raw reference to an asset.
         * @details Be careful with the lifetime of the retrieved reference.
         * @param handle Handle to the asset to get.
         * @return Reference to the asset, if it is valid.
         */
        [[nodiscard]] std::optional<std::reference_wrapper<T>> unsafe_get_raw(const Handle<T> handle) {
            if (handle.index >= assets_.size()) {
                return {};
            }

            return assets_[handle.index].value();
        }

        /**
         * @brief Visit a specific asset, applying a given function to it and returning its value.
         * @details If the handle is invalid, does nothing and returns the default value of the expected type.
         * @param handle Asset handle.
         * @param func Function to apply to the asset.
         * @return Return value of func, if any.
         */
        template<typename RetVal, typename FuncT>
        RetVal visit(const Handle<T> handle, FuncT func) {
            if (handle.index >= assets_.size()) {
                if constexpr (std::is_void_v<RetVal>) {
                    return;
                } else {
                    return {};
                }
            }

            if (!assets_[handle.index].has_value()) {
                if constexpr (std::is_void_v<RetVal>) {
                    return;
                } else {
                    return {};
                }
            }

            return func(assets_[handle.index].value());
        }

        /**
         * @brief Visit a specific asset, applying a given function to it and returning its value.
         * @details If the handle is invalid, does nothing and returns the default value of the expected type.
         * @param handle Asset handle.
         * @param func Function to apply to the asset.
         * @return Return value of func, if any.
         */
        template<typename RetVal, typename FuncT>
        RetVal visit(const Handle<T> handle, FuncT func) const {
            if (handle.index >= assets_.size()) {
                if constexpr (std::is_void_v<RetVal>) {
                    return;
                } else {
                    return {};
                }
            }

            if (!assets_[handle.index].has_value()) {
                if constexpr (std::is_void_v<RetVal>) {
                    return;
                } else {
                    return {};
                }
            }

            return func(assets_[handle.index].value());
        }

     private:
        std::vector<std::optional<T>> assets_;     /**< Actual asset collection. */
        std::vector<uint64_t>         free_slots_; /**< List of free indices in the assets_ vector. */
    };
} // namespace rw::core
