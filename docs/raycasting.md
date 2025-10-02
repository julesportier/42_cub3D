# Raycasting

[wikipedia page](https://en.wikipedia.org/wiki/Ray_casting)


## Tutos

- [lodev raycasting](https://lodev.org/cgtutor/raycasting.html)
- [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview)
- [Wolfenstein 3D raycasting in C](https://timallanwheeler.com/blog/2023/04/01/wolfenstein-3d-raycasting-in-c/)
- [permadi's raycast](https://www.permadi.com/tutorial/raycast/rayc1.html)

- [A raycasting engine in 7 easy steps](https://austinhenley.com/blog/raycasting.html)
(- [Elissa Black ray raycasting engine](https://www.elissablack.com/ray-the-raycasting-engine/))

## Videos

- **javidx9**: 
    - [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA)
- [Wolfenstein 3D's map renderer](https://www.youtube.com/watch?v=eOCQfxRQ2pY)

## Forums

- [fishbowl effect explained](https://gamedev.stackexchange.com/questions/156842/how-can-i-correct-an-unwanted-fisheye-effect-when-drawing-a-scene-with-raycastin)

## Recipe

### lodev summary

1. 2D square grid map, each square can either be 0 or 1 (a wall).
2. For each x of the screen (vertical stripes), send out a ray that starts at the player location with direction depending on both the player looking direction and the x coordinate of the screen. If it hits a wall calculate the distance and use the distance to calculate the height of the wall.
    - use DDA (Digital Differential Analysis) to find when the ray crosses a line.

# Maths for graphics

- [Essential Mathematics For Aspiring Game Developers](https://www.youtube.com/watch?v=DPfxjQ6sqrc)
- [vector normalization (stack overflow)](https://stackoverflow.com/questions/10002918/what-is-the-need-for-normalizing-a-vector)
- [godot engine vector documentation](https://docs.godotengine.org/en/stable/tutorials/math/vector_math.html)
