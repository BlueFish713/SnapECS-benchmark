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

|                               | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Create   256 (empty) entities | 67us      | 54us   | 18us      | 977us   | 7us        | 43us       | 5us                 | 5us                |
| Create   ~1K (empty) entities | 242us     | 150us  | 71us      | 1042us  | 25us       | 143us      | 15us                | 16us               |
| Create   ~4K (empty) entities | 925us     | 602us  | 268us     | 1331us  | 112us      | 508us      | 55us                | 59us               |
| Create  ~16K (empty) entities | 3466us    | 3127us | 1083us    | 2182us  | 463us      | 2176us     | 204us               | 241us              |

|                               | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Create  ~65K (empty) entities | 15ms      | 11ms   | 4ms       | 5ms     | 2ms        | 8ms        | 0ms                 | 0ms                |
| Create  262K (empty) entities | 56ms      | 47ms   | 19ms      | 17ms    | 9ms        | 40ms       | 3ms                 | 3ms                |
| Create   ~1M (empty) entities | 238ms     | 185ms  | 78ms      | 69ms    | 39ms       | 150ms      | 13ms                | 15ms               |
| Create   ~2M (empty) entities | 436ms     | 380ms  | 155ms     | 141ms   | 73ms       | 317ms      | 28ms                | 30ms               |


### Get No component from Entity

