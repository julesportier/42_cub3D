# Raycasting

[wikipedia page](https://en.wikipedia.org/wiki/Ray_casting)


## Tutos

- [lodev raycasting](https://lodev.org/cgtutor/raycasting.html)
- [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview)
- [Wolfenstein 3D raycasting in C](https://timallanwheeler.com/blog/2023/04/01/wolfenstein-3d-raycasting-in-c/)

- [permadi's raycast](https://www.permadi.com/tutorial/raycast/rayc1.html)

- [A raycasting engine in 7 easy steps](https://austinhenley.com/blog/raycasting.html)
(- [Elissa Black ray raycasting engine](https://www.elissablack.com/ray-the-raycasting-engine/))


## Recipe

### lodev summary

1. 2D square grid map, each square can either be 0 or 1 (a wall).
2. For each x of the screen (vertical stripes), send out a ray that starts at the player location with direction depending on both the player looking direction and the x coordinate of the screen. If it hits a wall calculate the distance and use the distance to calculate the height of the wall.
    - use DDA (Digital Differential Analysis) to find when the ray crosses a line.
