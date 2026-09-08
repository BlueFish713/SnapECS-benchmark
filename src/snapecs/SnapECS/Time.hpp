#pragma once

#include <algorithm>
#include <array>
#include <cstddef>

namespace snapecs {

class Time {
public:
  static inline float deltaTime = 0.0f;
  static inline float unscaledDeltaTime = 0.0f;
  static inline float smoothDeltaTime = 0.0f;
  static inline float fixedDeltaTime = 0.02f;
  static inline float fixedUnscaledDeltaTime = 0.02f;
  static inline float timeScale = 1.0f;

  static inline float time = 0.0f;
  static inline float unscaledTime = 0.0f;
  static inline float realtimeSinceStartup = 0.0f;

  static inline int frameCount = 0;

  static inline bool isPaused = false;

  static void ResetRuntime(float realtimeSeconds) {
    deltaTime = 0.0f;
    unscaledDeltaTime = 0.0f;
    smoothDeltaTime = 0.0f;
    fixedDeltaTime = isPaused ? 0.0f : fixedUnscaledDeltaTime * timeScale;

    time = 0.0f;
    unscaledTime = 0.0f;
    realtimeSinceStartup = 0.0f;

    frameCount = 0;

    startupRealtime_ = realtimeSeconds;
    lastRealtime_ = realtimeSeconds;
    ResetSmoothWindow();
  }

  static void Update(float realtimeSeconds) {
    const float rawUnscaledDelta = realtimeSeconds - lastRealtime_;
    unscaledDeltaTime = std::max(0.0f, rawUnscaledDelta);
    lastRealtime_ = realtimeSeconds;

    unscaledTime += unscaledDeltaTime;
    realtimeSinceStartup = std::max(0.0f, realtimeSeconds - startupRealtime_);
    ++frameCount;

    if (isPaused) {
      deltaTime = 0.0f;
      fixedDeltaTime = 0.0f;
      smoothDeltaTime = 0.0f;
      ResetSmoothWindow();
      return;
    }

    deltaTime = unscaledDeltaTime * timeScale;
    fixedDeltaTime = fixedUnscaledDeltaTime * timeScale;
    time += deltaTime;
    RecordSmoothDelta(deltaTime);
  }

private:
  static constexpr std::size_t smoothWindowSize_ = 10;

  static inline float startupRealtime_ = 0.0f;
  static inline float lastRealtime_ = 0.0f;
  static inline std::array<float, smoothWindowSize_> smoothDeltas_{};
  static inline std::size_t smoothIndex_ = 0;
  static inline std::size_t smoothCount_ = 0;

  static void ResetSmoothWindow() {
    smoothDeltas_.fill(0.0f);
    smoothIndex_ = 0;
    smoothCount_ = 0;
  }

  static void RecordSmoothDelta(float scaledDeltaTime) {
    smoothDeltas_[smoothIndex_] = scaledDeltaTime;
    smoothIndex_ = (smoothIndex_ + 1) % smoothWindowSize_;
    smoothCount_ = std::min(smoothCount_ + 1, smoothWindowSize_);

    float total = 0.0f;
    for (std::size_t i = 0; i < smoothCount_; ++i) {
      total += smoothDeltas_[i];
    }
    smoothDeltaTime = smoothCount_ == 0 ? 0.0f : total / static_cast<float>(smoothCount_);
  }
};

} // namespace snapecs
