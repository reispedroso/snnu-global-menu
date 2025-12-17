#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace Snnu::Domain {

    enum class MenuItemType {
        Standard,
        Separator,
        Submenu
    };

    class MenuItem {
    public:
      MenuItem(std::string label, MenuItemType type = MenuItemType::Standard)
      : label_(std::move(label)), type_(type) {}
   
        const std::string& GetLabel() const { return label_; }
        
        MenuItemType GetType() const { return type_; }
        
        void AddChild(std::shared_ptr<MenuItem> child) {
            children_.push_back(std::move(child));
            type_ = MenuItemType::Submenu;
        }

        const std::vector<std::shared_ptr<MenuItem>>& GetChildren() const {
            return children_;
        }

        void SetAction(std::string actionName) {
            action_name_ = std::move(actionName);
        }

        std::optional<std::string> GetAction() const {
            return action_name_;
        }

    private:
        std::string label_;
        MenuItemType type_;
        std::vector<std::shared_ptr<MenuItem>> children_;
        std::optional<std::string> action_name_;
    };

}