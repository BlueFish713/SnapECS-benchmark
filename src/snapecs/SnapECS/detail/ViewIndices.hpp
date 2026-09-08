#pragma once

#include <SnapECS/detail/AccessSignature.hpp>

namespace snapecs::detail {

// 연관된 view index
template <typename Component, typename SignatureList>
struct RelatedViewIndices {
private:
  template <typename Index>
  using IsRelated =
      SignatureRelatedTo<mp::mp_at_c<SignatureList, Index::value>, Component>;

public:
  using Type = mp::mp_copy_if<mp::mp_iota_c<mp::mp_size<SignatureList>::value>,
                              IsRelated>;
};

template <typename Component, typename SignatureList>
using RelatedViewIndicesT =
    typename RelatedViewIndices<Component, SignatureList>::Type;

// 특정 cached Component 값이 바뀔 때 복사본 동기화 후보가 되는 view index
template <typename Component, typename SignatureList>
struct CachedSyncViewIndices {
private:
  template <typename Index>
  using StoresCached =
      SignatureStoresCached<mp::mp_at_c<SignatureList, Index::value>,
                            Component>;

public:
  using Type = mp::mp_copy_if<mp::mp_iota_c<mp::mp_size<SignatureList>::value>,
                              StoresCached>;
};

template <typename Component, typename SignatureList>
using CachedSyncViewIndicesT =
    typename CachedSyncViewIndices<Component, SignatureList>::Type;

} // namespace snapecs::detail
