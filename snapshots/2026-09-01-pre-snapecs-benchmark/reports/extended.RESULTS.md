# Results

## TL;DR Results

![Summary SystemsUpdate Plot](img/SystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update   256 entities with 7 systems | 11us   | 48us             | 13us           | 17us            | 12us      | 4us     | 6us        | 5us        | 4us              | 10us      |
| Update   ~1K entities with 7 systems | 66us   | 210us            | 54us           | 87us            | 55us      | 11us    | 25us       | 21us       | 22us             | 39us      |
| Update   ~4K entities with 7 systems | 357us  | 798us            | 307us          | 355us           | 207us     | 39us    | 85us       | 128us      | 114us            | 183us     |
| Update  ~16K entities with 7 systems | 2204us | 2652us           | 1076us         | 1726us          | 832us     | 159us   | 616us      | 593us      | 578us            | 732us     |

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update  ~65K entities with 7 systems | 8ms    | 9ms              | 5ms            | 6ms             | 3ms       | 0ms     | 1ms        | 2ms        | 2ms              | 3ms       |
| Update  262K entities with 7 systems | 36ms   | 52ms             | 21ms           | 27ms            | 17ms      | 4ms     | 7ms        | 10ms       | 11ms             | 13ms      |
| Update   ~1M entities with 7 systems | 167ms  | 218ms            | 110ms          | 108ms           | 74ms      | 18ms    | 27ms       | 52ms       | 52ms             | 311ms     |
| Update   ~2M entities with 7 systems | 297ms  | 348ms            | 209ms          | 220ms           | 149ms     | 38ms    | 78ms       | 90ms       | 87ms             | 1283ms    |



## Benchmarks

### Update systems (for-each entities in 7 systems)

![ComplexSystemsUpdate Plot](img/ComplexSystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update   256 entities with 7 systems | 11us   | 48us             | 13us           | 17us            | 12us      | 4us     | 6us        | 5us        | 4us              | 10us      |
| Update   ~1K entities with 7 systems | 66us   | 210us            | 54us           | 87us            | 55us      | 11us    | 25us       | 21us       | 22us             | 39us      |
| Update   ~4K entities with 7 systems | 357us  | 798us            | 307us          | 355us           | 207us     | 39us    | 85us       | 128us      | 114us            | 183us     |
| Update  ~16K entities with 7 systems | 2204us | 2652us           | 1076us         | 1726us          | 832us     | 159us   | 616us      | 593us      | 578us            | 732us     |

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update  ~65K entities with 7 systems | 8ms    | 9ms              | 5ms            | 6ms             | 3ms       | 0ms     | 1ms        | 2ms        | 2ms              | 3ms       |
| Update  262K entities with 7 systems | 36ms   | 52ms             | 21ms           | 27ms            | 17ms      | 4ms     | 7ms        | 10ms       | 11ms             | 13ms      |
| Update   ~1M entities with 7 systems | 167ms  | 218ms            | 110ms          | 108ms           | 74ms      | 18ms    | 27ms       | 52ms       | 52ms             | 311ms     |
| Update   ~2M entities with 7 systems | 297ms  | 348ms            | 209ms          | 220ms           | 149ms     | 38ms    | 78ms       | 90ms       | 87ms             | 1283ms    |


### Update systems (for-each entities (with mixed components) in 7 systems)

![ComplexSystemsUpdateMixedEntities Plot](img/ComplexSystemsUpdateMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update   256 entities with 7 Systems | 8us    | 36us             | 7us            | 9us             | 9us       | 4us     | 6us        | 2us        | 2us              | 6us       |
| Update   ~1K entities with 7 Systems | 48us   | 150us            | 34us           | 42us            | 38us      | 10us    | 18us       | 12us       | 12us             | 30us      |
| Update   ~4K entities with 7 Systems | 375us  | 633us            | 162us          | 219us           | 76us      | 29us    | 69us       | 56us       | 68us             | 122us     |
| Update  ~16K entities with 7 Systems | 806us  | 2220us           | 908us          | 886us           | 323us     | 132us   | 359us      | 276us      | 341us            | 553us     |

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update  ~65K entities with 7 Systems | 3ms    | 10ms             | 4ms            | 3ms             | 1ms       | 0ms     | 1ms        | 1ms        | 1ms              | 2ms       |
| Update  262K entities with 7 Systems | 20ms   | 44ms             | 19ms           | 9ms             | 7ms       | 3ms     | 5ms        | 7ms        | 7ms              | 9ms       |
| Update   ~1M entities with 7 Systems | 84ms   | 177ms            | 92ms           | 39ms            | 28ms      | 14ms    | 27ms       | 32ms       | 35ms             | 46ms      |
| Update   ~2M entities with 7 Systems | 104ms  | 351ms            | 257ms          | 78ms            | 56ms      | 32ms    | 45ms       | 60ms       | 70ms             | 577ms     |


### Iterate over entities with one component

![IterateSingleComponent Plot](img/IterateSingleComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                | EnTT   | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-----------------------------------------------|:-------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------------|:----------|
| Iterate over   256 entities with one component | 0us    | 1us              | 0us             | 0us       | 0us     | 0us        | 0us              | 0us       |
| Iterate over   ~1K entities with one component | 0us    | 6us              | 0us             | 1us       | 0us     | 0us        | 0us              | 3us       |
| Iterate over   ~4K entities with one component | 1us    | 25us             | 1us             | 4us       | 0us     | 2us        | 1us              | 12us      |
| Iterate over  ~16K entities with one component | 7us    | 97us             | 7us             | 21us      | 2us     | 10us       | 7us              | 50us      |

|                                                | EnTT   | EnTT (runtime)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-----------------------------------------------|:-------|:-----------------|:----------------|:----------|:--------|:-----------|:-----------------|:----------|
| Iterate over  ~65K entities with one component | 0ms    | 0ms              | 0ms             | 0ms       | 0ms     | 0ms        | 0ms              | 0ms       |
| Iterate over  262K entities with one component | 0ms    | 1ms              | 0ms             | 0ms       | 0ms     | 0ms        | 0ms              | 0ms       |
| Iterate over   ~1M entities with one component | 0ms    | 7ms              | 0ms             | 1ms       | 0ms     | 0ms        | 0ms              | 2ms       |
| Iterate over   ~2M entities with one component | 0ms    | 12ms             | 0ms             | 2ms       | 0ms     | 1ms        | 1ms              | 6ms       |


### Iterate over entities with two components

![IterateTwoComponents Plot](img/IterateTwoComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                 | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:----------|
| Iterate over   256 entities with two components | 0us    | 3us              | 0us                        | 0us                       | 0us                           | 0us             | 0us       | 0us     | 0us        | 0us              | 1us       |
| Iterate over   ~1K entities with two components | 3us    | 12us             | 0us                        | 3us                       | 2us                           | 0us             | 2us       | 0us     | 0us        | 0us              | 5us       |
| Iterate over   ~4K entities with two components | 15us   | 46us             | 3us                        | 15us                      | 8us                           | 0us             | 11us      | 1us     | 2us        | 3us              | 21us      |
| Iterate over  ~16K entities with two components | 63us   | 196us            | 15us                       | 63us                      | 40us                          | 0us             | 45us      | 4us     | 9us        | 12us             | 79us      |

|                                                 | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:----------|
| Iterate over  ~65K entities with two components | 0ms    | 0ms              | 0ms                        | 0ms                       | 0ms                           | 0ms             | 0ms       | 0ms     | 0ms        | 0ms              | 0ms       |
| Iterate over  262K entities with two components | 1ms    | 3ms              | 0ms                        | 1ms                       | 0ms                           | 0ms             | 0ms       | 0ms     | 0ms        | 0ms              | 1ms       |
| Iterate over   ~1M entities with two components | 4ms    | 12ms             | 1ms                        | 4ms                       | 2ms                           | 0ms             | 3ms       | 0ms     | 0ms        | 0ms              | 5ms       |
| Iterate over   ~2M entities with two components | 8ms    | 26ms             | 2ms                        | 7ms                       | 5ms                           | 0ms             | 6ms       | 0ms     | 1ms        | 1ms              | 10ms      |


### Iterate over entities with three components

![IterateThreeComponentsWithMixedEntities Plot](img/IterateThreeComponentsWithMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                   | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:--------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:----------|
| Iterate over   256 entities with three components | 1us    | 3us              | 0us                        | 1us                       | 0us                           | 0us             | 0us       | 0us     | 0us        | 0us              | 1us       |
| Iterate over   ~1K entities with three components | 5us    | 16us             | 1us                        | 4us                       | 2us                           | 0us             | 4us       | 0us     | 0us        | 0us              | 6us       |
| Iterate over   ~4K entities with three components | 21us   | 63us             | 5us                        | 19us                      | 11us                          | 0us             | 17us      | 1us     | 2us        | 2us              | 29us      |
| Iterate over  ~16K entities with three components | 87us   | 265us            | 21us                       | 52us                      | 43us                          | 0us             | 66us      | 5us     | 8us        | 9us              | 114us     |

|                                                   | EnTT   | EnTT (runtime)   | EnTT (full-owning group)   | EnTT (non-owning group)   | EnTT (partial-owning group)   | EnTT (stable)   | Ginseng   | Flecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:--------------------------------------------------|:-------|:-----------------|:---------------------------|:--------------------------|:------------------------------|:----------------|:----------|:--------|:-----------|:-----------------|:----------|
| Iterate over  ~65K entities with three components | 0ms    | 1ms              | 0ms                        | 0ms                       | 0ms                           | 0ms             | 0ms       | 0ms     | 0ms        | 0ms              | 0ms       |
| Iterate over  262K entities with three components | 1ms    | 4ms              | 0ms                        | 0ms                       | 0ms                           | 0ms             | 1ms       | 0ms     | 0ms        | 0ms              | 1ms       |
| Iterate over   ~1M entities with three components | 5ms    | 17ms             | 1ms                        | 5ms                       | 3ms                           | 0ms             | 5ms       | 0ms     | 0ms        | 0ms              | 8ms       |
| Iterate over   ~2M entities with three components | 11ms   | 35ms             | 2ms                        | 10ms                      | 6ms                           | 0ms             | 11ms      | 0ms     | 1ms        | 1ms              | 16ms      |





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

#### SnapECS 

> Local Entity-Component-System implementation using indexed data components and tag components.





### Environment

- **OS:** Windows
- **CPU:** 3.00GHz @ 22Cores
- **RAM:** 31.50GB

