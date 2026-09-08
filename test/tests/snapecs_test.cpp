#include "benchmark_adapter/Application.h"
#include "benchmark_adapter/EntityFactory.h"
#include "benchmark_adapter/Worlds.h"

#include <SnapECS/SnapECS.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>

namespace ecs::benchmarks::snapecs::tests {

namespace {

class OneComponentCounter final
    : public ::snapecs::System<::snapecs::Query<>, ::snapecs::Read<components::PositionComponent>,
                               ::snapecs::Write<>> {
public:
  void BeginUpdate() { count = 0; }
  void Update(::snapecs::Entity, const components::PositionComponent&) override { ++count; }
  std::size_t count{0};
};

class TwoComponentCounter final
    : public ::snapecs::System<::snapecs::Query<>,
                               ::snapecs::Read<components::PositionComponent, components::VelocityComponent>,
                               ::snapecs::Write<>> {
public:
  void BeginUpdate() { count = 0; }
  void Update(::snapecs::Entity, const components::PositionComponent&,
              const components::VelocityComponent&) override {
    ++count;
  }
  std::size_t count{0};
};

class ThreeComponentCounter final
    : public ::snapecs::System<
          ::snapecs::Query<>,
          ::snapecs::Read<components::PositionComponent, components::VelocityComponent, components::DataComponent>,
          ::snapecs::Write<>> {
public:
  void BeginUpdate() { count = 0; }
  void Update(::snapecs::Entity, const components::PositionComponent&, const components::VelocityComponent&,
              const components::DataComponent&) override {
    ++count;
  }
  std::size_t count{0};
};

using CounterWorld =
    ::snapecs::World<BenchmarkComponents,
                     ::snapecs::Systems<OneComponentCounter, TwoComponentCounter, ThreeComponentCounter>>;

template <class ComponentsT>
class PolicyOneComponentCounter final
    : public ::snapecs::System<::snapecs::Query<>, ::snapecs::Read<typename ComponentsT::PositionComponent>,
                               ::snapecs::Write<>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;

  void BeginUpdate() { count = 0; }
  void Update(::snapecs::Entity, const PositionComponent&) override { ++count; }
  std::size_t count{0};
};

template <class ComponentsT>
class PolicyTwoComponentCounter final
    : public ::snapecs::System<::snapecs::Query<>,
                               ::snapecs::Read<typename ComponentsT::PositionComponent,
                                               typename ComponentsT::VelocityComponent>,
                               ::snapecs::Write<>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;
  using VelocityComponent = typename ComponentsT::VelocityComponent;

  void BeginUpdate() { count = 0; }
  void Update(::snapecs::Entity, const PositionComponent&, const VelocityComponent&) override { ++count; }
  std::size_t count{0};
};

template <class ComponentsT>
class PolicyThreeComponentCounter final
    : public ::snapecs::System<
          ::snapecs::Query<>,
          ::snapecs::Read<typename ComponentsT::PositionComponent, typename ComponentsT::VelocityComponent,
                          typename ComponentsT::DataComponent>,
          ::snapecs::Write<>> {
public:
  using PositionComponent = typename ComponentsT::PositionComponent;
  using VelocityComponent = typename ComponentsT::VelocityComponent;
  using DataComponent = typename ComponentsT::DataComponent;

  void BeginUpdate() { count = 0; }
  void Update(::snapecs::Entity, const PositionComponent&, const VelocityComponent&, const DataComponent&) override {
    ++count;
  }
  std::size_t count{0};
};

