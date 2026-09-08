#ifndef ECS_BENCHMARKS_SNAPECS_HEROMONSTERENTITYFACTORY_H_
#define ECS_BENCHMARKS_SNAPECS_HEROMONSTERENTITYFACTORY_H_

#include "base/entities/HeroMonsterEntityFactory.h"
#include "benchmark_adapter/Components.h"

#include <SnapECS/SnapECS.hpp>

namespace ecs::benchmarks::snapecs::entities {

template <class WorldT, class ComponentsT = components::IndexedComponentSet>
class HeroMonsterEntityFactory final
    : public base::entities::HeroMonsterEntityFactory<
          WorldT, ::snapecs::Entity, typename ComponentsT::PositionComponent, typename ComponentsT::PlayerComponent,
          typename ComponentsT::HealthComponent, typename ComponentsT::DamageComponent,
          typename ComponentsT::SpriteComponent> {
  using PositionComponent = typename ComponentsT::PositionComponent;
  using PlayerComponent = typename ComponentsT::PlayerComponent;
  using HealthComponent = typename ComponentsT::HealthComponent;
  using DamageComponent = typename ComponentsT::DamageComponent;
  using SpriteComponent = typename ComponentsT::SpriteComponent;
  using Base = base::entities::HeroMonsterEntityFactory<WorldT, ::snapecs::Entity, PositionComponent, PlayerComponent,
                                                        HealthComponent, DamageComponent, SpriteComponent>;

public:
  using typename Base::Entity;
  using typename Base::EntityManager;

  Entity createRandom(EntityManager& world) override {
    const Entity entity = world.create_entity();
    addComponents(world, entity);
    this->initComponents(world, entity);
    return entity;
  }

  Entity createHero(EntityManager& world) override {
    const Entity entity = world.create_entity();
    addComponents(world, entity);
    this->initComponents(world, entity, base::components::PlayerType::Hero);
    return entity;
  }

  Entity createMonster(EntityManager& world) override {
    const Entity entity = world.create_entity();
    addComponents(world, entity);
    this->initComponents(world, entity, base::components::PlayerType::Monster);
    return entity;
  }

  void addComponents(EntityManager& world, Entity entity) override {
    world.template set_component<PlayerComponent>(entity);
    world.template set_component<HealthComponent>(entity);
    world.template set_component<DamageComponent>(entity);
    world.template set_component<PositionComponent>(entity);
    world.template set_component<SpriteComponent>(entity);
  }

  PositionComponent& getPositionComponent(EntityManager& world, Entity entity) override {
    return world.template get_component<PositionComponent>(entity);
  }

  PlayerComponent& getPlayerComponent(EntityManager& world, Entity entity) override {
    return world.template get_component<PlayerComponent>(entity);
  }

  HealthComponent& getHealthComponent(EntityManager& world, Entity entity) override {
    return world.template get_component<HealthComponent>(entity);
  }

  DamageComponent& getDamageComponent(EntityManager& world, Entity entity) override {
    return world.template get_component<DamageComponent>(entity);
  }

  SpriteComponent& getSpriteComponent(EntityManager& world, Entity entity) override {
    return world.template get_component<SpriteComponent>(entity);
  }

  const PlayerComponent& getPlayerComponentConst(EntityManager& world, Entity entity) override {
    return world.template get_component<PlayerComponent>(entity);
  }
};

} // namespace ecs::benchmarks::snapecs::entities

#endif // ECS_BENCHMARKS_SNAPECS_HEROMONSTERENTITYFACTORY_H_
