#pragma once

#include <type_traits>

#include <SnapECS/Core/System.hpp>
#include <SnapECS/detail/MetaBasics.hpp>
#include <SnapECS/detail/MetaQuery.hpp>

namespace snapecs::detail {

// Access Slot은 특정 시스템에 대한 등록된 컴포넌트의 접근 권한을 표시합니다.
template <typename Component, bool Admission, bool Eviction, bool ReadAccess, bool WriteAccess>
struct AccessSlot {
    using ComponentType = Component;

    // 이 컴포넌트가 있어야 함
    static constexpr bool AdmissionRequired = Admission;

    // 이 컴포넌트가 있으면 안됨.
    static constexpr bool EvictionRequired = Eviction;

    // 이 컴포넌트가 연관되어 있음(<=>이 컴포넌트의 add/remove가 해당 view를 바꿀 수 있음)
    static constexpr bool Related = AdmissionRequired || EvictionRequired;

    // Reads/Writes
    static constexpr bool Reads = ReadAccess;
    static constexpr bool Writes = WriteAccess;

    // 데이터에 실제로 접근하는지 여부
    static constexpr bool Accessed = ReadAccess || WriteAccess;

    // cached component
    static constexpr bool StoresCached = Accessed && IsCachedComponentV<Component>;

    // indexed component
    static constexpr bool StoresIndexed = Accessed && IsIndexedComponentV<Component>;
};

// 한 system의 전체 AccessSlot list
template <typename... Slots>
struct AccessSignature {
    using SlotsList = mp::mp_list<Slots...>;
};

// alias template
template <typename Slot>
using SlotComponentT = typename Slot::ComponentType;

// alias template
template <typename Slot>
using SlotStoresCachedP = std::bool_constant<Slot::StoresCached>;

// alias template
template <typename Slot>
using SlotStoresIndexedP = std::bool_constant<Slot::StoresIndexed>;

// alias template
template <typename Slot>
using SlotWritesCachedP =
    std::bool_constant<Slot::Writes &&
                       IsCachedComponentV<typename Slot::ComponentType>>;

// 두 AccessSlot이 read/write 충돌하는지 계산
template <typename SlotA, typename SlotB>
struct AccessSlotsConflict
    : std::bool_constant<
          std::is_same_v<typename SlotA::ComponentType, typename SlotB::ComponentType> &&
          ((SlotA::Writes && (SlotB::Reads || SlotB::Writes)) ||
           (SlotB::Writes && SlotA::Reads))> {};

// AccessSlot이 AccessSignature와 충돌하는지 확인
// 기본 템플릿
template <typename Slot, typename Signature>
struct AccessSlotConflictsWithSignature;

// 특수 템플릿
template <typename Slot, typename... Slots>
struct AccessSlotConflictsWithSignature<Slot, AccessSignature<Slots...>>
    : std::bool_constant<(false || ... || AccessSlotsConflict<Slot, Slots>::value)> {};

// 두 AccessSignature가 충돌하는지 확인
// 기본 템플릿
template <typename SignatureA, typename SignatureB>
struct AccessSignaturesConflict;

// 특수 템플릿
template <typename... SlotsA, typename SignatureB>
struct AccessSignaturesConflict<AccessSignature<SlotsA...>, SignatureB>
    : std::bool_constant<
          (false || ... || AccessSlotConflictsWithSignature<SlotsA, SignatureB>::value)> {};

// AccessSignature 내에 있는 CachedComponent
// 기본 템플릿
template <typename Signature>
struct SignatureCachedComponents;

// 특수 템플릿
template <typename... Slots>
struct SignatureCachedComponents<AccessSignature<Slots...>> {
    using Type = mp::mp_transform<
        SlotComponentT, mp::mp_copy_if<mp::mp_list<Slots...>, SlotStoresCachedP>>;
};

// alias template
template <typename Signature>
using SignatureCachedComponentsT =
    typename SignatureCachedComponents<Signature>::Type;

// AccessSignature 내에 있는 CachedComponent
// 기본 템플릿
template <typename Signature>
struct SignatureIndexedComponents;

// 특수 템플릿
template <typename... Slots>
struct SignatureIndexedComponents<AccessSignature<Slots...>> {
    using Type =
        mp::mp_transform<SlotComponentT, mp::mp_copy_if<mp::mp_list<Slots...>,
                                                        SlotStoresIndexedP>>;
};

// alias template
template <typename Signature>
using SignatureIndexedComponentsT =
    typename SignatureIndexedComponents<Signature>::Type;

// system이 write하는 CachedComponent
template <typename Signature>
struct SignatureWrittenCachedComponents;

template <typename... Slots>
struct SignatureWrittenCachedComponents<AccessSignature<Slots...>> {
    using Type = mp::mp_transform<
        SlotComponentT, mp::mp_copy_if<mp::mp_list<Slots...>, SlotWritesCachedP>>;
};

template <typename Signature>
using SignatureWrittenCachedComponentsT =
    typename SignatureWrittenCachedComponents<Signature>::Type;

// 특정 signature가 특정 component의 add/remove와 관련있음(연관된 component)
template <typename Signature, typename Component>
struct SignatureRelatedTo;

// 부분 특수화
template <typename... Slots, typename Component>
struct SignatureRelatedTo<AccessSignature<Slots...>, Component>
    : std::bool_constant<(
          (std::is_same_v<typename Slots::ComponentType, Component> &&
           Slots::Related) ||
          ...)> {};

// cached component && signatrure에 포함
template <typename Signature, typename Component>
struct SignatureStoresCached;

template <typename... Slots, typename Component>
struct SignatureStoresCached<AccessSignature<Slots...>, Component>
    : std::bool_constant<(
          (std::is_same_v<typename Slots::ComponentType, Component> &&
           Slots::StoresCached) ||
          ...)> {};

// System 타입을 AccessSignature로 변환
template <typename ComponentList, typename SystemT>
struct MakeAccessSignature;

template <typename... RegisteredComponents, typename SystemT>
struct MakeAccessSignature<mp::mp_list<RegisteredComponents...>, SystemT> {
  private:
    using QueryType = typename SystemT::QueryType;
    using ReadType = typename SystemT::ReadType;
    using WriteType = typename SystemT::WriteType;
    using QueryIncludeList = QueryIncludesT<QueryType>;
    using QueryExcludeList = QueryExcludesT<QueryType>;
    using ReadList = typename ReadType::List;
    using WriteList = typename WriteType::List;

