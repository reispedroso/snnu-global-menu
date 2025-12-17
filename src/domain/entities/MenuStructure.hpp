#pragma once

#include <vector>
#include <memory>
#include "MenuItem.hpp"

namespace Snnu::Domain
{

  class MenuStructure
  {
  public:
    void AddRoot(std::shared_ptr<MenuItem> item)
    {
      roots_.push_back(std::move(item));
    }

    const std::vector<std::shared_ptr<MenuItem>> &GetRoots() const
    {
      return roots_;
    }

    void Clear()
    {
      roots_.clear();
    }

    bool IsEmpty() const
    {
      return roots_.empty();
    }

  private:
    std::vector<std::shared_ptr<MenuItem>> roots_;
  };
}