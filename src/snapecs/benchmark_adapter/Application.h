#ifndef ECS_BENCHMARKS_SNAPECS_APPLICATION_H_
#define ECS_BENCHMARKS_SNAPECS_APPLICATION_H_

#include "base/Application.h"
#include "base/FrameBuffer.h"
#include "benchmark_adapter/Systems.h"

namespace ecs::benchmarks::snapecs {

template <class WorldT, class SystemTypesT = systems::IndexedSystemSet, class... DisabledSystems>
class Application {
public:
  using EntityManager = WorldT;
  using TimeDelta = float;

  Application() : frame_buffer_(FrameBufferWidth, FrameBufferHeight) {}

  explicit Application(base::add_more_complex_system_t add_more_complex_system)
      : add_more_complex_system_(add_more_complex_system), frame_buffer_(FrameBufferWidth, FrameBufferHeight) {}

  EntityManager& getEntities() noexcept { return world_; }

  void init() {
    (world_.template remove_system<DisabledSystems>(), ...);
    if (add_more_complex_system_ == base::add_more_complex_system_t::UseBasicSystems) {
      world_.template remove_system<typename SystemTypesT::MoreComplexSystem>();
      world_.template remove_system<typename SystemTypesT::HealthSystem>();
      world_.template remove_system<typename SystemTypesT::DamageSystem>();
      world_.template remove_system<typename SystemTypesT::SpriteSystem>();
      world_.template remove_system<typename SystemTypesT::RenderSystem>();
    }
    world_.template system<typename SystemTypesT::RenderSystem>().setFrameBuffer(frame_buffer_);
  }

  void uninit() {}

  void update(TimeDelta dt) {
    world_.template system<typename SystemTypesT::MovementSystem>().setTimeDelta(dt);
    world_.template system<typename SystemTypesT::DataSystem>().setTimeDelta(dt);
    world_.update();
  }

private:
  inline static constexpr std::uint32_t FrameBufferWidth = 320;
  inline static constexpr std::uint32_t FrameBufferHeight = 240;

  base::add_more_complex_system_t add_more_complex_system_{base::add_more_complex_system_t::UseBasicSystems};
  base::FrameBuffer frame_buffer_;
  EntityManager world_;
};

} // namespace ecs::benchmarks::snapecs

#endif // ECS_BENCHMARKS_SNAPECS_APPLICATION_H_
