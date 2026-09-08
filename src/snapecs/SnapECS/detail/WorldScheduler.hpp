#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace snapecs {

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
constexpr typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::SchedulerMask
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::system_access_mask() {
  static_assert(detail::ContainsV<SystemList, SystemT>,
                "System must be registered in World<..., Systems<...>>.");
  return make_system_mask<SystemT>();
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::World()
    : World(SchedulerConfig{}) {}

template <typename... RegisteredComponents, typename... RegisteredUnits>
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::World(SchedulerConfig config)
    : schedulerConfig_(config) {
  initialize_system_instances(SystemList{});
  systemEntries_.fill(Npos);
  initialize_group_tree();
  initialize_registered_units(RegisteredUnitList{});
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::update() {
  ensure_execution_plan(); // 현재 execution plan이 최신인지 dirty-flag로 확인
  execute_plan(); // plan 실행
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::configure_scheduler(SchedulerConfig config) {
  schedulerConfig_ = config;

  // worker count가 바뀔 수 있으므로 기존 pool을 멈춥니다.
  // 다음 병렬 batch 실행 시 새 설정으로 lazy configure됩니다.
  threadPool_.stop();
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
const SchedulerConfig &World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::scheduler_config() const {
  return schedulerConfig_;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
GroupId World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::create_group(
    GroupId parent,
    GroupPolicy policy,
    SchedulePosition position) {
  if (!valid_group(parent)) {
    return {};
  }

  const auto group = create_group_node(policy, false);
  attach_group_entry(parent.value, group.value, position);
  return group;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::destroy_group(GroupId group) {
  if (!valid_group(group) || groups_[group.value].fixed) {
    return false;
  }

  destroy_group_contents(group.value);

  const auto parentEntry = groups_[group.value].parentEntry;
  if (parentEntry != Npos) {
    detach_entry(parentEntry);
    entries_[parentEntry].active = false;
  }

  groups_[group.value].active = false;
  groups_[group.value].firstChild = Npos;
  groups_[group.value].lastChild = Npos;
  groups_[group.value].parentEntry = Npos;
  groups_[group.value].plan = {};
  return true;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::move_group(
    GroupId group,
    GroupId newParent) {
  // fixed core group은 이동할 수 없고, 자기 자신이나 자식 밑으로 이동하면 cycle이 생기므로 막습니다.
  if (!valid_group(group) || !valid_group(newParent) || groups_[group.value].fixed ||
      is_group_ancestor_or_self(group.value, newParent.value)) {
    return false;
  }

  const auto parentEntry = groups_[group.value].parentEntry;
  if (parentEntry == Npos) {
    return false;
  }

  detach_entry(parentEntry);
  attach_entry(newParent.value, parentEntry);
  return true;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::insert_system(
    GroupId parent,
    SchedulePosition position) {
  static_assert(detail::ContainsV<SystemList, SystemT>,
                "System must be registered in World<..., Systems<...>>.");
  return insert_system_by_index(SystemId<SystemT>, parent, position);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::remove_system() {
  static_assert(detail::ContainsV<SystemList, SystemT>,
                "System must be registered in World<..., Systems<...>>.");
  return remove_system_by_index(SystemId<SystemT>);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::move_system(GroupId newParent) {
  static_assert(detail::ContainsV<SystemList, SystemT>,
                "System must be registered in World<..., Systems<...>>.");
  return move_system_by_index(SystemId<SystemT>, newParent);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
constexpr typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::SchedulerMask
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::make_system_mask() {
  SchedulerMask mask{};
  add_signature_bits<SystemT>(mask, SystemSignature<SystemT>{});
  add_structural_bits<SystemT>(mask);

  // 같은 SystemViewTable을 공유하는 system이 동시에 for_each를 실행하면 view 내부 state가 충돌합니다.
  // 그래서 view id도 write bit로 표시합니다.
  detail::set_mask_bit(mask.write, ComponentCount + ViewId<SystemT>);
  return mask;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
constexpr void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_structural_bits(
    SchedulerMask& mask) {
  using Change = typename SystemT::StructuralChangeType;
  using StructuralComponents = typename Change::ComponentList;

  if constexpr (mp::mp_size<StructuralComponents>::value != 0) {
    detail::set_mask_bit(mask.read, EntityRegistryAccessBit);
  }
  if constexpr (Change::CreatesEntities) {
    detail::set_mask_bit(mask.write, EntityRegistryAccessBit);
  }

  add_structural_component_bits(mask, StructuralComponents{});
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename... ComponentsT>
constexpr void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_structural_component_bits(
    SchedulerMask& mask,
    mp::mp_list<ComponentsT...>) {
  (add_structural_component_bit<ComponentsT>(mask), ...);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Component>
constexpr void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_structural_component_bit(
    SchedulerMask& mask) {
  static_assert(detail::ContainsV<ComponentList, Component>,
                "StructuralChange components must be registered in the World.");
  static_assert(!IsSingleComponentV<Component>,
                "Single components cannot be added or removed structurally.");

  detail::set_mask_bit(mask.write, ComponentId<Component>);
  add_view_write_bits(mask, RelatedViewIndices<Component>{});
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename... Indices>
constexpr void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_view_write_bits(
    SchedulerMask& mask,
    mp::mp_list<Indices...>) {
  (detail::set_mask_bit(mask.write, ComponentCount + Indices::value), ...);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT, typename... Slots>
constexpr void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_signature_bits(
    SchedulerMask &mask,
    detail::AccessSignature<Slots...>) {
  (add_slot_bits<Slots>(mask), ...);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Slot>
constexpr void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_slot_bits(
    SchedulerMask &mask) {
  constexpr auto bit = ComponentId<typename Slot::ComponentType>;
  if constexpr (Slot::Reads) {
    detail::set_mask_bit(mask.read, bit);
  }
  if constexpr (Slot::Writes) {
    detail::set_mask_bit(mask.write, bit);
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::run_system_static(World &world) {
  world.template update_system<SystemT>();
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
std::size_t World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::system_work_static(
    const World &world) {
  return world.template view_for<SystemT>().entity_count();
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename... SystemTypes>
constexpr std::array<
    typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::RuntimeSystemInfo,
    sizeof...(SystemTypes)>
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::make_system_infos(
    mp::mp_list<SystemTypes...>) {
  return {RuntimeSystemInfo{system_access_mask<SystemTypes>(),
                            &run_system_static<SystemTypes>,
                            &system_work_static<SystemTypes>}...};
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::initialize_system_instance() {
  initialize_system_instance<SystemT>(typename SystemT::SingletonReadOnlyList{},
                                      typename SystemT::SingletonWriteList{});
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename SystemT, typename... ReadSingletons, typename... WriteSingletons>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::initialize_system_instance(
    mp::mp_list<ReadSingletons...>,
    mp::mp_list<WriteSingletons...>) {
  static_assert(
      std::is_constructible_v<SystemT, const ReadSingletons&..., WriteSingletons&...>,
      "A System using SingleComponent must accept its read-only singleton const references "
      "followed by its writable singleton references in its constructor.");

  detail::get_by_index<SystemId<SystemT>>(systems_)
      .emplace(std::as_const(single_component<ReadSingletons>())...,
               single_component<WriteSingletons>()...);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename... SystemsT>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::initialize_system_instances(
    mp::mp_list<SystemsT...>) {
  (initialize_system_instance<SystemsT>(), ...);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::initialize_group_tree() {
  groups_.reserve(5);
  entries_.reserve(5 + SystemCount);

  GroupNode mainGroup;
  mainGroup.policy = GroupPolicy::NonParallel;
  mainGroup.fixed = true;
  groups_.push_back(mainGroup);

  // index 1..4는 public core group getter와 대응합니다.
  add_core_group(GroupPolicy::Parallel);
  add_core_group(GroupPolicy::Parallel);
  add_core_group(GroupPolicy::Parallel);
  add_core_group(GroupPolicy::Parallel);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::add_core_group(GroupPolicy policy) {
  const auto group = create_group_node(policy, true);
  attach_group_entry(MainGroupIndex, group.value, SchedulePosition::Auto);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename... Units>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::initialize_registered_units(
    mp::mp_list<Units...>) {
  // Systems<...>에 등록된 top-level unit은 기본적으로 script group 아래에 들어갑니다.
  ((void)insert_initial_unit<Units>(script_group(), SchedulePosition::Auto), ...);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename Unit>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::insert_initial_unit(
    GroupId parent,
    SchedulePosition position) {
  if constexpr (detail::IsScheduledV<Unit>) {
    insert_initial_unit<typename Unit::UnitType>(parent, Unit::PositionValue);
  } else if constexpr (detail::IsSystemGroupV<Unit>) {
    const auto group = create_group(parent, Unit::PolicyValue, position);
    insert_initial_units(group, typename Unit::UnitList{});
  } else {
    (void)insert_system<Unit>(parent, position);
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
template <typename... Units>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::insert_initial_units(
    GroupId parent,
    mp::mp_list<Units...>) {
  ((void)insert_initial_unit<Units>(parent, SchedulePosition::Auto), ...);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
GroupId World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::create_group_node(
    GroupPolicy policy,
    bool fixed) {
  GroupNode node;
  node.policy = policy;
  node.fixed = fixed;
  groups_.push_back(std::move(node));
  return GroupId{groups_.size() - 1};
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
std::size_t World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::create_entry(
    UnitKind kind,
    std::size_t id,
    SchedulePosition position) {
  UnitEntry entry;
  entry.kind = kind;
  entry.id = id;
  entry.position = position;
  entries_.push_back(entry);
  return entries_.size() - 1;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::attach_group_entry(
    std::size_t parentGroup,
    std::size_t childGroup,
    SchedulePosition position) {
  const auto entry = create_entry(UnitKind::Group, childGroup, position);
  groups_[childGroup].parentEntry = entry;
  attach_entry(parentGroup, entry);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::attach_entry(
    std::size_t parentGroup,
    std::size_t entryIndex) {
  auto &parent = groups_[parentGroup];
  auto &entry = entries_[entryIndex];

  entry.parentGroup = parentGroup;
  entry.prev = parent.lastChild;
  entry.next = Npos;

  if (parent.lastChild != Npos) {
    entries_[parent.lastChild].next = entryIndex;
  } else {
    parent.firstChild = entryIndex;
  }

  parent.lastChild = entryIndex;
  mark_ancestors_dirty(parentGroup);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::detach_entry(std::size_t entryIndex) {
  auto &entry = entries_[entryIndex];
  if (entry.parentGroup == Npos) {
    return;
  }

  auto &parent = groups_[entry.parentGroup];
  if (entry.prev != Npos) {
    entries_[entry.prev].next = entry.next;
  } else {
    parent.firstChild = entry.next;
  }

  if (entry.next != Npos) {
    entries_[entry.next].prev = entry.prev;
  } else {
    parent.lastChild = entry.prev;
  }

  const auto oldParent = entry.parentGroup;
  entry.parentGroup = Npos;
  entry.prev = Npos;
  entry.next = Npos;
  mark_ancestors_dirty(oldParent);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::mark_ancestors_dirty(
    std::size_t groupIndex) {
  executionPlanDirty_ = true;

  while (groupIndex != Npos) {
    auto &group = groups_[groupIndex];
    group.dirty = true;
    if (group.parentEntry == Npos) {
      break;
    }
    groupIndex = entries_[group.parentEntry].parentGroup;
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::valid_group(GroupId group) const {
  return group.is_valid() && group.value < groups_.size() && groups_[group.value].active;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::is_group_ancestor_or_self(
    std::size_t ancestor,
    std::size_t child) const {
  std::size_t current = child;
  while (current != Npos) {
    if (current == ancestor) {
      return true;
    }
    const auto parentEntry = groups_[current].parentEntry;
    if (parentEntry == Npos) {
      break;
    }
    current = entries_[parentEntry].parentGroup;
  }
  return false;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::insert_system_by_index(
    std::size_t systemIndex,
    GroupId parent,
    SchedulePosition position) {
  if (!valid_group(parent) || systemIndex >= SystemCount || systemEntries_[systemIndex] != Npos) {
    return false;
  }

  const auto entry = create_entry(UnitKind::System, systemIndex, position);
  systemEntries_[systemIndex] = entry;
  attach_entry(parent.value, entry);
  return true;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::remove_system_by_index(
    std::size_t systemIndex) {
  if (systemIndex >= SystemCount || systemEntries_[systemIndex] == Npos) {
    return false;
  }

  const auto entry = systemEntries_[systemIndex];
  detach_entry(entry);
  entries_[entry].active = false;
  systemEntries_[systemIndex] = Npos;
  return true;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
bool World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::move_system_by_index(
    std::size_t systemIndex,
    GroupId newParent) {
  if (!valid_group(newParent) || systemIndex >= SystemCount || systemEntries_[systemIndex] == Npos) {
    return false;
  }

  const auto entry = systemEntries_[systemIndex];
  detach_entry(entry);
  attach_entry(newParent.value, entry);
  return true;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::destroy_group_contents(
    std::size_t groupIndex) {
  auto &group = groups_[groupIndex];
  auto entryIndex = group.firstChild;

  while (entryIndex != Npos) {
    auto &entry = entries_[entryIndex];
    const auto nextEntry = entry.next;

    if (entry.kind == UnitKind::System) {
      systemEntries_[entry.id] = Npos;
    } else {
      destroy_group_contents(entry.id);
      groups_[entry.id].active = false;
      groups_[entry.id].firstChild = Npos;
      groups_[entry.id].lastChild = Npos;
      groups_[entry.id].parentEntry = Npos;
      groups_[entry.id].plan = {};
    }

    entry.active = false;
    entry.parentGroup = Npos;
    entry.prev = Npos;
    entry.next = Npos;
    entryIndex = nextEntry;
  }

  group.firstChild = Npos;
  group.lastChild = Npos;
  group.dirty = true;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::ensure_execution_plan() {
  if (!executionPlanDirty_) {
    return;
  }

  rebuild_execution_plan();
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::rebuild_execution_plan() {
  rebuild_group(MainGroupIndex);
  executionPlan_ = build_group_execution_plan(MainGroupIndex);
  executionPlanDirty_ = false;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::ExecutionPlan
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::build_group_execution_plan(
    std::size_t groupIndex) const {
  ExecutionPlan plan;
  if (groupIndex >= groups_.size() || !groups_[groupIndex].active) {
    return plan;
  }

  const auto &group = groups_[groupIndex];
  if (group.policy == GroupPolicy::NonParallel) {
    for (auto entry = group.firstChild; entry != Npos; entry = entries_[entry].next) {
      append_execution_plan(plan, build_entry_execution_plan(entry));
    }
    return plan;
  }

  append_execution_plan(plan, build_entries_sequence_execution_plan(group.plan.beforeEntries));
  for (const auto &batch : group.plan.batches) {
    append_execution_plan(plan, build_parallel_batch_execution_plan(batch));
  }
  append_execution_plan(plan, build_entries_sequence_execution_plan(group.plan.afterEntries));
  return plan;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::ExecutionPlan
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::build_entry_execution_plan(
    std::size_t entryIndex) const {
  ExecutionPlan plan;
  if (entryIndex == Npos || entryIndex >= entries_.size()) {
    return plan;
  }

  const auto &entry = entries_[entryIndex];
  if (!entry.active) {
    return plan;
  }

  if (entry.kind == UnitKind::System) {
    plan.steps.push_back(ExecutionStep{{entryIndex}});
    return plan;
  }

  return build_group_execution_plan(entry.id);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::ExecutionPlan
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::build_entries_sequence_execution_plan(
    const std::vector<std::size_t> &entries) const {
  ExecutionPlan plan;
  for (const auto entry : entries) {
    append_execution_plan(plan, build_entry_execution_plan(entry));
  }
  return plan;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::ExecutionPlan
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::build_parallel_batch_execution_plan(
    const std::vector<std::size_t> &batch) const {
  ExecutionPlan plan;
  for (const auto entry : batch) {
    merge_parallel_execution_plan(plan, build_entry_execution_plan(entry));
  }
  return plan;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::append_execution_plan(
    ExecutionPlan &target,
    ExecutionPlan &&source) {
  for (auto &step : source.steps) {
    if (!step.systemEntries.empty()) {
      target.steps.push_back(std::move(step));
    }
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::merge_parallel_execution_plan(
    ExecutionPlan &target,
    ExecutionPlan &&source) {
  if (target.steps.size() < source.steps.size()) {
    target.steps.resize(source.steps.size());
  }

  for (std::size_t index = 0; index < source.steps.size(); ++index) {
    auto &targetEntries = target.steps[index].systemEntries;
    auto &sourceEntries = source.steps[index].systemEntries;
    targetEntries.insert(targetEntries.end(), sourceEntries.begin(), sourceEntries.end());
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::execute_plan() {
  for (const auto &step : executionPlan_.steps) {
    execute_step(step);
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::execute_step(
    const ExecutionStep &step) {
  const auto &entries = step.systemEntries;
  if (entries.empty()) {
    return;
  }

  const auto stepWork = estimate_entries_work(entries);
  if (entries.size() < 2 || stepWork < schedulerConfig_.minParallelWork) {
    execute_system_entries(entries);
    return;
  }

  ensure_thread_pool();
  std::vector<std::function<void()>> tasks;
  tasks.reserve(entries.size());

  for (const auto entry : entries) {
    tasks.push_back([this, entry] { execute_system_entry(entry); });
  }

  threadPool_.run_batch(tasks);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::execute_system_entries(
    const std::vector<std::size_t> &entries) {
  for (const auto entry : entries) {
    execute_system_entry(entry);
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::execute_system_entry(
    std::size_t entryIndex) {
  if (entryIndex == Npos || entryIndex >= entries_.size()) {
    return;
  }

  const auto &entry = entries_[entryIndex];
  if (!entry.active || entry.kind != UnitKind::System) {
    return;
  }

  systemInfos_[entry.id].run(*this);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::ensure_thread_pool() {
  if (threadPool_.worker_count() == 0) {
    threadPool_.configure(detail::resolve_worker_count(schedulerConfig_.workerCount));
  }
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
void World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::rebuild_group(std::size_t groupIndex) {
  if (groupIndex >= groups_.size() || !groups_[groupIndex].active) {
    return;
  }

  // child group의 mask/plan을 먼저 최신화해야 parent group이 정확한 mask를 병합할 수 있습니다.
  for (auto entry = groups_[groupIndex].firstChild; entry != Npos; entry = entries_[entry].next) {
    if (entries_[entry].active && entries_[entry].kind == UnitKind::Group) {
      rebuild_group(entries_[entry].id);
    }
  }

  auto &group = groups_[groupIndex];
  if (!group.dirty) {
    return;
  }

  group.plan = {};
  group.mask = {};

  if (group.policy == GroupPolicy::NonParallel) {
    for (auto entry = group.firstChild; entry != Npos; entry = entries_[entry].next) {
      if (entries_[entry].active) {
        detail::merge_mask(group.mask, entry_mask(entry));
      }
    }
    group.dirty = false;
    return;
  }

  std::vector<SchedulerMask> batchMasks;

  for (auto entry = group.firstChild; entry != Npos; entry = entries_[entry].next) {
    if (!entries_[entry].active) {
      continue;
    }

    const auto mask = entry_mask(entry);
    detail::merge_mask(group.mask, mask);

    if (entries_[entry].position == SchedulePosition::BeforeParallel) {
      group.plan.beforeEntries.push_back(entry);
      continue;
    }

    if (entries_[entry].position == SchedulePosition::AfterParallel) {
      group.plan.afterEntries.push_back(entry);
      continue;
    }

    // greedy batching: 이미 있는 batch 중 충돌하지 않는 첫 batch에 넣고,
    // 모두 충돌하면 새 batch를 만듭니다.
    bool placed = false;
    for (std::size_t batchIndex = 0; batchIndex < batchMasks.size(); ++batchIndex) {
      if (!detail::masks_conflict(batchMasks[batchIndex], mask)) {
        group.plan.batches[batchIndex].push_back(entry);
        detail::merge_mask(batchMasks[batchIndex], mask);
        placed = true;
        break;
      }
    }

    if (!placed) {
      group.plan.batches.push_back(std::vector<std::size_t>{entry});
      batchMasks.push_back(mask);
    }
  }

  group.dirty = false;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
typename World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::SchedulerMask
World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::entry_mask(std::size_t entryIndex) const {
  const auto &entry = entries_[entryIndex];
  if (entry.kind == UnitKind::System) {
    return systemInfos_[entry.id].mask;
  }
  return groups_[entry.id].mask;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
std::size_t World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::estimate_entries_work(
    const std::vector<std::size_t> &entries) const {
  std::size_t total = 0;
  for (const auto entry : entries) {
    total += estimate_entry_work(entry);
  }
  return total;
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
std::size_t World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::estimate_entry_work(
    std::size_t entryIndex) const {
  const auto &entry = entries_[entryIndex];
  if (!entry.active) {
    return 0;
  }

  if (entry.kind == UnitKind::System) {
    return systemInfos_[entry.id].work(*this);
  }

  return estimate_group_work(entry.id);
}

template <typename... RegisteredComponents, typename... RegisteredUnits>
std::size_t World<Components<RegisteredComponents...>, Systems<RegisteredUnits...>>::estimate_group_work(
    std::size_t groupIndex) const {
  std::size_t total = 0;
  for (auto entry = groups_[groupIndex].firstChild; entry != Npos; entry = entries_[entry].next) {
    total += estimate_entry_work(entry);
  }
  return total;
}

} // namespace snapecs
