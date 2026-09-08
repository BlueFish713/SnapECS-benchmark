# Results

## TL;DR Results

![Summary SystemsUpdate Plot](img/SystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                             | EnTT    | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:--------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:--------------------|:-------------------|
| Steady update   256 entities with 7 systems | 357us   | 601us            | 224us           | 230us     | 4us     | 58us       | 60us       | 144us            | 119us               | 250us              |
| Steady update   ~1K entities with 7 systems | 1351us  | 2350us           | 851us           | 959us     | 11us    | 223us      | 222us      | 495us            | 485us               | 909us              |
| Steady update   ~4K entities with 7 systems | 5394us  | 9651us           | 3511us          | 3979us    | 39us    | 916us      | 858us      | 2115us           | 1851us              | 3861us             |
| Steady update  ~16K entities with 7 systems | 20892us | 37247us          | 12368us         | 14272us   | 159us   | 3626us     | 3338us     | 9634us           | 6938us              | 14847us            |

|                                             | EnTT   | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:-------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:--------------------|:-------------------|
| Steady update  ~65K entities with 7 systems | 85ms   | 150ms            | 49ms            | 61ms      | 0ms     | 14ms       | 13ms       | 39ms             | 28ms                | 60ms               |
| Steady update  262K entities with 7 systems | 327ms  | 611ms            | 198ms           | 251ms     | 4ms     | 57ms       | 54ms       | 122ms            | 114ms               | 229ms              |
| Steady update   ~1M entities with 7 systems | 1351ms | 2454ms           | 815ms           | 1043ms    | 18ms    | 251ms      | 210ms      | 174ms            | 461ms               | 891ms              |
| Steady update   ~2M entities with 7 systems | 2617ms | 4778ms           | 1596ms          | 1918ms    | 38ms    | 481ms      | 389ms      | 353ms            | 963ms               | 1719ms             |



## Benchmarks

### Steady update after one untimed warm-up (7 systems)

![ComplexSystemsUpdate Plot](img/ComplexSystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                             | EnTT    | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:--------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:--------------------|:-------------------|
| Steady update   256 entities with 7 systems | 357us   | 601us            | 224us           | 230us     | 4us     | 58us       | 60us       | 144us            | 119us               | 250us              |
| Steady update   ~1K entities with 7 systems | 1351us  | 2350us           | 851us           | 959us     | 11us    | 223us      | 222us      | 495us            | 485us               | 909us              |
| Steady update   ~4K entities with 7 systems | 5394us  | 9651us           | 3511us          | 3979us    | 39us    | 916us      | 858us      | 2115us           | 1851us              | 3861us             |
| Steady update  ~16K entities with 7 systems | 20892us | 37247us          | 12368us         | 14272us   | 159us   | 3626us     | 3338us     | 9634us           | 6938us              | 14847us            |

|                                             | EnTT   | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:-------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:--------------------|:-------------------|
| Steady update  ~65K entities with 7 systems | 85ms   | 150ms            | 49ms            | 61ms      | 0ms     | 14ms       | 13ms       | 39ms             | 28ms                | 60ms               |
| Steady update  262K entities with 7 systems | 327ms  | 611ms            | 198ms           | 251ms     | 4ms     | 57ms       | 54ms       | 122ms            | 114ms               | 229ms              |
| Steady update   ~1M entities with 7 systems | 1351ms | 2454ms           | 815ms           | 1043ms    | 18ms    | 251ms      | 210ms      | 174ms            | 461ms               | 891ms              |
| Steady update   ~2M entities with 7 systems | 2617ms | 4778ms           | 1596ms          | 1918ms    | 38ms    | 481ms      | 389ms      | 353ms            | 963ms               | 1719ms             |


### Steady update after one untimed warm-up (7 systems, mixed components)

![ComplexSystemsUpdateMixedEntities Plot](img/ComplexSystemsUpdateMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                             | EnTT    | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:--------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:--------------------|:-------------------|
| Steady update   256 entities with 7 systems | 238us   | 450us            | 108us           | 184us     | 4us     | 45us       | 41us       | 33us             | 85us                | 181us              |
| Steady update   ~1K entities with 7 systems | 949us   | 1779us           | 416us           | 726us     | 10us    | 173us      | 167us      | 132us            | 345us               | 710us              |
| Steady update   ~4K entities with 7 systems | 3907us  | 7091us           | 1616us          | 2735us    | 29us    | 706us      | 636us      | 488us            | 1271us              | 2683us             |
| Steady update  ~16K entities with 7 systems | 14911us | 27037us          | 6415us          | 11063us   | 132us   | 2895us     | 2569us     | 1925us           | 5274us              | 10910us            |

|                                             | EnTT   | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------|:-------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:--------------------|:-------------------|
| Steady update  ~65K entities with 7 systems | 60ms   | 116ms            | 26ms            | 43ms      | 0ms     | 10ms       | 9ms        | 7ms              | 21ms                | 44ms               |
| Steady update  262K entities with 7 systems | 249ms  | 476ms            | 106ms           | 183ms     | 3ms     | 46ms       | 37ms       | 30ms             | 86ms                | 175ms              |
| Steady update   ~1M entities with 7 systems | 1052ms | 1890ms           | 455ms           | 763ms     | 14ms    | 173ms      | 142ms      | 123ms            | 310ms               | 677ms              |
| Steady update   ~2M entities with 7 systems | 2040ms | 3743ms           | 850ms           | 1479ms    | 32ms    | 345ms      | 301ms      | 228ms            | 644ms               | 1377ms             |


### Iterate over entities with one component

![IterateSingleComponent Plot](img/IterateSingleComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                | EnTT   | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-----------------------------------------------|:-------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------------|:--------------------|:-------------------|
| Iterate over   256 entities with one component | 4us    | 59us             | 4us             | 14us      | 0us     | 2us        | 1us              | 7us                 | 4us                |
| Iterate over   ~1K entities with one component | 16us   | 230us            | 15us            | 58us      | 0us     | 8us        | 7us              | 28us                | 18us               |
| Iterate over   ~4K entities with one component | 65us   | 889us            | 64us            | 221us     | 0us     | 32us       | 28us             | 108us               | 76us               |
| Iterate over  ~16K entities with one component | 270us  | 3797us           | 259us           | 910us     | 2us     | 132us      | 108us            | 463us               | 313us              |

|                                                | EnTT   | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-----------------------------------------------|:-------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------------|:--------------------|:-------------------|
| Iterate over  ~65K entities with one component | 1ms    | 17ms             | 0ms             | 3ms       | 0ms     | 0ms        | 0ms              | 1ms                 | 1ms                |
| Iterate over  262K entities with one component | 3ms    | 62ms             | 3ms             | 14ms      | 0ms     | 2ms        | 1ms              | 7ms                 | 4ms                |
| Iterate over   ~1M entities with one component | 15ms   | 219ms            | 15ms            | 59ms      | 0ms     | 9ms        | 7ms              | 29ms                | 18ms               |
| Iterate over   ~2M entities with one component | 30ms   | 438ms            | 32ms            | 113ms     | 0ms     | 17ms       | 14ms             | 56ms                | 37ms               |


### Iterate over entities with two components

![IterateTwoComponents Plot](img/IterateTwoComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                 | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:--------------------|:-------------------|
| Iterate over   256 entities with two components | 54us   | 102us            | 40us                       | 45us                      | 45us                          | 0us             | 29us      | 0us     | 5us        | 4us              | 12us                | 6us                |
| Iterate over   ~1K entities with two components | 208us  | 395us            | 159us                      | 180us                     | 185us                         | 0us             | 121us     | 0us     | 18us       | 14us             | 52us                | 28us               |
| Iterate over   ~4K entities with two components | 794us  | 1595us           | 627us                      | 736us                     | 704us                         | 0us             | 485us     | 1us     | 75us       | 55us             | 202us               | 112us              |
| Iterate over  ~16K entities with two components | 3262us | 6944us           | 2564us                     | 2827us                    | 2946us                        | 0us             | 1843us    | 4us     | 299us      | 228us            | 854us               | 429us              |

|                                                 | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:--------------------|:-------------------|
| Iterate over  ~65K entities with two components | 13ms   | 26ms             | 10ms                       | 10ms                      | 12ms                          | 0ms             | 7ms       | 0ms     | 1ms        | 0ms              | 3ms                 | 1ms                |
| Iterate over  262K entities with two components | 54ms   | 106ms            | 38ms                       | 47ms                      | 54ms                          | 0ms             | 29ms      | 0ms     | 4ms        | 3ms              | 13ms                | 7ms                |
| Iterate over   ~1M entities with two components | 210ms  | 444ms            | 155ms                      | 191ms                     | 198ms                         | 0ms             | 135ms     | 0ms     | 19ms       | 15ms             | 54ms                | 28ms               |
| Iterate over   ~2M entities with two components | 444ms  | 823ms            | 283ms                      | 392ms                     | 378ms                         | 0ms             | 240ms     | 0ms     | 40ms       | 32ms             | 108ms               | 57ms               |


### Iterate over entities with three components

![IterateThreeComponentsWithMixedEntities Plot](img/IterateThreeComponentsWithMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                   | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:--------------------|:-------------------|
| Iterate over   256 entities with three components | 77us   | 152us            | 51us                       | 66us                      | 59us                          | 0us             | 46us      | 0us     | 19us       | 5us              | 20us                | 9us                |
| Iterate over   ~1K entities with three components | 296us  | 565us            | 213us                      | 269us                     | 259us                         | 0us             | 181us     | 0us     | 84us       | 20us             | 82us                | 40us               |
| Iterate over   ~4K entities with three components | 1178us | 2344us           | 821us                      | 1111us                    | 971us                         | 0us             | 713us     | 1us     | 310us      | 77us             | 339us               | 143us              |
| Iterate over  ~16K entities with three components | 4876us | 9559us           | 3344us                     | 4570us                    | 4119us                        | 0us             | 3446us    | 5us     | 1309us     | 306us            | 1302us              | 588us              |

|                                                   | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:--------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:--------------------|:-------------------|
| Iterate over  ~65K entities with three components | 19ms   | 38ms             | 12ms                       | 18ms                      | 15ms                          | 0ms             | 12ms      | 0ms     | 5ms        | 1ms              | 5ms                 | 2ms                |
| Iterate over  262K entities with three components | 76ms   | 155ms            | 57ms                       | 70ms                      | 69ms                          | 0ms             | 49ms      | 0ms     | 21ms       | 5ms              | 21ms                | 10ms               |
| Iterate over   ~1M entities with three components | 317ms  | 665ms            | 256ms                      | 273ms                     | 267ms                         | 0ms             | 219ms     | 0ms     | 86ms       | 21ms             | 86ms                | 41ms               |
| Iterate over   ~2M entities with three components | 600ms  | 1268ms           | 441ms                      | 585ms                     | 489ms                         | 0ms             | 407ms     | 0ms     | 171ms      | 44ms             | 175ms               | 78ms               |


### First update: cold view build and one system-logic pass (7 systems)

![ComplexSystemsFirstUpdate Plot](img/ComplexSystemsFirstUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                            | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------|:--------------------|:-------------------|
| First update   256 entities with 7 systems | 559us               | 567us              |
| First update   ~1K entities with 7 systems | 1160us              | 2498us             |
| First update   ~4K entities with 7 systems | 4986us              | 9537us             |
| First update  ~16K entities with 7 systems | 25518us             | 34992us            |

|                                            | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------|:--------------------|:-------------------|
| First update  ~65K entities with 7 systems | 80ms                | 142ms              |
| First update  262K entities with 7 systems | 331ms               | 598ms              |
| First update   ~1M entities with 7 systems | 1352ms              | 2442ms             |
| First update   ~2M entities with 7 systems | 2948ms              | 4888ms             |


### First update: cold view build and one system-logic pass (7 systems, mixed components)

![ComplexSystemsFirstUpdateMixedEntities Plot](img/ComplexSystemsFirstUpdateMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                  | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------------|:--------------------|:-------------------|
| First update   256 mixed entities with 7 systems | 252us               | 466us              |
| First update   ~1K mixed entities with 7 systems | 887us               | 1567us             |
| First update   ~4K mixed entities with 7 systems | 4828us              | 6171us             |
| First update  ~16K mixed entities with 7 systems | 14890us             | 25086us            |

|                                                  | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-------------------------------------------------|:--------------------|:-------------------|
| First update  ~65K mixed entities with 7 systems | 64ms                | 110ms              |
| First update  262K mixed entities with 7 systems | 241ms               | 442ms              |
| First update   ~1M mixed entities with 7 systems | 938ms               | 1806ms             |
| First update   ~2M mixed entities with 7 systems | 1903ms              | 3619ms             |





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

