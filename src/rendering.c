/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/09 12:04:18 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "rendering.h"
#include <math.h>

double	calc_angle_increment(int screen_width, double fov)
{
	return (fov / screen_width);
}

t_point	calc_first_y_intersection(
	t_point pos,
	t_direction direction,
	double angle
)
{
	t_point	intersection;

	// equivalent to:
	// intersection.y = floor((double)pos.y / 512) * 512;
	// but shift is roughly 40% faster than division
	// and we don't use floor neither floating points.
	intersection.y = (pos.y >> SHIFT_OP_512 << SHIFT_OP_512);

	// Or as grid coordinate:
	// coordinate.y -= (pos.y >> SHIFT_OP_512) - 1;
	intersection.x = (pos.y - intersection.y) * tan(angle);
	// Minus 1 to make the coordinate belong to the above block.
	if (direction == north)
		intersection.y -= 1;
	else
		intersection.y += CUBE_SIZE;
	intersection.x = pos.x + intersection.x;
	return (intersection);
}

t_point	calc_first_x_intersection(
	t_point pos,
	t_direction direction,
	double angle
)
{
	t_point	intersection;

	intersection.x = (pos.x >> SHIFT_OP_512 << SHIFT_OP_512);
	intersection.y = (pos.x - intersection.x) / cos(M_PI_2 - angle);
	if (direction == west)
		intersection.x -= 1;
	else
		intersection.x += CUBE_SIZE;
	intersection.y = pos.y + intersection.y;
	return (intersection);
}

t_point	calc_y_vector(t_direction direction, double angle)
{
	t_point	vector;

	vector.y = CUBE_SIZE;
	vector.x = vector.y * tan(angle);
	if (direction == north)
		vector.y *= -1;
	return (vector);
}

t_point	calc_x_vector(t_direction direction, double angle)
{
	t_point	vector;

	vector.x = CUBE_SIZE;
	vector.y = vector.x / cos(M_PI_2 - angle);
	if (direction == west)
		vector.x *= -1;
	return (vector);
}

// t_point	dda(t_point pos, t_point vector_x, t_point vector_y)
// {
// 	int	x_dist;
// 	int	y_dist;
//
// 	x_dist = 0;
// 	y_dist = 0;
// 	while (pos.x > 0 && pos.x < WIN_WIDTH && pos.y> 0 && pos.y < WIN_HEIGHT)
// 	{
// 		if (x_dist < y_dist)
// 		{
//
//
//
// }

#include <stdio.h>

int	main(void)
{
	t_point	pos = {.x = 256, .y = 1024 + 256};

	printf("calc_first_y_intersection()\n");
	// Angled to the left/west
	double	angle = (M_PI * 2) - (double)FOV / 2;
	t_point	inter = calc_first_y_intersection(pos, north, angle);
	printf("inter north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_first_y_intersection(pos, south, angle);
	printf("inter south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the right/est
	angle = (double)FOV / 2;
	inter = calc_first_y_intersection(pos, north, angle);
	printf("inter north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_first_y_intersection(pos, south, angle);
	printf("inter south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the right/est but faster on x than y
	angle = 1;
	inter = calc_first_y_intersection(pos, north, angle);
	printf("inter north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_first_y_intersection(pos, south, angle);
	printf("inter south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);

	printf("-------\ncalc_first_x_intersection()\n");
	// Angled to the left/west
	angle = (M_PI * 2) - (double)FOV / 2;
	inter = calc_first_x_intersection(pos, west, angle);
	printf("inter west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_first_x_intersection(pos, est, angle);
	printf("inter est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the right/est
	angle = (double)FOV / 2;
	inter = calc_first_x_intersection(pos, west, angle);
	printf("inter west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_first_x_intersection(pos, est, angle);
	printf("inter est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the right/est but faster on x than y
	angle = 1;
	inter = calc_first_x_intersection(pos, west, angle);
	printf("inter west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_first_x_intersection(pos, est, angle);
	printf("inter est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);

	printf("-------\ncalc_y_vector()\n");
	// Angled to the right/est
	angle = (double)FOV / 2;
	inter = calc_y_vector(north, angle);
	printf("inter vector north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_y_vector(south, angle);
	printf("inter vector south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the left/west
	angle = (M_PI * 2) - (double)FOV / 2;
	inter = calc_y_vector(north, angle);
	printf("inter vector north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_y_vector(south, angle);
	printf("inter vector south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the right/est but faster on x than y
	angle = 1;
	inter = calc_y_vector(north, angle);
	printf("inter vector north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_y_vector(south, angle);
	printf("inter vector south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);

	printf("-------\ncalc_x_vector()\n");
	// Angled to the right/est
	angle = (double)FOV / 2;
	inter = calc_x_vector(west, angle);
	printf("inter vector west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_x_vector(est, angle);
	printf("inter vector est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the left/west
	angle = (M_PI * 2) - (double)FOV / 2;
	inter = calc_x_vector(west, angle);
	printf("inter vector west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_x_vector(est, angle);
	printf("inter vector est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	// Angled to the right/est but faster on x than y
	angle = 1;
	inter = calc_x_vector(west, angle);
	printf("inter vector west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	inter = calc_x_vector(est, angle);
	printf("inter vector est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
	return (0);
}
