# Results

## TL;DR Results

![Summary SystemsUpdate Plot](img/SystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                             | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Steady update   256 entities with 2 systems | 128us     | 70us   | 55us      | 16us    | 17us       | 15us       | 28us                | 54us               |
| Steady update   ~1K entities with 2 systems | 508us     | 339us  | 257us     | 51us    | 71us       | 59us       | 111us               | 228us              |
| Steady update   ~4K entities with 2 systems | 1995us    | 1226us | 814us     | 190us   | 290us      | 225us      | 480us               | 876us              |
| Steady update  ~16K entities with 2 systems | 8367us    | 4961us | 3317us    | 739us   | 1123us     | 944us      | 1908us              | 3471us             |

|                                             | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Steady update  ~65K entities with 2 systems | 33ms      | 20ms   | 13ms      | 2ms     | 4ms        | 3ms        | 6ms                 | 13ms               |
| Steady update  262K entities with 2 systems | 134ms     | 79ms   | 52ms      | 10ms    | 17ms       | 14ms       | 28ms                | 53ms               |
| Steady update   ~1M entities with 2 systems | 508ms     | 324ms  | 231ms     | 44ms    | 72ms       | 62ms       | 111ms               | 219ms              |
| Steady update   ~2M entities with 2 systems | 1018ms    | 600ms  | 463ms     | 92ms    | 147ms      | 120ms      | 224ms               | 427ms              |



## Benchmarks

### Steady update after one untimed warm-up (2 systems)

![SystemsUpdate Plot](img/SystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                             | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Steady update   256 entities with 2 systems | 128us     | 70us   | 55us      | 16us    | 17us       | 15us       | 28us                | 54us               |
| Steady update   ~1K entities with 2 systems | 508us     | 339us  | 257us     | 51us    | 71us       | 59us       | 111us               | 228us              |
| Steady update   ~4K entities with 2 systems | 1995us    | 1226us | 814us     | 190us   | 290us      | 225us      | 480us               | 876us              |
| Steady update  ~16K entities with 2 systems | 8367us    | 4961us | 3317us    | 739us   | 1123us     | 944us      | 1908us              | 3471us             |

|                                             | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Steady update  ~65K entities with 2 systems | 33ms      | 20ms   | 13ms      | 2ms     | 4ms        | 3ms        | 6ms                 | 13ms               |
| Steady update  262K entities with 2 systems | 134ms     | 79ms   | 52ms      | 10ms    | 17ms       | 14ms       | 28ms                | 53ms               |
| Steady update   ~1M entities with 2 systems | 508ms     | 324ms  | 231ms     | 44ms    | 72ms       | 62ms       | 111ms               | 219ms              |
| Steady update   ~2M entities with 2 systems | 1018ms    | 600ms  | 463ms     | 92ms    | 147ms      | 120ms      | 224ms               | 427ms              |


### Steady update after one untimed warm-up (2 systems, mixed components)

![SystemsUpdateMixedEntities Plot](img/SystemsUpdateMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                             | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Steady update   256 entities with 2 systems | 127us     | 79us   | 54us      | 16us    | 16us       | 15us       | 29us                | 55us               |
| Steady update   ~1K entities with 2 systems | 477us     | 318us  | 226us     | 53us    | 69us       | 62us       | 112us               | 206us              |
| Steady update   ~4K entities with 2 systems | 1978us    | 1410us | 911us     | 197us   | 275us      | 233us      | 495us               | 883us              |
| Steady update  ~16K entities with 2 systems | 7719us    | 5721us | 3357us    | 735us   | 1092us     | 866us      | 1827us              | 3321us             |

|                                             | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Steady update  ~65K entities with 2 systems | 29ms      | 19ms   | 13ms      | 2ms     | 4ms        | 3ms        | 7ms                 | 13ms               |
| Steady update  262K entities with 2 systems | 125ms     | 87ms   | 52ms      | 11ms    | 18ms       | 14ms       | 28ms                | 50ms               |
| Steady update   ~1M entities with 2 systems | 495ms     | 325ms  | 234ms     | 43ms    | 75ms       | 63ms       | 112ms               | 211ms              |
| Steady update   ~2M entities with 2 systems | 988ms     | 642ms  | 435ms     | 86ms    | 142ms      | 122ms      | 242ms               | 411ms              |


### First update: cold view build and one system-logic pass (2 systems)

![SystemsFirstUpdate Plot](img/SystemsFirstUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                            | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------|:--------------------|:-------------------|
| First update   256 entities with 2 systems | 95us                | 162us              |
| First update   ~1K entities with 2 systems | 532us               | 540us              |
| First update   ~4K entities with 2 systems | 1407us              | 2259us             |
| First update  ~16K entities with 2 systems | 5971us              | 9296us             |

|                                            | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------|:--------------------|:-------------------|
| First update  ~65K entities with 2 systems | 24ms                | 37ms               |
| First update  262K entities with 2 systems | 87ms                | 156ms              |
| First update   ~1M entities with 2 systems | 383ms               | 636ms              |
| First update   ~2M entities with 2 systems | 830ms               | 1320ms             |


### First update: cold view build and one system-logic pass (2 systems, mixed components)

![SystemsFirstUpdateMixedEntities Plot](img/SystemsFirstUpdateMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                  | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------------|:--------------------|:-------------------|
| First update   256 mixed entities with 2 systems | 98us                | 176us              |
| First update   ~1K mixed entities with 2 systems | 380us               | 625us              |
| First update   ~4K mixed entities with 2 systems | 1726us              | 2281us             |
| First update  ~16K mixed entities with 2 systems | 6032us              | 9550us             |

|                                                  | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------------|:--------------------|:-------------------|
| First update  ~65K mixed entities with 2 systems | 23ms                | 40ms               |
| First update  262K mixed entities with 2 systems | 99ms                | 160ms              |
| First update   ~1M mixed entities with 2 systems | 358ms               | 637ms              |
| First update   ~2M mixed entities with 2 systems | 747ms               | 1238ms             |





### Candidates

#### EntityX by @alecthomas 

> Entity Component Systems (ECS) are a form of decomposition that completely decouples entity logic and data from the entity "objects" themselves. The Evolve your Hierarchy article provides a solid overview of EC systems and why you should use them.

Version: 1.1.2 (Apr 2023)

#### EnTT by @skypjack 

> EnTT is a header-only, tiny and easy to use library for game programming and much more written in modern C++.

Version: v3.13.2

#### Ginseng by @apples 

> Ginseng is an entity-component-system (ECS) library designed for use in games.

The main advantage over similar libraries is that the component types do not need to be listed or registered. Component types are detected dynamically.

Any function-like object can be used as a system. The function's parameters are used to determine the required components.

Version: 1.1 (Dec 2021)

#### Flecs by @SanderMertens 

> Flecs is a fast and lightweight Entity Component System that lets you build games and simulations with millions of entities.

Version: v4.0.1

#### pico_ecs by @empyreanx 

> A collection of cross-platform single header libraries written in C. Pure and simple ECS.

Version: 2.3 (Sep 2023)

#### gaia-ecs by @richardbiely 

> Gaia-ECS is a fast and easy-to-use ECS framework.

Version: v0.8.6

#### SnapECS (Indexed) 

> SnapECS using IndexedComponent storage for data components and TagComponent storage for empty components.


#### SnapECS (Cached) 

> SnapECS using CachedComponent storage for data components and TagComponent storage for empty components.





### Environment

- **OS:** Windows
- **CPU:** 3.00GHz @ 22Cores
- **RAM:** 31.50GB

