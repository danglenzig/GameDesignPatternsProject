# Design Patterns for Game Development Assignment
*Bug Slapper -- Chapter One, The Slappening*

This project is a down-scoped *Vampire Survivor*-style game built in C++ and using the `raylib` game library. Its primary purpose is to demonstrate my understanding of object-oriented design patterns in general, with special focus on those covered in the coursework.

# Assignment Context
## Course Assignment
Implement a small clone of *Vampire Survivor*, focusing on a subset of its core features.
## Learning Goals
- **Describe** several design patterns
- **Identify** where they are used in the game
- **Explain/fix** issues that arise when patterns are not used or misused

This project is my implementation of that assignment, with an emphasis on clean architecture and pattern deployment rather than full feature parity.

# Game Overview
- **Language/Library**: C++ with `raylib` and `raymath`
- **Window**: 1280x720 at 60 FPS
- **Player**:
  - WASD movement control
  - Frame/texture-based 2D animation
  - IDLE, WALKING, REACTING, and ATTACKING states (state pattern)
  - Left mouse click triggers an attack with a cooldown
- **Enemies**:
  - Instantiated en-masse at the beginning of gameplay (object pool pattern)
  - Spawned periodically at predefined points around the edge of the screen
  - Frame/texture-based 2D animation (flyweight pattern)
  - Can be killed by:
    - Entering the player’s attack sector during a player attack (a cone-shaped area in front of the player), or
    - Entering the player hit radius to deal damage ("kamikaze" attacks)
- **HUD** displays...
  - The amount of time the player has survived in seconds
  - The current player health
  - Player control information (WASD to move)
- **Objective**
  - Survive as long as possible against an endless, escalating swarm of angry killer bees

# Design Patterns Employed
## Finite State Machine (FSM) Pattern
FSM is implemented in `StateMachine.h` (`FSM`, `FSM_State`, and `FSM_Events` classes). The `Player` owns an `FSM` instance that manages named states (`IDLE`, `WALKING`, `ATTACKING`, `REACTING`) and allowed transitions. The FSM also emits state enter/exit events, which trigger various behaviors and animations.

This pattern separates **state management** from **state-specific behavior**, improving readability and cleanliness vs. unwieldy chains of conditionals in the update loop.

## Singleton Pattern
Several systems are implemented as singletons (`InputHandler`, `PlayerStatus`, `RenderSystem`, and `GameEvents`). Each of these classes hides its constructor, deletes copy/assignment, and exposes a single `Instance()` method that returns a static, long-lived object. This gives me globally accessible services (input, shared player state, rendering, and the central event bus) without having to manually thread references through every part of the code.

The trade-off is that these classes behave like global state and are harder to unit test in isolation, but for a small, arcade-style project the simplicity and clarity I gain from “just ask the singleton” is worth the downside.

## Observer/Event Pattern
The observer pattern is implemented via the templated `Event<T>` and `SimpleEvent` classes in `Event.h`, and coordinated through the `GameEvents` singleton. `GameEvents` exposes high-level signals such as `OnFrameUpdate`, `OnPlayerSlap`, `OnEnemyDied`, `OnAttackInput`, and `DamageTaken`.

Core game objects subscribe to these events instead of talking to each other directly. For example, `GameObject` subscribes to `OnFrameUpdate` so every `Player` and `Enemy` automatically receives per-frame updates. The `InputHandler` fires `OnAttackInput`, which the `Player` listens to in order to trigger attacks. Enemies listen for `OnPlayerSlap` to decide when they should die, and `EnemyFactory` listens for `OnEnemyDied` so it can recycle enemies. `PlayerAnimator` also exposes its own mini-event system (`AnimEvent`) so the `Player` can react when an animation finishes.

This event-driven approach keeps systems decoupled: emitters do not need to know who is listening, and adding new reactions is usually as simple as subscribing another listener.

## Factory Method/Simple Factory Pattern
Enemy creation is centralized in the `EnemyFactory` class, which acts as a simple factory. It owns configuration data for enemies (via `EnemyConfig` and `GetSkeeterConfig()`), controls spawn timing and locations, and knows how to wire each `Enemy` up with shared resources like the `FlyweightAnimator`.

Client code does not construct `Enemy` objects directly; instead it relies on `EnemyFactory` to prepare and manage them. If I want to introduce a new enemy type later, I can add a new configuration and extend the factory logic, without touching all the call sites that currently assume “give me an enemy.”

## Object Pool Pattern
`EnemyFactory` also implements an object pool for enemies. At startup, `InitializeEnemyPool()` pre-allocates a large number of `Enemy` instances (500 by default) and keeps them in a `std::vector`. When it is time to spawn, `SpawnEnemy()` scans for the first inactive enemy, configures it (position, speed skew, etc.), and re-activates it instead of allocating a brand-new object.

When an enemy dies, it raises `OnEnemyDied`, which `EnemyFactory` handles by calling `Recycle()` on the matching `Enemy` and marking it inactive so it can be reused later. This pattern is a natural fit for a swarm game where enemies are constantly being created and destroyed, because it avoids frequent heap allocations and keeps performance more predictable as the swarm grows.

## Component
While this project does not use a full entity–component system, it does lean on simple components via composition. The `Player` is a `GameObject` that owns an `FSM` for behavior/state transitions and a `PlayerAnimator` for visuals. The FSM decides *what* the player is doing (IDLE, WALKING, ATTACKING, REACTING), while the animator decides *how it looks* (which sprite sequence to play and when to loop or finish).

Because these pieces are self-contained, they behave like reusable components: the same state machine and animation ideas could be attached to other characters without rewriting the core game loop. This keeps responsibilities narrow, reduces duplication, and makes it clear where to plug in new behavior.

## Flyweight Pattern
The `FlyweightAnimator` class is a small flyweight responsible for sharing enemy animation textures. On construction it loads the “SKEETER” frames once into a `std::vector<Texture2D>`, and each `Enemy` simply asks `GetPtrToTexture(configName, currentFrame)` for the texture it needs.

Instead of every enemy instance owning its own copy of the same textures, they all reference a shared set of frames. This cuts down on memory usage and texture load time, which matters when you have hundreds of visually identical enemies on screen. The `PlayerAnimator` applies a similar idea for the player character’s animations, even though it is not structured exactly the same as `FlyweightAnimator`.

# Reflections & Conclusion
Overall, the patterns in this project are aimed at keeping the main loop thin and pushing behavior into focused, well-named units. Singletons and the event system give me simple, globally accessible services while still keeping gameplay code reasonably decoupled. The factory + object pool combination lets the enemy swarm scale up without turning into a performance problem, and the FSM + animation components make the player’s behavior easy to reason about and extend.

If I were to take the project further, I would likely replace some singletons with dependency-injected services, move more configuration into data, and explore a more formal component system. For the purposes of this assignment, though, the chosen patterns strike a good balance between clarity, flexibility, and implementation effort.