    // 등록된 컴포넌트 하나를 AccessSlot으로 바꿈
    template <typename Component>
    using Slot = AccessSlot<
        Component,

        // Admission
        ContainsV<QueryIncludeList, Component> ||
            (!IsSingleComponentV<Component> &&
             (ContainsV<ReadList, Component> || ContainsV<WriteList, Component>)),

        // Eviction
        ContainsV<QueryExcludeList, Component>,

        // Read Access
        ContainsV<ReadList, Component> && !ContainsV<WriteList, Component>,

        // Write Access
        ContainsV<WriteList, Component>>;

  public:
    // AccessSignature 는 AccessSlot의 묶음
    using Type = AccessSignature<Slot<RegisteredComponents>...>;
};

// View signature는 entity별 접근만 남깁니다. SingleComponent 접근 차이는
// scheduler signature에는 남지만 SystemViewTable의 저장/멤버십을 나누지 않습니다.
template <typename ComponentList, typename SystemT>
struct MakeViewSignature;

template <typename... RegisteredComponents, typename SystemT>
struct MakeViewSignature<mp::mp_list<RegisteredComponents...>, SystemT> {
  private:
    using QueryType = typename SystemT::QueryType;
    using QueryIncludeList = QueryIncludesT<QueryType>;
    using QueryExcludeList = QueryExcludesT<QueryType>;
    using EntityReadList = typename SystemT::EntityReadOnlyList;
    using EntityWriteList = typename SystemT::EntityWriteList;

    template <typename Component>
    using Slot = AccessSlot<
        Component,
        ContainsV<QueryIncludeList, Component> ||
            ContainsV<EntityReadList, Component> || ContainsV<EntityWriteList, Component>,
        ContainsV<QueryExcludeList, Component>,
        ContainsV<EntityReadList, Component>,
        ContainsV<EntityWriteList, Component>>;

  public:
    using Type = AccessSignature<Slot<RegisteredComponents>...>;
};

template <typename ComponentList, typename SystemT>
using MakeViewSignatureT = typename MakeViewSignature<ComponentList, SystemT>::Type;

template <typename ComponentList>
struct MakeViewSignatureFor {
    template <typename SystemT>
    using Fn = MakeViewSignatureT<ComponentList, SystemT>;
};

template <typename ComponentList, typename SystemT>
using MakeAccessSignatureT =
    typename MakeAccessSignature<ComponentList, SystemT>::Type;

// mp_transform에 넘기기 위해 ComponentList를 먼저 고정한 메타 함수 wrapper입니다.
template <typename ComponentList>
struct  MakeAccessSignatureFor{
    template <typename SystemT>
    using Fn = MakeAccessSignatureT<ComponentList, SystemT>;
};

template <typename ComponentList>
struct ComponentSetIsUnique;

template <typename... Components>
struct ComponentSetIsUnique<mp::mp_list<Components...>>
    : TypeListIsUnique<mp::mp_list<Components...>> {};

} // namespace snapecs::detail
