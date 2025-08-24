# Strafer

A C++ raylib App (Game)

Teaches how to counter strafe through time through a physics simulation of the game. 
Your goal is optimize your time till accurate. It should be near 80ms-120ms with 100ms being the best.
On the left is your history which shows previous attempts. It's currently bugged and needs to be turned into a scrollview esq thing.

## Technical Information

We do a physics simulation of the game using these variables:
```cpp
constexpr float MAX_SPEED = 215.0f;
constexpr float SV_ACCELERATE = 5.5f;
constexpr float SV_FRICTION = 4.0f;
constexpr float SURFACE_FRICTION = 1.0f;
```
Which in the end just makes it so that when you let go of the strafe key from 215 velo, it takes 230ms to get to near 80 velo.
This math is all subject to change and I might in the end, implement a subtick emulation as its not that hard.
Ends up being 3 threads: UI thread, Simulation thread, Input thread. UI does rendering and reads data from mutex of StrafeManager. 
Simulation thread runs the 64 tick simulation which actually updates the state, locking mutex and etc.
Input thread is very fast always checking for input using raw devices, it then pushes it into a threadsafequeue. With timestamps
which is then processed by the simulation thread as in order.

## How to obtain

Need cmake and a bunch of other tools, simple way to get it is CLion. And using that to build.
I don't really have a production build setup yet so bear with me as this was just a fun project.