template <class ConfigT>
void verifyStoragePolicy() {
  using Components = typename ConfigT::ComponentTypes;
  using EntityWorldT = typename ConfigT::EntityWorld;
  using ApplicationWorldT = typename ConfigT::ApplicationWorld;
  using SystemTypes = typename ConfigT::SystemTypes;

  EntityWorldT entity_world;
  entities::EntityFactory<EntityWorldT, Components> entity_factory;
  const auto empty = entity_factory.createEmpty(entity_world);
  const auto single = entity_factory.createSingle(entity_world);
  const auto minimal = entity_factory.createMinimal(entity_world);
  const auto full = entity_factory.create(entity_world);

  REQUIRE(entity_factory.valid(entity_world, empty));
  REQUIRE(entity_factory.valid(entity_world, single));
  REQUIRE(entity_factory.valid(entity_world, minimal));
  REQUIRE(entity_factory.valid(entity_world, full));
  REQUIRE_FALSE(entity_world.template has_component<typename Components::PositionComponent>(empty));
  REQUIRE(entity_world.template has_component<typename Components::PositionComponent>(single));
  REQUIRE(entity_world.template has_component<typename Components::VelocityComponent>(minimal));
  REQUIRE(entity_world.template has_component<typename Components::DataComponent>(full));

  auto& position = entity_factory.getComponentOne(entity_world, minimal);
  position.x = 42.0F;
  REQUIRE(entity_factory.getComponentOneConst(entity_world, minimal).x == Catch::Approx(42.0F));
  entity_factory.removeComponentOne(entity_world, minimal);
  REQUIRE_FALSE(entity_world.template has_component<typename Components::PositionComponent>(minimal));
  entity_factory.addComponentOne(entity_world, minimal);
  REQUIRE(entity_world.template has_component<typename Components::PositionComponent>(minimal));
  entity_factory.addComponentEmpty(entity_world, minimal);
  REQUIRE(entity_world.template has_component<typename Components::EmptyComponent>(minimal));

  Application<ApplicationWorldT, SystemTypes> app;
  entities::EntityFactory<ApplicationWorldT, Components> application_factory;
  app.init();
  const auto updated_entity = application_factory.createMinimal(app.getEntities());
  app.update(1.0F / 60.0F);
  app.update(1.0F / 60.0F);

  // This accessor reads the source table. For CachedComponent, the assertion also
  // verifies that system-view writes synchronize back after every update.
  const auto& updated_position = application_factory.getComponentOneConst(app.getEntities(), updated_entity);
  REQUIRE(updated_position.x == Catch::Approx(2.0F / 60.0F));
  REQUIRE(updated_position.y == Catch::Approx(2.0F / 60.0F));
  app.uninit();

  using OneCounter = PolicyOneComponentCounter<Components>;
  using TwoCounter = PolicyTwoComponentCounter<Components>;
  using ThreeCounter = PolicyThreeComponentCounter<Components>;
  using PolicyCounterWorld =
      ::snapecs::World<typename ConfigT::BenchmarkComponents,
                       ::snapecs::Systems<OneCounter, TwoCounter, ThreeCounter>>;

  PolicyCounterWorld counter_world;
  entities::EntityFactory<PolicyCounterWorld, Components> counter_factory;
  counter_factory.createSingle(counter_world);
  counter_factory.createMinimal(counter_world);
  counter_factory.create(counter_world);
  counter_world.update();

  REQUIRE(counter_world.template system<OneCounter>().count == 3);
  REQUIRE(counter_world.template system<TwoCounter>().count == 2);
  REQUIRE(counter_world.template system<ThreeCounter>().count == 1);
}

} // namespace

TEST_CASE("SnapECS creates supported entity shapes", "[snapecs][entity_factory]") {
  EntityWorld world;
  entities::EntityFactory<EntityWorld> factory;

  const auto empty = factory.createEmpty(world);
  const auto single = factory.createSingle(world);
  const auto minimal = factory.createMinimal(world);
  const auto full = factory.create(world);

  REQUIRE(factory.valid(world, empty));
  REQUIRE(factory.valid(world, single));
  REQUIRE(factory.valid(world, minimal));
  REQUIRE(factory.valid(world, full));

  REQUIRE_FALSE(world.has_component<components::PositionComponent>(empty));
  REQUIRE(world.has_component<components::PositionComponent>(single));
  REQUIRE(world.has_component<components::PositionComponent>(minimal));
  REQUIRE(world.has_component<components::VelocityComponent>(minimal));
  REQUIRE(world.has_component<components::DataComponent>(full));
}

TEST_CASE("SnapECS manipulates indexed components and tags", "[snapecs][components]") {
  EntityWorld world;
  entities::EntityFactory<EntityWorld> factory;
  const auto entity = factory.createMinimal(world);

  auto& position = factory.getComponentOne(world, entity);
  position.x = 42.0F;
  REQUIRE(factory.getComponentOneConst(world, entity).x == Catch::Approx(42.0F));

  factory.removeComponentOne(world, entity);
  REQUIRE_FALSE(world.has_component<components::PositionComponent>(entity));
  factory.addComponentOne(world, entity);
  REQUIRE(world.has_component<components::PositionComponent>(entity));

  factory.addComponentEmpty(world, entity);
  REQUIRE(world.has_component<components::EmptyComponent>(entity));
  world.remove_component<components::EmptyComponent>(entity);
  REQUIRE_FALSE(world.has_component<components::EmptyComponent>(entity));
}

TEST_CASE("SnapECS application updates position", "[snapecs][application]") {
  Application<ApplicationWorld> app;
  entities::EntityFactory<ApplicationWorld> factory;
  app.init();

  const auto entity = factory.createMinimal(app.getEntities());
  app.update(1.0F / 60.0F);

  const auto& position = factory.getComponentOneConst(app.getEntities(), entity);
  REQUIRE(position.x == Catch::Approx(1.0F / 60.0F));
  REQUIRE(position.y == Catch::Approx(1.0F / 60.0F));
  app.uninit();
}

TEST_CASE("SnapECS views match one two and three components", "[snapecs][views]") {
  CounterWorld world;
  entities::EntityFactory<CounterWorld> factory;
  factory.createSingle(world);
  factory.createMinimal(world);
  factory.create(world);

  world.update();

  REQUIRE(world.system<OneComponentCounter>().count == 3);
  REQUIRE(world.system<TwoComponentCounter>().count == 2);
  REQUIRE(world.system<ThreeComponentCounter>().count == 1);
}

TEST_CASE("SnapECS adapter supports indexed and cached storage policies", "[snapecs][storage_policy]") {
  SECTION("Indexed") { verifyStoragePolicy<IndexedBenchmarkConfig>(); }
  SECTION("Cached") { verifyStoragePolicy<CachedBenchmarkConfig>(); }
}

} // namespace ecs::benchmarks::snapecs::tests
