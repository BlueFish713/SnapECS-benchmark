# Results

## TL;DR Results

![Summary SystemsUpdate Plot](img/SystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update   256 entities with 2 systems | 5us       | 3us    | 1us       | 3us     | 7us        | 4us        | 3us       |
| Update   ~1K entities with 2 systems | 21us      | 8us    | 6us       | 8us     | 32us       | 15us       | 14us      |
| Update   ~4K entities with 2 systems | 96us      | 35us   | 30us      | 24us    | 78us       | 83us       | 56us      |
| Update  ~16K entities with 2 systems | 351us     | 158us  | 146us     | 143us   | 317us      | 238us      | 223us     |

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update  ~65K entities with 2 systems | 1ms       | 0ms    | 0ms       | 0ms     | 1ms        | 1ms        | 0ms       |
| Update  262K entities with 2 systems | 7ms       | 2ms    | 4ms       | 2ms     | 5ms        | 5ms        | 3ms       |
| Update   ~1M entities with 2 systems | 27ms      | 13ms   | 20ms      | 9ms     | 22ms       | 27ms       | 16ms      |
| Update   ~2M entities with 2 systems | 48ms      | 22ms   | 41ms      | 16ms    | 47ms       | 37ms       | 254ms     |



## Benchmarks

### Update systems (for-each entities in 2 systems)

![SystemsUpdate Plot](img/SystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update   256 entities with 2 systems | 5us       | 3us    | 1us       | 3us     | 7us        | 4us        | 3us       |
| Update   ~1K entities with 2 systems | 21us      | 8us    | 6us       | 8us     | 32us       | 15us       | 14us      |
| Update   ~4K entities with 2 systems | 96us      | 35us   | 30us      | 24us    | 78us       | 83us       | 56us      |
| Update  ~16K entities with 2 systems | 351us     | 158us  | 146us     | 143us   | 317us      | 238us      | 223us     |

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update  ~65K entities with 2 systems | 1ms       | 0ms    | 0ms       | 0ms     | 1ms        | 1ms        | 0ms       |
| Update  262K entities with 2 systems | 7ms       | 2ms    | 4ms       | 2ms     | 5ms        | 5ms        | 3ms       |
| Update   ~1M entities with 2 systems | 27ms      | 13ms   | 20ms      | 9ms     | 22ms       | 27ms       | 16ms      |
| Update   ~2M entities with 2 systems | 48ms      | 22ms   | 41ms      | 16ms    | 47ms       | 37ms       | 254ms     |


### Update systems (for-each entities (with mixed components) in 2 systems)

![SystemsUpdateMixedEntities Plot](img/SystemsUpdateMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update   256 entities with 2 systems | 4us       | 1us    | 4us       | 2us     | 7us        | 3us        | 3us       |
| Update   ~1K entities with 2 systems | 20us      | 9us    | 19us      | 6us     | 20us       | 14us       | 17us      |
| Update   ~4K entities with 2 systems | 86us      | 34us   | 65us      | 24us    | 127us      | 29us       | 59us      |
| Update  ~16K entities with 2 systems | 330us     | 124us  | 218us     | 88us    | 543us      | 92us       | 318us     |

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update  ~65K entities with 2 systems | 1ms       | 0ms    | 0ms       | 0ms     | 1ms        | 0ms        | 0ms       |
| Update  262K entities with 2 systems | 5ms       | 2ms    | 4ms       | 1ms     | 6ms        | 2ms        | 2ms       |
| Update   ~1M entities with 2 systems | 21ms      | 11ms   | 14ms      | 12ms    | 25ms       | 12ms       | 14ms      |
| Update   ~2M entities with 2 systems | 51ms      | 24ms   | 29ms      | 27ms    | 70ms       | 24ms       | 37ms      |





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

