#pragma once

#include <cstdint>
#include <optional>
#include "../domain/entities/MenuStructure.hpp"

namespace Snnu::Ports {
  class IMenuProvider {
    public:
      virtual ~IMenuProvider() = default;

      virtual std::optional<Snnu::Domain::MenuStructure> GetMenuForWindow(uint32_t windowId) = 0;
  };
}