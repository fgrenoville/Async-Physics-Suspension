# Stable Vehicle Suspensions with Async Physics in UE5

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-%23313131.svg?logo=unrealengine&logoColor=white)


This repository contains an Unreal Engine 5 example project that demonstrates how to use Chaos asynchronous physics with a concrete case: a vehicle supported by four spring-damper suspensions.

The project shows how to:

- Run the physics step on a separate thread (independent from the game tick).

- Exchange data safely between the game thread and the physics thread using FSimCallbackInput / FSimCallbackOutput.

- Implement a simple but effective spring-damper suspension system.

- Visualize debug data (line traces, force vectors) directly on screen.

- Adjust suspension parameters in real time via a small in-game HUD.


![Demo](./GitHubAssets/Vehicle_working.gif)


## 📖 Related Article

This project is explained step by step in my Medium article:

👉 Taming Chaos: Stable Vehicle Suspensions with Async Physics in UE5 (https://levelup.gitconnected.com/taming-chaos-stable-vehicle-suspensions-with-async-physics-in-ue5-566369c7b097)


If you want to dive into the theory behind integration and physics in game engines, check out the previous article here:

👉 [Tick by Tick: How Unreal Engine Simulates Real-World Physics](https://levelup.gitconnected.com/tick-by-tick-how-unreal-engine-simulates-real-world-physics-4ac7bc9ee5fb)


## 🔧 Requirements

- Unreal Engine 5.4+

- C++ toolchain set up


## Contributing

❤️ Enjoying it?
Leave a star on the repo!
Fork it, experiment, and tag me if you build something cool — I’d love to see it.

