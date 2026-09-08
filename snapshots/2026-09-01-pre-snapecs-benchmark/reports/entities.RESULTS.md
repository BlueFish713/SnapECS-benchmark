# Results

## TL;DR Results

![Summary SystemsUpdate Plot](img/SystemsUpdate.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

None

None



## Benchmarks

### Create No entities

![CreateNoEntities Plot](img/CreateNoEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._






### Create empty entities

![CreateEmptyEntities Plot](img/CreateEmptyEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                               | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Create   256 (empty) entities | 12us      | 5us    | 2us       | 538us   | 2us        | 10us       | 1us       |
| Create   ~1K (empty) entities | 49us      | 18us   | 7us       | 594us   | 5us        | 33us       | 2us       |
| Create   ~4K (empty) entities | 170us     | 55us   | 49us      | 1003us  | 29us       | 119us      | 6us       |
| Create  ~16K (empty) entities | 742us     | 260us  | 120us     | 1303us  | 105us      | 502us      | 23us      |

|                               | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Create  ~65K (empty) entities | 2ms       | 1ms    | 0ms       | 4ms     | 0ms        | 4ms        | 0ms       |
| Create  262K (empty) entities | 14ms      | 5ms    | 4ms       | 9ms     | 4ms        | 25ms       | 0ms       |
| Create   ~1M (empty) entities | 52ms      | 21ms   | 19ms      | 32ms    | 21ms       | 93ms       | 2ms       |
| Create   ~2M (empty) entities | 120ms     | 48ms   | 42ms      | 72ms    | 29ms       | 222ms      | 5ms       |


### Get No component from Entity

![UnpackNoComponent Plot](img/UnpackNoComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                    | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Get No component in   256 entities | 0us       | 0us    | 0us       | 2us     | 0us        | 0us        | 0us       |
| Get No component in   ~1K entities | 0us       | 0us    | 1us       | 9us     | 0us        | 2us        | 0us       |
| Get No component in   ~4K entities | 0us       | 3us    | 6us       | 39us    | 0us        | 8us        | 2us       |
| Get No component in  ~16K entities | 1us       | 13us   | 24us      | 166us   | 1us        | 35us       | 17us      |

|                                    | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Get No component in  ~65K entities | 0ms       | 0ms    | 0ms       | 0ms     | 0ms        | 0ms        | 0ms       |
| Get No component in  262K entities | 0ms       | 0ms    | 0ms       | 2ms     | 0ms        | 0ms        | 0ms       |
| Get No component in   ~1M entities | 0ms       | 0ms    | 1ms       | 10ms    | 0ms        | 3ms        | 0ms       |
| Get No component in   ~2M entities | 0ms       | 1ms    | 4ms       | 19ms    | 0ms        | 8ms        | 1ms       |


### Create entities

![CreateEntities Plot](img/CreateEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Create   256 entities with two Components | 38us      | 16us   | 32us      | 1047us  | 7us        | 34us       | 7us       |
| Create   ~1K entities with two Components | 139us     | 52us   | 45us      | 1345us  | 20us       | 121us      | 18us      |
| Create   ~4K entities with two Components | 544us     | 148us  | 146us     | 2758us  | 137us      | 486us      | 79us      |
| Create  ~16K entities with two Components | 2317us    | 574us  | 506us     | 7515us  | 492us      | 1949us     | 315us     |

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Create  ~65K entities with two Components | 9ms       | 2ms    | 2ms       | 26ms    | 1ms        | 10ms       | 2ms       |
| Create  262K entities with two Components | 40ms      | 15ms   | 16ms      | 104ms   | 12ms       | 46ms       | 12ms      |
| Create   ~1M entities with two Components | 157ms     | 67ms   | 95ms      | 436ms   | 83ms       | 181ms      | 50ms      |
| Create   ~2M entities with two Components | 320ms     | 143ms  | 184ms     | 774ms   | 138ms      | 391ms      | 91ms      |


### Get one (non-const) component from Entity

![UnpackOneComponent Plot](img/UnpackOneComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                        | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:---------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack one component in   256 entities | 0us       | 0us    | 0us       | 2us     | 0us        | 0us        | 0us       |
| Unpack one component in   ~1K entities | 3us       | 2us    | 2us       | 11us    | 1us        | 2us        | 1us       |
| Unpack one component in   ~4K entities | 12us      | 10us   | 8us       | 42us    | 5us        | 11us       | 6us       |
| Unpack one component in  ~16K entities | 53us      | 41us   | 28us      | 172us   | 19us       | 46us       | 23us      |

|                                        | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:---------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack one component in  ~65K entities | 0ms       | 0ms    | 0ms       | 0ms     | 0ms        | 0ms        | 0ms       |
| Unpack one component in  262K entities | 0ms       | 0ms    | 0ms       | 3ms     | 0ms        | 0ms        | 0ms       |
| Unpack one component in   ~1M entities | 3ms       | 3ms    | 2ms       | 14ms    | 1ms        | 4ms        | 2ms       |
| Unpack one component in   ~2M entities | 7ms       | 6ms    | 5ms       | 24ms    | 2ms        | 10ms       | 4ms       |


### Get two components from entity

![UnpackTwoComponents Plot](img/UnpackTwoComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                         | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:----------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack two components in   256 entities | 1us       | 1us    | 0us       | 9us     | 0us        | 1us        | 0us       |
| Unpack two components in   ~1K entities | 6us       | 5us    | 3us       | 32us    | 2us        | 4us        | 2us       |
| Unpack two components in   ~4K entities | 25us      | 23us   | 13us      | 153us   | 9us        | 18us       | 10us      |
| Unpack two components in  ~16K entities | 100us     | 92us   | 55us      | 616us   | 35us       | 67us       | 40us      |

|                                         | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:----------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack two components in  ~65K entities | 0ms       | 0ms    | 0ms       | 2ms     | 0ms        | 0ms        | 0ms       |
| Unpack two components in  262K entities | 1ms       | 1ms    | 1ms       | 9ms     | 0ms        | 1ms        | 0ms       |
| Unpack two components in   ~1M entities | 6ms       | 6ms    | 4ms       | 38ms    | 2ms        | 6ms        | 3ms       |
| Unpack two components in   ~2M entities | 13ms      | 12ms   | 8ms       | 77ms    | 4ms        | 14ms       | 6ms       |


### Get three components from entity

![UnpackThreeComponents Plot](img/UnpackThreeComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack three components in   512 entities | 3us       | 4us    | 3us       | 42us    | 1us        | 4us        | 2us       |
| Unpack three components in   ~2K entities | 14us      | 17us   | 11us      | 150us   | 6us        | 19us       | 8us       |
| Unpack three components in   ~8K entities | 62us      | 69us   | 53us      | 610us   | 24us       | 70us       | 35us      |

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack three components in  ~32K entities | 0ms       | 0ms    | 0ms       | 2ms     | 0ms        | 0ms        | 0ms       |
| Unpack three components in  131K entities | 1ms       | 1ms    | 1ms       | 10ms    | 0ms        | 1ms        | 0ms       |
| Unpack three components in  524K entities | 4ms       | 5ms    | 4ms       | 41ms    | 1ms        | 6ms        | 2ms       |
| Unpack three components in   ~1M entities | 8ms       | 10ms   | 8ms       | 90ms    | 3ms        | 12ms       | 6ms       |


### Add component

![AddComponent Plot](img/AddComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                   | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Add a Component in   256 entities | 45us      | 55us   | 53us      | 82us    | 54us       | 82us       | 63us      |
| Add a Component in   ~1K entities | 196us     | 183us  | 221us     | 372us   | 231us      | 337us      | 210us     |
| Add a Component in   ~4K entities | 821us     | 819us  | 833us     | 1473us  | 902us      | 1264us     | 886us     |
| Add a Component in  ~16K entities | 3074us    | 3087us | 3294us    | 4163us  | 3649us     | 4847us     | 3687us    |

|                                   | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Add a Component in  ~65K entities | 13ms      | 11ms   | 12ms      | 16ms    | 13ms       | 20ms       | 13ms      |
| Add a Component in  262K entities | 49ms      | 48ms   | 55ms      | 65ms    | 58ms       | 80ms       | 61ms      |
| Add a Component in   ~1M entities | 213ms     | 191ms  | 232ms     | 276ms   | 260ms      | 280ms      | 256ms     |
| Add a Component in   ~2M entities | 415ms     | 355ms  | 485ms     | 561ms   | 478ms      | 630ms      | 499ms     |


### Remove and add component

![RemoveAddComponent Plot](img/RemoveAddComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                              | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:---------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Remove and Add a Component in   256 entities | 9us       | 5us    | 3us       | 51us    | 2us        | 28us       | 1us       |
| Remove and Add a Component in   ~1K entities | 38us      | 20us   | 11us      | 213us   | 8us        | 118us      | 5us       |
| Remove and Add a Component in   ~4K entities | 152us     | 88us   | 48us      | 890us   | 35us       | 456us      | 19us      |
| Remove and Add a Component in  ~16K entities | 649us     | 329us  | 203us     | 3486us  | 148us      | 1772us     | 82us      |

|                                              | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:---------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Remove and Add a Component in  ~65K entities | 2ms       | 1ms    | 0ms       | 14ms    | 0ms        | 7ms        | 0ms       |
| Remove and Add a Component in  262K entities | 9ms       | 5ms    | 3ms       | 51ms    | 2ms        | 30ms       | 1ms       |
| Remove and Add a Component in   ~1M entities | 40ms      | 22ms   | 14ms      | 204ms   | 9ms        | 133ms      | 4ms       |
| Remove and Add a Component in   ~2M entities | 79ms      | 45ms   | 28ms      | 471ms   | 20ms       | 249ms      | 10ms      |


### Destroy entities

![DestroyEntities Plot](img/DestroyEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                            | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   |
|:-------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|
| Destroy   256 entities with two components | 24us      | 11us   | 34us      | 851us   | 3us        | 29us       |
| Destroy   ~1K entities with two components | 89us      | 40us   | 118us     | 902us   | 11us       | 104us      |
| Destroy   ~4K entities with two components | 340us     | 167us  | 480us     | 1023us  | 46us       | 468us      |
| Destroy  ~16K entities with two components | 1480us    | 637us  | 2006us    | 1797us  | 167us      | 2069us     |

|                                            | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   |
|:-------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|
| Destroy  ~65K entities with two components | 8ms       | 2ms    | 8ms       | 5ms     | 0ms        | 10ms       |
| Destroy  262K entities with two components | 30ms      | 10ms   | 34ms      | 17ms    | 4ms        | 47ms       |
| Destroy   ~1M entities with two components | 138ms     | 46ms   | 140ms     | 63ms    | 19ms       | 218ms      |
| Destroy   ~2M entities with two components | 236ms     | 97ms   | 274ms     | 130ms   | 34ms       | 477ms      |


### Create entities at once

![CreateEntitiesInBulk Plot](img/CreateEntitiesInBulk.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                   | EnTT   | Flecs   |
|:--------------------------------------------------|:-------|:--------|
| Create   256 entities with two components at once | 10us   | 973us   |
| Create   ~1K entities with two components at once | 25us   | 1108us  |
| Create   ~4K entities with two components at once | 82us   | 1313us  |
| Create  ~16K entities with two components at once | 339us  | 3439us  |

|                                                   | EnTT   | Flecs   |
|:--------------------------------------------------|:-------|:--------|
| Create  ~65K entities with two components at once | 1ms    | 13ms    |
| Create  262K entities with two components at once | 9ms    | 49ms    |
| Create   ~1M entities with two components at once | 44ms   | 199ms   |
| Create   ~2M entities with two components at once | 100ms  | 361ms   |


### Create empty entities at once

![CreateEmptyEntitiesInBulk Plot](img/CreateEmptyEntitiesInBulk.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                       | EnTT   | Flecs   |
|:--------------------------------------|:-------|:--------|
| Create   256 (empty) entities at once | 3us    | 535us   |
| Create   ~1K (empty) entities at once | 9us    | 643us   |
| Create   ~4K (empty) entities at once | 32us   | 789us   |
| Create  ~16K (empty) entities at once | 130us  | 856us   |

|                                       | EnTT   | Flecs   |
|:--------------------------------------|:-------|:--------|
| Create  ~65K (empty) entities at once | 0ms    | 1ms     |
| Create  262K (empty) entities at once | 3ms    | 3ms     |
| Create   ~1M (empty) entities at once | 14ms   | 14ms    |
| Create   ~2M (empty) entities at once | 30ms   | 35ms    |


### Destroy entities at once

![DestroyEntitiesInBulk Plot](img/DestroyEntitiesInBulk.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                    | EnTT   |
|:---------------------------------------------------|:-------|
| Destroy   256 entities with two components at once | 6us    |
| Destroy   ~1K entities with two components at once | 21us   |
| Destroy   ~4K entities with two components at once | 68us   |
| Destroy  ~16K entities with two components at once | 296us  |

|                                                    | EnTT   |
|:---------------------------------------------------|:-------|
| Destroy  ~65K entities with two components at once | 1ms    |
| Destroy  262K entities with two components at once | 7ms    |
| Destroy   ~1M entities with two components at once | 26ms   |
| Destroy   ~2M entities with two components at once | 60ms   |





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

#### OpenEcs by @Gronis 

> Open Ecs is an Entity Component System that uses metaprogramming, cache coherency, and other useful tricks to maximize performance and configurability. It is written in c++11 without further dependencies.

Version: 0.1.101 (Apr 2017)

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

