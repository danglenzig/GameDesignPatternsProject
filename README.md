# Design Patterns for Game Development Assignment
*Bug Slapper -- Chapter One, The Slappening*

This project is a down-scoped *Vampire Survivor* style game built in C++ , and using the Raylib software development library. It's primary purpose is to demomstrate my understanding of object-oriented design patterns in general, with special focus on those covered in the coursework.

# Assignment Context
## Course Assignment
Implement a smal clone of *Vampire Survivor*, focusing on a subset of its core features.
## Learning Goals
- **Describe** several design patterns
- **Identify** where they are used in the game
- **Explain/fix** issues that arise when patterns are not used or misused

This project is my implementation of that assignment, with an emphasis on clean architecture and pattern deploymeny rather than full feature pairity

# Game Overview
- **Language/Library**: C++ with `raylib` and `raymath`
- **Window**: 1280x720 at 60 FPS
- **Player**:
  - WASD movement control
  - Frame/texture based 2D animation
  - IDLE, WALKING, REACTING, and ATTACKING states (state pattern)
  - Left mouse click triggers an attack with a cooldown
- **Enemies**:
  - Instantiated en-masse at the beginning of gameplay (object pool pattern)
  - Spawned periodically at predefined points around the edge of the screen
  - Fram/texture based 2D animation (flyweight pattern)
  - Can be killed by:
    - Entering the player’s attack sector during a player attack (a cone-shaped area in front of the player), or
    - Entering the player hit radius to deal damage ("kamikaze attack)
- **HUD** displays...
  - The amount of time the player has survived in seconds
  - The current player health
  - Player control information (WASD to move)
- **Objective**
  - Survive as long as possible against an endless, escalating swarm of angry killer bees

# Design Patterns Employed
## Finite State Machine (FSM) Pattern
FSM is implemented in `StateMachine.h` (`FSM`, `FSM_State`, and `FSM_Events` classes). The `Player` owns an `FSM` instance that manages named states (`IDLE`, `WALKING`, `ATTACKING`, `REACTING`) and allowed transitions. The FSM also emits state enter/exit events, which trigger various behaviors and animations.

This pattern separates **state management** from **state-specific behavior**, improving readability and cleanliness vs. unwieldy chains of conditionals in the update loop

## Singleton Pattern
TODO: discuss `InputHandler`, `PlayerStatus`, `RenderSystem` and `GameEvents` classes

## Observer/Event Pattern
TODO: discuss the `GameEvents` class, as well as `PlayerAnimator`, and `FSM` callbacks

## Factory Method/Simple Factory Pattern
TODO: discuss `EnemyFactory`

## Object Pool Pattern
TODO: discuss `EnemyFactory` and `Enemy`

## Component
TODO: discuss `FSM` and `PlayerAnimator` as components of the `Player`, etc.

## FlyWeight Pattern
TODO: discuss `FlyWeightAnimator`

# Reflections & Conclusion
TODO: discuss the benefits and architectural tradeoffs of the various patterns

