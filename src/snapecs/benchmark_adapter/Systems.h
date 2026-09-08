#ifndef ECS_BENCHMARKS_SNAPECS_SYSTEMS_H_
#define ECS_BENCHMARKS_SNAPECS_SYSTEMS_H_

#include "base/FrameBuffer.h"
#include "base/systems/DamageSystem.h"
#include "base/systems/DataSystem.h"
#include "base/systems/HealthSystem.h"
#include "base/systems/MoreComplexSystem.h"
#include "base/systems/MovementSystem.h"
#include "base/systems/RenderSystem.h"
#include "base/systems/SpriteSystem.h"
#include "benchmark_adapter/Components.h"

#include <SnapECS/SnapECS.hpp>

namespace ecs::benchmarks::snapecs::systems {

struct LogicEntityManager {};

using MovementLogic = base::systems::MovementSystem<LogicEntityManager, float>;
using DataLogic = base::systems::DataSystem<LogicEntityManager, float>;
using MoreComplexLogic = base::systems::MoreComplexSystem<LogicEntityManager, float>;
using HealthLogic = base::systems::HealthSystem<LogicEntityManager, float>;
using DamageLogic = base::systems::DamageSystem<LogicEntityManager, float>;
using SpriteLogic = base::systems::SpriteSystem<LogicEntityManager, float>;
using RenderLogic = base::systems::RenderSystem<LogicEntityManager, float>;

template <class ComponentsT>
class MovementSystemT final
    : public ::snapecs::System<::snapecs::Query<>, ::snapecs::Read<typename ComponentsT::VelocityComponent>,
                               ::snapecs::Write<typename ComponentsT::PositionComponent>> {
public:
  using VelocityComponent = typename ComponentsT::VelocityComponent;
  using PositionComponent = typename ComponentsT::PositionComponent;

  void setTimeDelta(float dt) { dt_ = dt; }

  void Update(::snapecs::Entity, const VelocityComponent& velocity, PositionComponent& position) override {
    MovementLogic::updatePosition(position, velocity, dt_);
  }

private:
  float dt_{0.0F};
};

template <class ComponentsT>
class DataSystemT final
    : public ::snapecs::System<::snapecs::Query<>, ::snapecs::Read<>,
                               ::snapecs::Write<typename ComponentsT::DataComponent>> {
public:
  using DataComponent = typename ComponentsT::DataComponent;

  void setTimeDelta(float dt) { dt_ = dt; }

  void Update(::snapecs::Entity, DataComponent& data) override { DataLogic::updateData(data, dt_); }

private:
  float dt_{0.0F};
};

template <class ComponentsT>
class MoreComplexSystemT final
    : public ::snapecs::System<
          ::snapecs::Query<>, ::snapecs::Read<typename ComponentsT::PositionComponent>,
          ::snapecs::Write<typename ComponentsT::VelocityComponent, typename ComponentsT::DataComponent>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;
  using VelocityComponent = typename ComponentsT::VelocityComponent;
  using DataComponent = typename ComponentsT::DataComponent;

  void Update(::snapecs::Entity, const PositionComponent& position, VelocityComponent& velocity,
              DataComponent& data) override {
    MoreComplexLogic::updateComponents(position, velocity, data);
  }
};

template <class ComponentsT>
class HealthSystemT final
    : public ::snapecs::System<::snapecs::Query<>, ::snapecs::Read<>,
                               ::snapecs::Write<typename ComponentsT::HealthComponent>> {
public:
  using HealthComponent = typename ComponentsT::HealthComponent;

  void Update(::snapecs::Entity, HealthComponent& health) override { HealthLogic::updateHealth(health); }
};

template <class ComponentsT>
class DamageSystemT final
    : public ::snapecs::System<::snapecs::Query<>, ::snapecs::Read<typename ComponentsT::DamageComponent>,
                               ::snapecs::Write<typename ComponentsT::HealthComponent>> {
public:
  using DamageComponent = typename ComponentsT::DamageComponent;
  using HealthComponent = typename ComponentsT::HealthComponent;

  void Update(::snapecs::Entity, const DamageComponent& damage, HealthComponent& health) override {
    DamageLogic::updateDamage(health, damage);
  }
};

template <class ComponentsT>
class SpriteSystemT final
    : public ::snapecs::System<::snapecs::Query<>,
                               ::snapecs::Read<typename ComponentsT::PlayerComponent,
                                               typename ComponentsT::HealthComponent>,
                               ::snapecs::Write<typename ComponentsT::SpriteComponent>> {
public:
  using PlayerComponent = typename ComponentsT::PlayerComponent;
  using HealthComponent = typename ComponentsT::HealthComponent;
  using SpriteComponent = typename ComponentsT::SpriteComponent;

  void Update(::snapecs::Entity, const PlayerComponent& player, const HealthComponent& health,
              SpriteComponent& sprite) override {
    SpriteLogic::updateSprite(sprite, player, health);
  }
};

template <class ComponentsT>
class RenderSystemT final
    : public ::snapecs::System<::snapecs::Query<>,
                               ::snapecs::Read<typename ComponentsT::PositionComponent,
                                               typename ComponentsT::SpriteComponent>,
                               ::snapecs::Write<>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;
  using SpriteComponent = typename ComponentsT::SpriteComponent;

  void setFrameBuffer(base::FrameBuffer& frame_buffer) { frame_buffer_ = &frame_buffer; }

  void Update(::snapecs::Entity, const PositionComponent& position, const SpriteComponent& sprite) override {
    if (frame_buffer_ != nullptr) {
      RenderLogic::renderSprite(*frame_buffer_, position, sprite);
    }
  }

private:
  base::FrameBuffer* frame_buffer_{nullptr};
};

template <class ComponentsT>
struct SystemSet {
  using MovementSystem = MovementSystemT<ComponentsT>;
  using DataSystem = DataSystemT<ComponentsT>;
  using MoreComplexSystem = MoreComplexSystemT<ComponentsT>;
  using HealthSystem = HealthSystemT<ComponentsT>;
  using DamageSystem = DamageSystemT<ComponentsT>;
  using SpriteSystem = SpriteSystemT<ComponentsT>;
  using RenderSystem = RenderSystemT<ComponentsT>;
};

using IndexedSystemSet = SystemSet<components::IndexedComponentSet>;
using CachedSystemSet = SystemSet<components::CachedComponentSet>;

using MovementSystem = IndexedSystemSet::MovementSystem;
using DataSystem = IndexedSystemSet::DataSystem;
using MoreComplexSystem = IndexedSystemSet::MoreComplexSystem;
using HealthSystem = IndexedSystemSet::HealthSystem;
using DamageSystem = IndexedSystemSet::DamageSystem;
using SpriteSystem = IndexedSystemSet::SpriteSystem;
using RenderSystem = IndexedSystemSet::RenderSystem;

} // namespace ecs::benchmarks::snapecs::systems

#endif // ECS_BENCHMARKS_SNAPECS_SYSTEMS_H_
