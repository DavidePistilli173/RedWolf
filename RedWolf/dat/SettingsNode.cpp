#include "SettingsNode.hpp"

using namespace rw::dat;

SettingsNode::SettingsNode(SettingsNode* parent, std::string_view name, std::string_view value) :
   parent_{ parent }, name_{ name }, value_{ value }
{
}

SettingsNode* SettingsNode::addChild(SettingsNode* parent, const std::string& name, std::string_view value)
{
   std::scoped_lock lck{ mtx_ };

   auto it = children_.find(name);
   if (it == children_.end())
   {
      children_.emplace(name, std::vector<std::unique_ptr<SettingsNode>>());
   }

   return children_[name].emplace_back(std::make_unique<SettingsNode>(parent, name, value)).get();
}

std::optional<std::string> SettingsNode::attribute(std::string_view name) const
{
   std::shared_lock lck{ mtx_ };

   auto it = attributes_.find(name);
   if (it == attributes_.end()) return std::optional<std::string>();

   return it->second;
}

std::string SettingsNode::attribute(std::string_view name, std::string_view defaultValue) const
{
   std::shared_lock lck{ mtx_ };

   auto it = attributes_.find(name);
   if (it == attributes_.end()) return std::string(defaultValue);

   return it->second;
}

SettingsNode* SettingsNode::child(std::string_view name, size_t index)
{
   std::shared_lock lck{ mtx_ };

   auto it = children_.find(name);
   if (it == children_.end()) return nullptr;
   if (it->second.size() <= index) return nullptr;

   return it->second[index].get();
}

size_t SettingsNode::childNumber(std::string_view name) const
{
   std::shared_lock lck{ mtx_ };

   auto it = children_.find(name);
   if (it == children_.end()) return 0U;
   return it->second.size();
}

std::string SettingsNode::name() const
{
   std::shared_lock lck{ mtx_ };

   return name_;
}

SettingsNode* SettingsNode::parent()
{
   std::shared_lock lck{ mtx_ };

   return parent_;
}

const SettingsNode* SettingsNode::parent() const
{
   std::shared_lock lck{ mtx_ };

   return parent_;
}

void SettingsNode::setAttribute(const std::string& name, std::string_view value)
{
   std::scoped_lock lck{ mtx_ };
   attributes_[name] = value;
}

void SettingsNode::setName(std::string_view newName)
{
   std::scoped_lock lck{ mtx_ };

   name_ = newName;
}

void SettingsNode::setValue(std::string_view newValue)
{
   std::scoped_lock lck{ mtx_ };

   value_ = newValue;
}

std::string SettingsNode::value() const
{
   std::shared_lock lck{ mtx_ };

   return value_;
}