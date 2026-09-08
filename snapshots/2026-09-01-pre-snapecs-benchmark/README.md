# Entity-Component-System Benchmarks

[![Actions Status](https://github.com/abeimler/ecs_benchmark/workflows/ci/badge.svg)](https://github.com/abeimler/ecs_benchmark/actions)
![License](https://img.shields.io/github/license/abeimler/ecs_benchmark)
![Standard](https://img.shields.io/badge/c%2B%2B-20-blue)

This repository contains a collection of benchmarks for popular Entity-Component-System (ECS) frameworks.
The benchmarks cover different aspects of ECS frameworks, such as update systems, component additions/removals, and entity creation/destruction.
It's important to note that different ECS frameworks have different strengths and weaknesses.
For example, some frameworks might excel in adding/removing components, while others might be better at creating/destroying entities or have query support.
Therefore, it's crucial to choose an ECS framework based on your specific requirements.

ECS (Entity-Component-System) Frameworks:

* [EntityX](https://github.com/alecthomas/entityx)
* [EnTT](https://github.com/skypjack/entt)
* [Ginseng](https://github.com/apples/ginseng)
* [flecs](https://github.com/SanderMertens/flecs)
* [pico_ecs](https://github.com/empyreanx/pico_headers)
* [gaia-ecs](https://github.com/richardbiely/gaia-ecs)
* SnapECS (local implementation)

mustache is excluded from this report because its full-range Release benchmark corrupts the heap on MinGW at `BM_SystemsUpdate/2048`.

## TL;DR Results

Main feature of an ECS is the iterating over a lot of entities and using system to update the components.
The results of these benchmarks should be used as a starting point for your own benchmarking efforts.

### Update systems (for-each entities (with mixed components) in 7 systems)


![ComplexSystemsUpdateMixedEntities Plot](img/ComplexSystemsUpdateMixedEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update     1 entities with 7 Systems | 213ns  | 186ns            | 236ns          | 225ns           | 113ns     | 1651ns  | 80ns       | 209ns      | 203ns            | 161ns     |
| Update     4 entities with 7 Systems | 344ns  | 522ns            | 305ns          | 307ns           | 211ns     | 2964ns  | 137ns      | 390ns      | 468ns            | 273ns     |
| Update     8 entities with 7 Systems | 570ns  | 1032ns           | 434ns          | 447ns           | 364ns     | 3033ns  | 257ns      | 418ns      | 534ns            | 387ns     |
| Update    16 entities with 7 Systems | 336ns  | 1143ns           | 371ns          | 343ns           | 428ns     | 2800ns  | 316ns      | 356ns      | 367ns            | 443ns     |
| Update    32 entities with 7 Systems | 852ns  | 2376ns           | 543ns          | 446ns           | 824ns     | 2740ns  | 579ns      | 472ns      | 410ns            | 643ns     |
| Update    64 entities with 7 Systems | 1018ns | 4527ns           | 858ns          | 656ns           | 1485ns    | 3392ns  | 1080ns     | 556ns      | 503ns            | 1119ns    |

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



While this benchmark only includes up to 6 components and 7 small systems,
it's important to note that Entity-Component-Systems can become much more complex in the wild,
with hundreds of components and systems.
Therefore, it's crucial to always benchmark your specific cases and systems when necessary and compare results.
Choose an ECS framework based on its features.
For example, EnTT offers [resource management](https://github.com/skypjack/entt/wiki/Crash-Course:-resource-management),
while flecs provides useful [add-ons](https://github.com/SanderMertens/flecs#addons) and [querying](https://github.com/SanderMertens/flecs/tree/master/examples/cpp/queries/basics),
and EntityX includes a built-in [world/system manager](https://github.com/alecthomas/entityx#manager-tying-it-all-together=).

To evaluate a framework, look at the examples and API design, and pick the one that suits your needs.


## Details

### Features

All benchmarks are located in the [`benchmark/benchmarks/`](benchmark/benchmarks/) directory and write with with the [google/benchmark](https://github.com/google/benchmark) library.
Each benchmark uses an example application for each framework (see [`src/`](src) directory), and every example application has specific base features implemented (see [`src/base`](src/base)).

#### Components

1. `PositionComponent`: includes `x` and `y` coordinates.
2. `VelocityComponent`: includes `x` and `y` coordinates for movement.
3. `DataComponent`: includes some arbitrary data.
4. `HealthComponent`: Hero/Monster data includes HP/MaxHP and status.
5. `DamageComponent`: Hero/Monster data includes damage.
6. `SpriteComponent`: Hero/Monster ASCII character as sprite.

#### Systems

1. `MovementSystem`: updates the `PositionComponent` with a constant `VelocityComponent`.
2. `DataSystem`: updates the `DataComponent` with arbitrary data.
3. `MoreComplexSystem`: updates components with random data and arbitrary information.
4. `HealthSystem`: update Hero/Monster health (update HP and status).
5. `DamageSystem`: update Hero/Monster health by taking damage.
6. `SpriteSystem`: update Hero/Monster ASCII character depending on health and type.
7. `RenderSystem`: "render"(write) Hero/Monster character sprite into a "frame buffer"(string buffer).



## Additional Benchmarks

Benchmarks for more common features, such as "Creating entities", "Adding and removing components", and others.

### Features tested

* Entity Creation
* Entity Destruction
* Component Retrieval
* Adding and removing components
* Iterating entities/quires


### Environment

- **OS:** Windows
- **CPU:** 3.00GHz @ 22Cores
- **RAM:** 31.50GB


---

### Create entities


![CreateEntities Plot](img/CreateEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                           | EntityX   | EnTT   | Ginseng   | Flecs    | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------------------|:----------|:-------|:----------|:---------|:-----------|:-----------|:----------|
| Create     1 entities with two Components | 2414ns    | 4551ns | 16590ns   | 958872ns | 1779ns     | 5875ns     | 819ns     |
| Create     4 entities with two Components | 2836ns    | 6685ns | 16980ns   | 899116ns | 1900ns     | 6490ns     | 1720ns    |
| Create     8 entities with two Components | 3500ns    | 7098ns | 17331ns   | 964219ns | 1781ns     | 7190ns     | 2207ns    |
| Create    16 entities with two Components | 5256ns    | 8016ns | 18588ns   | 679468ns | 1985ns     | 7724ns     | 2709ns    |
| Create    32 entities with two Components | 6868ns    | 5894ns | 19571ns   | 643483ns | 2434ns     | 10382ns    | 3283ns    |
| Create    64 entities with two Components | 11484ns   | 7937ns | 19817ns   | 939354ns | 3508ns     | 13763ns    | 4348ns    |

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



### Destroy entities


![DestroyEntities Plot](img/DestroyEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                            | EntityX   | EnTT   | Ginseng   | Flecs    | pico_ecs   | gaia-ecs   |
|:-------------------------------------------|:----------|:-------|:----------|:---------|:-----------|:-----------|
| Destroy     1 entities with two components | 1225ns    | 819ns  | 1753ns    | 779606ns | 1336ns     | 3853ns     |
| Destroy     4 entities with two components | 1019ns    | 909ns  | 2094ns    | 779740ns | 1449ns     | 4010ns     |
| Destroy     8 entities with two components | 1539ns    | 1062ns | 2740ns    | 802770ns | 1549ns     | 4272ns     |
| Destroy    16 entities with two components | 3005ns    | 1559ns | 3840ns    | 861282ns | 1640ns     | 5375ns     |
| Destroy    32 entities with two components | 5234ns    | 2097ns | 5522ns    | 839476ns | 1940ns     | 7157ns     |
| Destroy    64 entities with two components | 6949ns    | 3268ns | 9540ns    | 783006ns | 1996ns     | 10631ns    |

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


**Note:** SnapECS is omitted from entity destruction and bulk-operation results because those APIs are not supported.


### Get one component from entity


![UnpackOneComponent Plot](img/UnpackOneComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                        | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:---------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack one component in     1 entities | 3ns       | 2ns    | 1ns       | 11ns    | 1ns        | 2ns        | 1ns       |
| Unpack one component in     4 entities | 13ns      | 10ns   | 6ns       | 76ns    | 6ns        | 9ns        | 6ns       |
| Unpack one component in     8 entities | 27ns      | 21ns   | 13ns      | 109ns   | 10ns       | 18ns       | 13ns      |
| Unpack one component in    16 entities | 56ns      | 47ns   | 28ns      | 201ns   | 38ns       | 45ns       | 28ns      |
| Unpack one component in    32 entities | 111ns     | 86ns   | 54ns      | 376ns   | 69ns       | 92ns       | 62ns      |
| Unpack one component in    64 entities | 220ns     | 168ns  | 111ns     | 728ns   | 116ns      | 183ns      | 100ns     |

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


**Note:**
* Get one non-const component
   1. `PositionComponent`


### Get two components from entity


![UnpackTwoComponents Plot](img/UnpackTwoComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                         | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:----------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack two components in     1 entities | 6ns       | 6ns    | 3ns       | 42ns    | 2ns        | 5ns        | 3ns       |
| Unpack two components in     4 entities | 26ns      | 23ns   | 12ns      | 141ns   | 7ns        | 18ns       | 10ns      |
| Unpack two components in     8 entities | 53ns      | 46ns   | 27ns      | 300ns   | 32ns       | 36ns       | 25ns      |
| Unpack two components in    16 entities | 108ns     | 98ns   | 55ns      | 606ns   | 58ns       | 83ns       | 55ns      |
| Unpack two components in    32 entities | 192ns     | 177ns  | 107ns     | 1041ns  | 90ns       | 145ns      | 96ns      |
| Unpack two components in    64 entities | 405ns     | 365ns  | 204ns     | 2103ns  | 147ns      | 295ns      | 186ns     |

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



**Note:**
* Get non-const- and const-component
   1. `PositionComponent`
   2. `const VelocityComponent`


### Get three components from entity


![UnpackThreeComponents Plot](img/UnpackThreeComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Unpack three components in     1 entities | 7ns       | 9ns    | 5ns       | 80ns    | 3ns        | 9ns        | 4ns       |
| Unpack three components in     2 entities | 17ns      | 16ns   | 10ns      | 150ns   | 6ns        | 18ns       | 8ns       |
| Unpack three components in     8 entities | 68ns      | 76ns   | 49ns      | 583ns   | 53ns       | 91ns       | 33ns      |
| Unpack three components in    16 entities | 131ns     | 142ns  | 99ns      | 1219ns  | 88ns       | 149ns      | 84ns      |
| Unpack three components in    32 entities | 258ns     | 272ns  | 210ns     | 2282ns  | 133ns      | 282ns      | 161ns     |
| Unpack three components in    64 entities | 465ns     | 503ns  | 406ns     | 4894ns  | 249ns      | 531ns      | 265ns     |
| Unpack three components in   128 entities | 1012ns    | 1029ns | 760ns     | 11564ns | 479ns      | 1084ns     | 568ns     |

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



**Note:**
* Not every entity has three components, some has only two
* Get two non-const- and const-component(s)
   1. `PositionComponent`
   2. `const VelocityComponent`
   3. `DataComponent` (optional)


### Remove and add component


![RemoveAddComponent Plot](img/RemoveAddComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                              | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:---------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Remove and Add a Component in     1 entities | 33ns      | 20ns   | 12ns      | 175ns   | 9ns        | 113ns      | 5ns       |
| Remove and Add a Component in     4 entities | 160ns     | 76ns   | 51ns      | 661ns   | 46ns       | 458ns      | 25ns      |
| Remove and Add a Component in     8 entities | 280ns     | 173ns  | 101ns     | 1323ns  | 92ns       | 902ns      | 61ns      |
| Remove and Add a Component in    16 entities | 524ns     | 327ns  | 204ns     | 2801ns  | 163ns      | 1799ns     | 90ns      |
| Remove and Add a Component in    32 entities | 1204ns    | 662ns  | 376ns     | 5353ns  | 296ns      | 3626ns     | 179ns     |
| Remove and Add a Component in    64 entities | 2343ns    | 1642ns | 734ns     | 13016ns | 547ns      | 7082ns     | 309ns     |

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



**Note:**
* Remove and add `PositionComponent`



### Update systems (for-each entities in 2 systems)


  Cost per entity                         |  Cost of all entities
:-------------------------------------------:|:------------------------------------------------------:
  ![SystemsUpdate Plot](img/SystemsUpdate.svg)  |  ![SystemsUpdate Line Plot](img/LineSystemsUpdate.svg)  


_Tables shows total cost. lower is faster._

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update     1 entities with 2 systems | 80ns      | 35ns   | 19ns      | 1711ns  | 43ns       | 380ns      | 51ns      |
| Update     4 entities with 2 systems | 135ns     | 61ns   | 37ns      | 1848ns  | 69ns       | 548ns      | 87ns      |
| Update     8 entities with 2 systems | 211ns     | 113ns  | 60ns      | 1832ns  | 99ns       | 524ns      | 169ns     |
| Update    16 entities with 2 systems | 371ns     | 199ns  | 110ns     | 1322ns  | 193ns      | 601ns      | 246ns     |
| Update    32 entities with 2 systems | 672ns     | 291ns  | 288ns     | 1818ns  | 273ns      | 810ns      | 502ns     |
| Update    64 entities with 2 systems | 1439ns    | 688ns  | 512ns     | 1707ns  | 1073ns     | 1279ns     | 957ns     |

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



**Note:**
* Systems used
    1. `MovementSystem`
    2. `DataSystem`


### Update systems (for-each entities (with mixed components) in 2 systems)


  Cost per entity                         |  Cost of all entities
:-------------------------------------------:|:------------------------------------------------------:
  ![SystemsUpdateMixedEntities Plot](img/SystemsUpdateMixedEntities.svg)  |  ![SystemsUpdateMixedEntities Line Plot](img/LineSystemsUpdateMixedEntities.svg)  


_Tables shows total cost. lower is faster._

|                                      | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS   |
|:-------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:----------|
| Update     1 entities with 2 systems | 63ns      | 16ns   | 18ns      | 540ns   | 56ns       | 109ns      | 31ns      |
| Update     4 entities with 2 systems | 105ns     | 49ns   | 56ns      | 1165ns  | 120ns      | 289ns      | 84ns      |
| Update     8 entities with 2 systems | 174ns     | 91ns   | 130ns     | 1526ns  | 215ns      | 338ns      | 128ns     |
| Update    16 entities with 2 systems | 328ns     | 145ns  | 286ns     | 1587ns  | 410ns      | 440ns      | 241ns     |
| Update    32 entities with 2 systems | 582ns     | 290ns  | 553ns     | 1605ns  | 662ns      | 592ns      | 579ns     |
| Update    64 entities with 2 systems | 1130ns    | 573ns  | 1062ns    | 1838ns  | 1419ns     | 1105ns     | 796ns     |

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


**Note:**
* Systems used
    1. `MovementSystem`
    2. `DataSystem`
* Not every entity has all three components, some got removed


### Update systems (for-each entities in 7 systems)


  Cost per entity                         |  Cost of all entities
:-------------------------------------------:|:------------------------------------------------------:
  ![ComplexSystemsUpdate Plot](img/ComplexSystemsUpdate.svg)  |  ![ComplexSystemsUpdate Line Plot](img/LineComplexSystemsUpdate.svg)  


_Tables shows total cost. lower is faster._

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update     1 entities with 7 systems | 170ns  | 137ns            | 268ns          | 84ns            | 53ns      | 2176ns  | 97ns       | 351ns      | 355ns            | 266ns     |
| Update     4 entities with 7 systems | 206ns  | 759ns            | 588ns          | 251ns           | 199ns     | 2176ns  | 231ns      | 355ns      | 308ns            | 351ns     |
| Update     8 entities with 7 systems | 512ns  | 1670ns           | 730ns          | 583ns           | 449ns     | 2229ns  | 319ns      | 506ns      | 494ns            | 553ns     |
| Update    16 entities with 7 systems | 742ns  | 2983ns           | 1492ns         | 1067ns          | 834ns     | 2370ns  | 578ns      | 532ns      | 456ns            | 911ns     |
| Update    32 entities with 7 systems | 1440ns | 6557ns           | 2508ns         | 1994ns          | 1678ns    | 2502ns  | 974ns      | 639ns      | 669ns            | 1698ns    |
| Update    64 entities with 7 systems | 2647ns | 12175ns          | 4825ns         | 4403ns          | 3541ns    | 3066ns  | 1416ns     | 1093ns     | 970ns            | 3385ns    |

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



**Note:**
* Systems used
    1. `MovementSystem`
    2. `DataSystem`
    3. `MoreComplexSystem`
    4. `HealthSystem`
    5. `DamageSystem`
    6. `SpriteSystem`
    7. `RenderSystem`
* \*   EnTT iterate components via [views](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#views=)
* \**  EnTT iterate components via [runtime views](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#runtime-views=)
* \*** EnTT iterate components via [groups](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#groups=):
   1. `DataSystem`: No Group, use `registry.view<DataComponent>`. _(Can't group a single component)_
   2. `MovementSystem`: Partial-owning group, `registry.group<PositionComponent>(entt::get<const VelocityComponent>)`
   3. `MoreComplexSystem`: Full-owning group, `registry.group<PositionComponent, VelocityComponent, DataComponent>()`
* \**** EnTT iterate components via view and uses a [stable component](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#pointer-stability=) (`StablePositionComponent`)
* \***** gaia-ecs (SoA) iterate via [SoA Layout](https://github.com/richardbiely/gaia-ecs?tab=readme-ov-file#data-layouts) using an `Position`- and `Velocity`-SoA-component



### Update systems (for-each entities (with mixed components) in 7 systems)


  Cost per entity                         |  Cost of all entities
:-------------------------------------------:|:------------------------------------------------------:
  ![ComplexSystemsUpdateMixedEntities Plot](img/ComplexSystemsUpdateMixedEntities.svg)  |  ![ComplexSystemsUpdateMixedEntities Line Plot](img/LineComplexSystemsUpdateMixedEntities.svg)  


_Tables shows total cost. lower is faster._

|                                      | EnTT   | EnTT (runtime)   | EnTT (group)   | EnTT (stable)   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | gaia-ecs (SoA)   | SnapECS   |
|:-------------------------------------|:-------|:-----------------|:---------------|:----------------|:----------|:--------|:-----------|:-----------|:-----------------|:----------|
| Update     1 entities with 7 Systems | 213ns  | 186ns            | 236ns          | 225ns           | 113ns     | 1651ns  | 80ns       | 209ns      | 203ns            | 161ns     |
| Update     4 entities with 7 Systems | 344ns  | 522ns            | 305ns          | 307ns           | 211ns     | 2964ns  | 137ns      | 390ns      | 468ns            | 273ns     |
| Update     8 entities with 7 Systems | 570ns  | 1032ns           | 434ns          | 447ns           | 364ns     | 3033ns  | 257ns      | 418ns      | 534ns            | 387ns     |
| Update    16 entities with 7 Systems | 336ns  | 1143ns           | 371ns          | 343ns           | 428ns     | 2800ns  | 316ns      | 356ns      | 367ns            | 443ns     |
| Update    32 entities with 7 Systems | 852ns  | 2376ns           | 543ns          | 446ns           | 824ns     | 2740ns  | 579ns      | 472ns      | 410ns            | 643ns     |
| Update    64 entities with 7 Systems | 1018ns | 4527ns           | 858ns          | 656ns           | 1485ns    | 3392ns  | 1080ns     | 556ns      | 503ns            | 1119ns    |

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



**Note:**
* Systems used
    1. `MovementSystem`
    2. `DataSystem`
    3. `MoreComplexSystem`
    4. `HealthSystem`
    5. `DamageSystem`
    6. `SpriteSystem`
    7. `RenderSystem`
* Not every entity has all three components, some got removed
* \*   EnTT iterate components via [views](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#views=)
* \**  EnTT iterate components via [runtime views](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#runtime-views=)
* \*** EnTT iterate components via [groups](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#groups=):
    1. `DataSystem`: No Group, use `registry.view<DataComponent>`. _(Can't group a single component)_
    2. `MovementSystem`: Partial-owning group, `registry.group<PositionComponent>(entt::get<const VelocityComponent>)`
    3. `MoreComplexSystem`: Full-owning group, `registry.group<PositionComponent, VelocityComponent, DataComponent>()`
* \**** EnTT iterate components via view and uses a [stable component](https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#pointer-stability=) (`StablePositionComponent`)
* \***** gaia-ecs (SoA) iterate via [SoA Layout](https://github.com/richardbiely/gaia-ecs?tab=readme-ov-file#data-layouts) using an `Position`- und `Velocity`-SoA-component


## Contributing

If you have any improvements to the ECS-examples for any of the frameworks,
feel free to make a pull request or open an issue.
The example(s) for each framework can be found in [`src/`](src/), and benchmarks are located in [`benchmarks/benchmarks/`](benchmarks/benchmarks/) for more details.

Additionally, you can write tests for the framework example and add some metadata to the [plot.config.json](plot.config.json) file.
Any contributions are greatly appreciated!

_TODO: make more detailed "how to add framework"_

Read [CONTRIBUTING.md](CONTRIBUTING) for more details.

## Run Benchmarks

### Using [Taskfile](https://taskfile.dev/installation/)

0. Install [Taskfile](https://taskfile.dev/installation/)
1. Run Build: `task build`
2. Run (All) Benchmarks: `task benchmark`
3. Plot Graphs: `task plot:all`

Or run `task generate`

You can now find the benchmark-results in [`reports/`](reports/).

_You need python (>=3.8.0) and some [dependencies](scripts/gen-benchmark-report/requirements.txt) to run [gen-benchmark-report](scripts/gen-benchmark-report) (plotting), install [pipx](https://pipx.pypa.io/stable/installation/) and [poetry](https://python-poetry.org/docs/#installing-with-pipx)._

Then install the dependencies for the script:
```bash
cd ./scripts/gen-benchmark-report && poetry install
```

Or run the script directly:
```bash
task plot:all
```

#### Generate README

```bash
task generate:readme
```


### ~~Step-by-Step~~

1. Configure and build benchmarks, see [Building Details](doc/README_building.md):
   1. `cmake -S . -B ./build`
   2. `cmake --build ./build`
2. Run benchmark(s), with [`run-benchmarks.sh`](run-benchmarks.sh) or each:
   * `./build/benchmark/benchmarks/entt/ecs-benchmark-entt` _(console output)_ _(optional)_
   * `./build/benchmark/benchmarks/entt/ecs-benchmark-entt --benchmark_format=json > ./reports/entt.json` _(write json report)_
3. Plot Reports, after generating json reports, see [`plot-results.sh`](plot-results.sh):
   1. `pipx run --spec ./scripts/gen-benchmark-report gen-benchmark-report -i ./info.json gen-plot ./reports/entityx.json ./reports/entt.json ./reports/ginseng.json ./reports/flecs.json` _(generate graphs)_
   2. `pipx run --spec ./scripts/gen-benchmark-report gen-benchmark-report -i ./info.json gen-results-md ./reports/entityx.json ./reports/entt.json ./reports/ginseng.json ./reports/flecs.json` _(generate full report)_


### Run a single benchmark

```bash
cmake -G Ninja -S . -B build
cmake --build build --target ecs-benchmark-entt -j 4
./build/benchmark/benchmarks/entt/ecs-benchmark-entt
```

You can use `-DCMAKE_BUILD_TYPE=Debug` to enable Sanitizers.


## Links and More

- [Dependency Setup](doc/README_dependencies.md)
- [Building Details](doc/README_building.md)
- Project Template, mix of: [starter_template](https://github.com/cpp-best-practices/gui_starter_template) and [ModernCppStarter](https://github.com/TheLartians/ModernCppStarter)
  - [Forked cpp_vcpkg_project](https://github.com/abeimler/cpp_vcpkg_project)
- [Google benchmark](https://github.com/google/benchmark)
- https://github.com/SanderMertens/ecs-faq
- https://github.com/jslee02/awesome-entity-component-system


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