![UnpackNoComponent Plot](img/UnpackNoComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                    | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Get No component in   256 entities | 0us       | 17us   | 5us       | 15us    | 0us        | 17us       | 2us                 | 2us                |
| Get No component in   ~1K entities | 1us       | 67us   | 19us      | 63us    | 1us        | 68us       | 7us                 | 8us                |
| Get No component in   ~4K entities | 4us       | 280us  | 80us      | 255us   | 5us        | 279us      | 32us                | 34us               |
| Get No component in  ~16K entities | 18us      | 1118us | 325us     | 961us   | 24us       | 1103us     | 140us               | 133us              |

|                                    | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:-----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Get No component in  ~65K entities | 0ms       | 4ms    | 1ms       | 3ms     | 0ms        | 4ms        | 0ms                 | 0ms                |
| Get No component in  262K entities | 0ms       | 17ms   | 5ms       | 16ms    | 0ms        | 18ms       | 2ms                 | 2ms                |
| Get No component in   ~1M entities | 1ms       | 72ms   | 21ms      | 69ms    | 1ms        | 73ms       | 8ms                 | 8ms                |
| Get No component in   ~2M entities | 2ms       | 146ms  | 43ms      | 135ms   | 3ms        | 156ms      | 17ms                | 18ms               |


### Create entities

![CreateEntities Plot](img/CreateEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                           | EntityX   | EnTT    | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------------------|:----------|:--------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Create   256 entities with two Components | 212us     | 286us   | 358us     | 1440us  | 23us       | 559us      | 51us                | 55us               |
| Create   ~1K entities with two Components | 783us     | 987us   | 611us     | 2137us  | 90us       | 2096us     | 184us               | 199us              |
| Create   ~4K entities with two Components | 3259us    | 3735us  | 1672us    | 4988us  | 406us      | 8475us     | 739us               | 820us              |
| Create  ~16K entities with two Components | 12321us   | 14213us | 5452us    | 16783us | 1605us     | 33259us    | 2848us              | 3104us             |

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Create  ~65K entities with two Components | 49ms      | 52ms   | 23ms      | 60ms    | 6ms        | 135ms      | 11ms                | 12ms               |
| Create  262K entities with two Components | 217ms     | 242ms  | 97ms      | 234ms   | 29ms       | 508ms      | 46ms                | 49ms               |
| Create   ~1M entities with two Components | 798ms     | 953ms  | 375ms     | 961ms   | 118ms      | 2208ms     | 186ms               | 202ms              |
| Create   ~2M entities with two Components | 1656ms    | 1966ms | 751ms     | 1880ms  | 232ms      | 4385ms     | 360ms               | 416ms              |


### Get one (non-const) component from Entity

![UnpackOneComponent Plot](img/UnpackOneComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                        | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:---------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Unpack one component in   256 entities | 24us      | 51us   | 9us       | 21us    | 3us        | 21us       | 5us                 | 5us                |
| Unpack one component in   ~1K entities | 108us     | 192us  | 37us      | 74us    | 11us       | 88us       | 20us                | 21us               |
| Unpack one component in   ~4K entities | 431us     | 799us  | 155us     | 298us   | 49us       | 375us      | 84us                | 84us               |
| Unpack one component in  ~16K entities | 1677us    | 3188us | 571us     | 1359us  | 203us      | 1442us     | 343us               | 346us              |

|                                        | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:---------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Unpack one component in  ~65K entities | 6ms       | 11ms   | 2ms       | 5ms     | 0ms        | 5ms        | 1ms                 | 1ms                |
| Unpack one component in  262K entities | 30ms      | 41ms   | 9ms       | 18ms    | 3ms        | 24ms       | 5ms                 | 5ms                |
| Unpack one component in   ~1M entities | 108ms     | 193ms  | 38ms      | 86ms    | 12ms       | 91ms       | 22ms                | 23ms               |
| Unpack one component in   ~2M entities | 247ms     | 333ms  | 78ms      | 172ms   | 25ms       | 197ms      | 45ms                | 42ms               |


### Get two components from entity

![UnpackTwoComponents Plot](img/UnpackTwoComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                         | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:----------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Unpack two components in   256 entities | 55us      | 80us   | 20us      | 38us    | 6us        | 46us       | 11us                | 11us               |
| Unpack two components in   ~1K entities | 217us     | 395us  | 79us      | 155us   | 24us       | 190us      | 46us                | 43us               |
| Unpack two components in   ~4K entities | 926us     | 1605us | 323us     | 648us   | 105us      | 789us      | 180us               | 180us              |
| Unpack two components in  ~16K entities | 4004us    | 5922us | 1306us    | 2443us  | 431us      | 3155us     | 779us               | 736us              |

|                                         | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:----------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Unpack two components in  ~65K entities | 17ms      | 22ms   | 5ms       | 10ms    | 1ms        | 13ms       | 2ms                 | 2ms                |
| Unpack two components in  262K entities | 57ms      | 99ms   | 20ms      | 43ms    | 6ms        | 51ms       | 12ms                | 12ms               |
| Unpack two components in   ~1M entities | 265ms     | 342ms  | 83ms      | 165ms   | 26ms       | 200ms      | 50ms                | 46ms               |
| Unpack two components in   ~2M entities | 542ms     | 684ms  | 168ms     | 314ms   | 57ms       | 417ms      | 95ms                | 101ms              |


### Get three components from entity

![UnpackThreeComponents Plot](img/UnpackThreeComponents.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Unpack three components in   512 entities | 138us     | 249us  | 82us      | 151us   | 20us       | 219us      | 47us                | 43us               |
| Unpack three components in   ~2K entities | 550us     | 1022us | 338us     | 578us   | 81us       | 823us      | 180us               | 181us              |
| Unpack three components in   ~8K entities | 2100us    | 4116us | 1362us    | 2224us  | 333us      | 3143us     | 764us               | 717us              |

|                                           | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Unpack three components in  ~32K entities | 9ms       | 15ms   | 5ms       | 9ms     | 1ms        | 12ms       | 2ms                 | 2ms                |
| Unpack three components in  131K entities | 35ms      | 64ms   | 23ms      | 39ms    | 5ms        | 51ms       | 12ms                | 12ms               |
| Unpack three components in  524K entities | 136ms     | 259ms  | 78ms      | 133ms   | 21ms       | 198ms      | 47ms                | 45ms               |
| Unpack three components in   ~1M entities | 283ms     | 521ms  | 161ms     | 302ms   | 45ms       | 415ms      | 92ms                | 92ms               |


### Add component

![AddComponent Plot](img/AddComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                   | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Add a Component in   256 entities | 93us      | 121us  | 75us      | 165us   | 55us       | 411us      | 60us                | 59us               |
| Add a Component in   ~1K entities | 352us     | 460us  | 312us     | 648us   | 217us      | 1592us     | 243us               | 235us              |
| Add a Component in   ~4K entities | 1533us    | 1864us | 1231us    | 2606us  | 858us      | 6559us     | 968us               | 960us              |
| Add a Component in  ~16K entities | 6299us    | 7586us | 4620us    | 10170us | 3649us     | 26320us    | 4200us              | 3910us             |

|                                   | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:----------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Add a Component in  ~65K entities | 21ms      | 31ms   | 20ms      | 42ms    | 13ms       | 101ms      | 15ms                | 14ms               |
| Add a Component in  262K entities | 89ms      | 120ms  | 83ms      | 171ms   | 54ms       | 402ms      | 63ms                | 60ms               |
| Add a Component in   ~1M entities | 320ms     | 487ms  | 321ms     | 653ms   | 227ms      | 1644ms     | 254ms               | 238ms              |
| Add a Component in   ~2M entities | 647ms     | 959ms  | 619ms     | 1348ms  | 451ms      | 3315ms     | 515ms               | 476ms              |


### Remove and add component

![RemoveAddComponent Plot](img/RemoveAddComponent.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                              | EntityX   | EnTT    | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:---------------------------------------------|:----------|:--------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Remove and Add a Component in   256 entities | 61us      | 157us   | 44us      | 203us   | 11us       | 578us      | 24us                | 27us               |
| Remove and Add a Component in   ~1K entities | 268us     | 630us   | 177us     | 841us   | 48us       | 2332us     | 100us               | 104us              |
| Remove and Add a Component in   ~4K entities | 1203us    | 2475us  | 710us     | 3593us  | 191us      | 9442us     | 397us               | 414us              |
| Remove and Add a Component in  ~16K entities | 4119us    | 10099us | 2988us    | 13135us | 753us      | 37146us    | 1600us              | 1621us             |

|                                              | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   | SnapECS (Indexed)   | SnapECS (Cached)   |
|:---------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|:--------------------|:-------------------|
| Remove and Add a Component in  ~65K entities | 17ms      | 42ms   | 11ms      | 52ms    | 3ms        | 151ms      | 6ms                 | 6ms                |
| Remove and Add a Component in  262K entities | 68ms      | 164ms  | 46ms      | 196ms   | 12ms       | 617ms      | 25ms                | 25ms               |
| Remove and Add a Component in   ~1M entities | 286ms     | 643ms  | 173ms     | 848ms   | 49ms       | 2456ms     | 108ms               | 107ms              |
| Remove and Add a Component in   ~2M entities | 563ms     | 1313ms | 341ms     | 1679ms  | 98ms       | 4832ms     | 215ms               | 209ms              |


### Destroy entities

![DestroyEntities Plot](img/DestroyEntities.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                            | EntityX   | EnTT    | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   |
|:-------------------------------------------|:----------|:--------|:----------|:--------|:-----------|:-----------|
| Destroy   256 entities with two components | 121us     | 216us   | 383us     | 1060us  | 10us       | 256us      |
| Destroy   ~1K entities with two components | 458us     | 863us   | 1382us    | 1194us  | 37us       | 1018us     |
| Destroy   ~4K entities with two components | 1967us    | 3395us  | 5181us    | 1632us  | 148us      | 3945us     |
| Destroy  ~16K entities with two components | 7385us    | 13374us | 20938us   | 3197us  | 609us      | 16002us    |

|                                            | EntityX   | EnTT   | Ginseng   | Flecs   | pico_ecs   | gaia-ecs   |
|:-------------------------------------------|:----------|:-------|:----------|:--------|:-----------|:-----------|
| Destroy  ~65K entities with two components | 36ms      | 53ms   | 88ms      | 9ms     | 2ms        | 68ms       |
| Destroy  262K entities with two components | 131ms     | 219ms  | 323ms     | 34ms    | 9ms        | 277ms      |
| Destroy   ~1M entities with two components | 525ms     | 852ms  | 1347ms    | 127ms   | 42ms       | 1090ms     |
| Destroy   ~2M entities with two components | 1060ms    | 1752ms | 2563ms    | 263ms   | 85ms       | 2227ms     |


### Create entities at once

![CreateEntitiesInBulk Plot](img/CreateEntitiesInBulk.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                   | EnTT   |
|:--------------------------------------------------|:-------|
| Create   256 entities with two components at once | 111us  |
| Create   ~1K entities with two components at once | 319us  |
| Create   ~4K entities with two components at once | 1156us |
| Create  ~16K entities with two components at once | 4629us |

|                                                   | EnTT   |
|:--------------------------------------------------|:-------|
| Create  ~65K entities with two components at once | 17ms   |
| Create  262K entities with two components at once | 81ms   |
| Create   ~1M entities with two components at once | 307ms  |
| Create   ~2M entities with two components at once | 625ms  |


### Create empty entities at once

![CreateEmptyEntitiesInBulk Plot](img/CreateEmptyEntitiesInBulk.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                       | EnTT   |
|:--------------------------------------|:-------|
| Create   256 (empty) entities at once | 41us   |
| Create   ~1K (empty) entities at once | 132us  |
| Create   ~4K (empty) entities at once | 492us  |
| Create  ~16K (empty) entities at once | 1983us |

|                                       | EnTT   |
|:--------------------------------------|:-------|
| Create  ~65K (empty) entities at once | 8ms    |
| Create  262K (empty) entities at once | 31ms   |
| Create   ~1M (empty) entities at once | 133ms  |
| Create   ~2M (empty) entities at once | 271ms  |


### Destroy entities at once

![DestroyEntitiesInBulk Plot](img/DestroyEntitiesInBulk.svg)

_Graph shows cost per entity, tables shows total cost. lower is faster._

|                                                    | EnTT    |
|:---------------------------------------------------|:--------|
| Destroy   256 entities with two components at once | 183us   |
| Destroy   ~1K entities with two components at once | 755us   |
| Destroy   ~4K entities with two components at once | 2861us  |
| Destroy  ~16K entities with two components at once | 10874us |

|                                                    | EnTT   |
|:---------------------------------------------------|:-------|
| Destroy  ~65K entities with two components at once | 44ms   |
| Destroy  262K entities with two components at once | 175ms  |
| Destroy   ~1M entities with two components at once | 716ms  |
| Destroy   ~2M entities with two components at once | 1399ms |





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

