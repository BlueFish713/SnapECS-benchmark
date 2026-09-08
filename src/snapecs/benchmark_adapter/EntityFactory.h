#ifndef ECS_BENCHMARKS_SNAPECS_ENTITYFACTORY_H_
#define ECS_BENCHMARKS_SNAPECS_ENTITYFACTORY_H_

#include "benchmark_adapter/Components.h"

#include <SnapECS/SnapECS.hpp>

namespace ecs::benchmarks::snapecs::entities {

template <class WorldT, class ComponentsT = components::IndexedComponentSet>
class EntityFactory {
public:
  using EntityManager = WorldT;
  using Entity = ::snapecs::Entity;

  Entity createEmpty(EntityManager& world) const { return world.create_entity(); }

  Entity createSingle(EntityManager& world) const {
    const auto entity = createEmpty(world);
    addComponentOne(world, entity);
    return entity;
  }

  Entity createMinimal(EntityManager& world) const {
    const auto entity = createSingle(world);
    addComponentTwo(world, entity);
    return entity;
  }

  Entity create(EntityManager& world) const {
    const auto entity = createMinimal(world);
    addComponentThree(world, entity);
    return entity;
  }

  [[nodiscard]] bool valid(const EntityManager& world, Entity entity) const { return world.is_active(entity); }

  [[nodiscard]] const base::components::PositionComponent& getComponentOneConst(const EntityManager& world,
                                                                                Entity entity) const {
    return world.template get_component<typename ComponentsT::PositionComponent>(entity);
  }

  [[nodiscard]] const base::components::VelocityComponent& getComponentTwoConst(const EntityManager& world,
                                                                                Entity entity) const {
    return world.template get_component<typename ComponentsT::VelocityComponent>(entity);
  }

  [[nodiscard]] base::components::PositionComponent& getComponentOne(EntityManager& world, Entity entity) const {
    return world.template get_component<typename ComponentsT::PositionComponent>(entity);
  }

  [[nodiscard]] base::components::VelocityComponent& getComponentTwo(EntityManager& world, Entity entity) const {
    return world.template get_component<typename ComponentsT::VelocityComponent>(entity);
  }

  [[nodiscard]] base::components::DataComponent* getOptionalComponentThree(EntityManager& world, Entity entity) const {
    return world.template has_component<typename ComponentsT::DataComponent>(entity)
               ? &world.template get_component<typename ComponentsT::DataComponent>(entity)
               : nullptr;
  }

  [[nodiscard]] const base::components::DataComponent* getOptionalComponentThreeConst(const EntityManager& world,
                                                                                      Entity entity) const {
    return world.template has_component<typename ComponentsT::DataComponent>(entity)
               ? &world.template get_component<typename ComponentsT::DataComponent>(entity)
               : nullptr;
  }

  void removeComponentOne(EntityManager& world, Entity entity) const {
    world.template remove_component<typename ComponentsT::PositionComponent>(entity);
  }

  void removeComponentTwo(EntityManager& world, Entity entity) const {
    world.template remove_component<typename ComponentsT::VelocityComponent>(entity);
  }

  void removeComponentThree(EntityManager& world, Entity entity) const {
    world.template remove_component<typename ComponentsT::DataComponent>(entity);
  }

  void addComponentOne(EntityManager& world, Entity entity) const {
    world.template add_component<typename ComponentsT::PositionComponent>(entity);
  }

  void addComponentTwo(EntityManager& world, Entity entity) const {
    world.template add_component<typename ComponentsT::VelocityComponent>(entity);
  }

  void addComponentThree(EntityManager& world, Entity entity) const {
    world.template add_component<typename ComponentsT::DataComponent>(entity);
  }

  void addComponentEmpty(EntityManager& world, Entity entity) const {
    world.template add_component<typename ComponentsT::EmptyComponent>(entity);
  }
};

} // namespace ecs::benchmarks::snapecs::entities

#endif // ECS_BENCHMARKS_SNAPECS_ENTITYFACTORY_H_
