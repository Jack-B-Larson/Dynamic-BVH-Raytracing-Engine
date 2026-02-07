# DynamicBVH-Raytracing-Engine

[YouTube Demo](https://youtu.be/LH8GmG5m9KE)

Made in Visual Studio, using GLFW for windowing and system interaction and OpenGL for rendering via compute shaders (using GLAD bindings).

Features:
Dynamic bounding volume hierarchy, generated at runtime and adaptive to moving objects, used for raytracing acceleration on the GPU as well as a potential future collision detection implementation.
Buffered input system to check for key-downs, key-ups, and currently held keys, as well as retrieving mouse input.
Scene system, allowing for multiple scenes to be loaded at once.
Transform hierarchy, allowing parenting of objects with local transformations.
Engine variables that can be set via launch options.
An partial implementation for uploading voxel data to the GPU, although currently this data is unused.
Separate game and render threads (definitely not completely thread safe), with individual frequencies.
Interpolated transforms for rendering between ticks.
Threaded logging system.

In the demo:
A spinning and bobbing rectangular prism with a child cube, an array of 512 large cubes underneath to stress test the BVH, and a controllable camera (WASD + Mouse).
Objects render as blue OBBs with a fade to show depth, and the BVH is rendered as red AABBs with increasing value to show more iterations (see shaders).
