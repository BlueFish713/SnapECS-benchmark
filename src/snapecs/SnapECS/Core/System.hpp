#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <SnapECS/Core/Component.hpp>
#include <SnapECS/Core/Query.hpp>

namespace snapecs {

template <typename... SystemTs> struct Systems {
    using List = mp::mp_list<SystemTs...>;
};

// 시스템 내부에 구조 변경 코드(component add/remove 및 entity 추가)가 없음을 의미
struct NoStructuralChange {
    using ComponentList = mp::mp_list<>;
    static constexpr bool CreatesEntities = false;
};

// Create Entity 코드가 존재함을 의미
struct CreateEntity {};

template <typename... Terms> struct StructuralChange {
    using TermList = mp::mp_unique<mp::mp_list<Terms...>>;       // 인자로 받은 Term을 List로 저장
    using ComponentList = mp::mp_remove<TermList, CreateEntity>; // {TermList} - CreateEntity
    static constexpr bool CreatesEntities = mp::mp_contains<TermList, CreateEntity>::value;
};

// Read
template <typename... ComponentTs> struct Read {
    static_assert((!IsTagComponentV<ComponentTs> && ...), "Tag components can only be used in Query, not Read.");
    using List = mp::mp_unique<mp::mp_list<ComponentTs...>>;
};

// Write
template <typename... ComponentTs> struct Write {
    static_assert((!IsTagComponentV<ComponentTs> && ...), "Tag components can only be used in Query, not Write.");
    using List = mp::mp_unique<mp::mp_list<ComponentTs...>>;
};

template <typename StructuralChangeT> class SystemWorld;

namespace detail {

// 구조 변경 연산자
template <typename Component> struct StructuralComponentOperations {
    void (*add)(void *, Entity, Component &&) = nullptr;
    void (*remove)(void *, Entity) = nullptr;
};

// 기본 템플릿
template <typename ComponentList, bool CreatesEntities> class SystemWorldStorage;

// 부분 특수화
template <typename... Components, bool CreatesEntities> class SystemWorldStorage<mp::mp_list<Components...>, CreatesEntities> {
  public:
    template <typename WorldT> explicit SystemWorldStorage(WorldT &world) : world_(static_cast<void *>(&world)), createEntity_(+[](void *rawWorld, bool active) -> Entity { return static_cast<WorldT *>(rawWorld)->create_entity(active); }), componentOperations_(make_component_operations<Components, WorldT>()...) {}

    Entity create_entity(bool active = true)
        requires(CreatesEntities)
    {
        return createEntity_(world_, active);
    }

    template <typename Component, typename... Args>
        requires((std::is_same_v<Component, Components> || ...))
    void add_component(Entity entity, Args &&...args) {
        Component value{std::forward<Args>(args)...};
        operations<Component>().add(world_, entity, std::move(value));
    }

    template <typename Component>
        requires((std::is_same_v<Component, Components> || ...))
    void remove_component(Entity entity) {
        operations<Component>().remove(world_, entity);
    }

  private:
    template <typename Component, typename WorldT> static StructuralComponentOperations<Component> make_component_operations() {
        return {+[](void *rawWorld, Entity entity, Component &&value) { static_cast<WorldT *>(rawWorld)->template add_component<Component>(entity, std::move(value)); }, +[](void *rawWorld, Entity entity) { static_cast<WorldT *>(rawWorld)->template remove_component<Component>(entity); }};
    }

    template <typename Component> StructuralComponentOperations<Component> &operations() {
        return std::get<StructuralComponentOperations<Component>>(componentOperations_);
    }

    void *world_ = nullptr;
    Entity (*createEntity_)(void *, bool) = nullptr;
    std::tuple<StructuralComponentOperations<Components>...> componentOperations_;
};

} // namespace detail

template <typename... Terms> class SystemWorld<StructuralChange<Terms...>> : public detail::SystemWorldStorage<typename StructuralChange<Terms...>::ComponentList, StructuralChange<Terms...>::CreatesEntities> {
    using Base = detail::SystemWorldStorage<typename StructuralChange<Terms...>::ComponentList, StructuralChange<Terms...>::CreatesEntities>;

  public:
    using Base::Base;
};

namespace detail {

template <typename ReadList, typename WriteList> struct ReadOnlyComponents;

template <typename... ReadComponents, typename WriteList> struct ReadOnlyComponents<mp::mp_list<ReadComponents...>, WriteList> {
    using Type = mp::mp_append<mp::mp_list<>, std::conditional_t<mp::mp_contains<WriteList, ReadComponents>::value, mp::mp_list<>, mp::mp_list<ReadComponents>>...>;
};

template <typename ReadList, typename WriteList> using ReadOnlyComponentsT = typename ReadOnlyComponents<ReadList, WriteList>::Type;

template <typename Component> using IsSingleComponentP = std::bool_constant<IsSingleComponentV<Component>>;

template <typename Component> using IsEntityComponentP = std::bool_constant<!IsSingleComponentV<Component>>;

template <typename QueryT, typename ReadT, typename WriteT, typename StructuralChangeT, typename EntityReadOnlyList, typename EntityWriteList> class SystemUpdateInterface;

template <typename QueryT, typename ReadT, typename WriteT, typename... ReadComponentsT, typename... WriteComponentsT> class SystemUpdateInterface<QueryT, ReadT, WriteT, NoStructuralChange, mp::mp_list<ReadComponentsT...>, mp::mp_list<WriteComponentsT...>> {
  public:
    using QueryType = QueryT;
    using ReadType = ReadT;
    using WriteType = WriteT;
    using StructuralChangeType = NoStructuralChange;

    virtual ~SystemUpdateInterface() = default;

    virtual void Update(Entity entity, const ReadComponentsT &...readComponents, WriteComponentsT &...writeComponents) = 0;
};

template <typename QueryT, typename ReadT, typename WriteT, typename... Terms, typename... ReadComponentsT, typename... WriteComponentsT> class SystemUpdateInterface<QueryT, ReadT, WriteT, StructuralChange<Terms...>, mp::mp_list<ReadComponentsT...>, mp::mp_list<WriteComponentsT...>> {
  public:
    using QueryType = QueryT;
    using ReadType = ReadT;
    using WriteType = WriteT;
    using StructuralChangeType = StructuralChange<Terms...>;
    using WorldContext = SystemWorld<StructuralChangeType>;

    virtual ~SystemUpdateInterface() = default;

    virtual void Update(WorldContext &world, Entity entity, const ReadComponentsT &...readComponents, WriteComponentsT &...writeComponents) = 0;
};

} // namespace detail

template <typename QueryT, typename ReadT, typename WriteT, typename StructuralChangeT = NoStructuralChange> class System : public detail::SystemUpdateInterface<QueryT, ReadT, WriteT, StructuralChangeT, mp::mp_copy_if<detail::ReadOnlyComponentsT<typename ReadT::List, typename WriteT::List>, detail::IsEntityComponentP>, mp::mp_copy_if<typename WriteT::List, detail::IsEntityComponentP>> {
    using ReadOnlyList = detail::ReadOnlyComponentsT<typename ReadT::List, typename WriteT::List>;

  public:
    using EntityReadOnlyList = mp::mp_copy_if<ReadOnlyList, detail::IsEntityComponentP>;
    using EntityWriteList = mp::mp_copy_if<typename WriteT::List, detail::IsEntityComponentP>;
    using SingletonReadOnlyList = mp::mp_copy_if<ReadOnlyList, detail::IsSingleComponentP>;
    using SingletonWriteList = mp::mp_copy_if<typename WriteT::List, detail::IsSingleComponentP>;
};

} // namespace snapecs
