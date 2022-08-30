#ifndef RW_DAT_SETTINGSNODE_HPP
#define RW_DAT_SETTINGSNODE_HPP

#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <vector>

namespace rw::dat
{
   /**
    * @brief Node of a settings tree.
    */
   class SettingsNode
   {
   public:
      /**
       * @brief Constructor.
       * @param parent Parent of the node.
       * @param name Name of the node.
       * @param value Value of the node.
       */
      explicit SettingsNode(SettingsNode* parent, std::string_view name, std::string_view value);

      /**
       * @brief Add a new child to the current node.
       * @param parent Parent of the child.
       * @param name Name of the child.
       * @param value Value of the child.
       * @return Pointer to the newly added child.
       */
      SettingsNode* addChild(SettingsNode* parent, const std::string& name, std::string_view value);

      /**
       * @brief Get the attribute with the given name.
       * @param name Name of the attribute to get.
       * @return Value of the attribute, if present.
       */
      [[nodiscard]] std::optional<std::string> attribute(std::string_view name) const;

      /**
       * @brief Get the attribute with the given name, or a default value if it does not exist.
       * @param name Name of the attribute to get.
       * @param defaultValue Default return value in case the attribute does not exist.
       * @return Value of the attribute or defaultValue.
       */
      [[nodiscard]] std::string attribute(std::string_view name, std::string_view defaultValue) const;

      /**
       * @brief Get a specific child of the node.
       * @param name Name of the child.
       * @param index Index of the child (0 by default).
       * @return Pointer to the child node. nullptr if non-existant.
       */
      [[nodiscard]] SettingsNode* child(std::string_view name, size_t index = 0U);

      /**
       * @brief Get the number of children with a specific name.
       * @param name Name of the children.
       * @return Number of children with the given name. 0U if empty or non-existant.
       */
      [[nodiscard]] size_t childNumber(std::string_view name) const;

      /**
       * @brief Get the name of the node.
       * @return Name of the node.
       */
      [[nodiscard]] std::string name() const;

      /**
       * @brief Get the parent of the current node.
       * @return Parent of the current node.
       */
      [[nodiscard]] SettingsNode* parent();

      /**
       * @brief Get the parent of the current node.
       * @return Parent of the current node.
       */
      [[nodiscard]] const SettingsNode* parent() const;

      /**
       * @brief Set the value of an attribute of the node.
       * @param name Name of the attribute to set.
       * @param value Value of the attribute to set.
       */
      void setAttribute(const std::string& name, std::string_view value);

      /**
       * @brief Set a new name for the node.
       * @param New name to set.
       */
      void setName(std::string_view newName);

      /**
       * @brief Set a new value for the node.
       * @param newValue New value to set.
       */
      void setValue(std::string_view newValue);

      /**
       * @brief Get the value of the node.
       * @return Name of the node.
       */
      [[nodiscard]] std::string value() const;

   private:
      SettingsNode*                                                                  parent_{ nullptr }; /**< Parent of the node. */
      std::map<std::string, std::vector<std::unique_ptr<SettingsNode>>, std::less<>> children_;          /**< Children of the node. */

      std::string                                     name_;       /**< Name of the node. */
      std::string                                     value_;      /**< Value of the node. */
      std::map<std::string, std::string, std::less<>> attributes_; /**< Attributes of the node. */

      mutable std::shared_mutex mtx_; /**< Mutex for protecting read/write operations. */
   };
} // namespace rw::dat

#endif