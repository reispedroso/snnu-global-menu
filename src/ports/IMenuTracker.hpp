#pragma once

#include <cstdint>
#include <functional>

namespace Snnu::Ports {
  using WindowFocusCallback = std::function<void(uint32_t windowId)>;

  class IWindowTracker {
    public:
      virtual ~IWindowTracker() = default;

      virtual void Start() = 0;
      virtual void SetOnFocusChanged(WindowFocusCallback callback) = 0;
  };
}