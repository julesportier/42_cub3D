/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/18 15:28:51 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <math.h>

t_direction	calc_direction(double angle)
{
	t_direction	direction;

	if (angle < TURN_180)
		direction.y = north;
	else
		direction.y = south;
	if (angle > TURN_90 && angle < TURN_270)
		direction.x = west;
	else
		direction.x = est;
	return (direction);
}

t_point	calc_first_y_intersection(
	t_point pos,
	t_direction direction,
	double angle
)
{
	t_point	intersection;
	double	tan_angle;

	// equivalent to:
	// intersection.y = floor((double)pos.y / 512) * 512;
	// but shift is roughly 40% faster than division
	// and we don't use floor neither floating points.
	intersection.y = (pos.y >> SHIFT_OP_512 << SHIFT_OP_512);
	// Or as grid coordinate:
	// coordinate.y -= (pos.y >> SHIFT_OP_512) - 1;
	// Minus 1 to make the coordinate belong to the above block.
	if (direction.y == south)
		intersection.y += CUBE_SIZE;

	// division by zero of double type don't cause much trouble with IEEE 754.
	intersection.x = round((pos.y - intersection.y) / tan(angle)) + pos.x;
	if (direction.y == north)
		intersection.y -= 1;


	return (intersection);
}

t_point	calc_first_x_intersection(
	t_point pos,
	t_direction direction,
	double angle
)
{
	t_point	intersection;
	double	tan_angle;

	intersection.x = (pos.x >> SHIFT_OP_512 << SHIFT_OP_512);
	if (direction.x == est)
		intersection.x += CUBE_SIZE;
	intersection.y = round((pos.x - intersection.x) * tan(angle)) + pos.y;
	if (direction.x == west)
		intersection.x -= 1;
	return (intersection);
}

t_point	calc_y_vector(t_direction direction, double angle)
{
	t_point	vector;

	vector.y = CUBE_SIZE;
	if (direction.y == north)
		vector.y *= -1;
	vector.x = round(vector.y / tan(angle));
	return (vector);
}

t_point	calc_x_vector(t_direction direction, double angle)
{
	t_point	vector;
	double	cos_angle;

	vector.x = CUBE_SIZE;
	if (direction.x == west)
		vector.x *= -1;
	vector.y = round(vector.x * tan(angle));
	return (vector);
}
//
//
//
//
// #include <stdio.h>
// # define RAD_TO_DEG(rad) ((int)(rad * (180 / M_PI)))
//
//
// int	main(void)
// {
// 	t_point	pos = {.x = 512 + 256, .y = 1024 + 256};
//
// 	printf("=========\nstart pos: x = %d ; y = %d\n", pos.x, pos.y);
// 	printf("---------\ncalc_first_y_intersection()\n");
// 	// Angled to the left/west
// 	double	angle = TURN_135;
// 	t_direction	direction = calc_direction(angle);
// 	t_point	inter = calc_first_y_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	angle = TURN_225;
// 	direction = calc_direction(angle);
// 	inter = calc_first_y_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	// Angled to the right/est
// 	angle = TURN_45;
// 	direction = calc_direction(angle);
// 	inter = calc_first_y_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	angle = TURN_315;
// 	direction = calc_direction(angle);
// 	inter = calc_first_y_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	// Angled to the right/est but faster on est than north
// 	angle = 0.2;
// 	direction = calc_direction(angle);
// 	inter = calc_first_y_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	printf("---------\ncalc_first_x_intersection()\n");
// 	// Angled to the left/west
// 	angle = TURN_135;
// 	direction = calc_direction(angle);
// 	inter = calc_first_x_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	angle = TURN_225;
// 	direction = calc_direction(angle);
// 	inter = calc_first_x_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	// Angled to the right/est
// 	angle = TURN_45;
// 	direction = calc_direction(angle);
// 	inter = calc_first_x_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	angle = TURN_315;
// 	direction = calc_direction(angle);
// 	inter = calc_first_x_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	// Angled to the right/est but faster on est than north
// 	angle = 0.2;
// 	direction = calc_direction(angle);
// 	inter = calc_first_x_intersection(pos, direction, angle);
// 	printf("%ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
//
// 	printf("---------\ncalc_y_vector()\n");
// 	// Angled to the left/west
// 	angle = TURN_135;
// 	direction = calc_direction(angle);
// 	inter = calc_y_vector(direction, angle);
// 	printf("vector %ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	angle = TURN_225;
// 	direction = calc_direction(angle);
// 	inter = calc_y_vector(direction, angle);
// 	printf("vector %ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	// Angled to the right/est
// 	angle = TURN_45;
// 	direction = calc_direction(angle);
// 	inter = calc_y_vector(direction, angle);
// 	printf("vector %ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	angle = TURN_315;
// 	direction = calc_direction(angle);
// 	inter = calc_y_vector(direction, angle);
// 	printf("vector %ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// 	// Angled to the right/est but faster on x than y
// 	angle = 0.2;
// 	direction = calc_direction(angle);
// 	inter = calc_y_vector(direction, angle);
// 	printf("vector %ddeg x = %d; y = %d\n", RAD_TO_DEG(angle), inter.x, inter.y);
// // 	angle = 1;
// // 	inter = calc_y_vector(north, angle);
// // 	printf("inter vector north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	inter = calc_y_vector(south, angle);
// // 	printf("inter vector south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// //
// // 	printf("-------\ncalc_x_vector()\n");
// // 	// Full angled to the left/west 0deg
// // 	angle = 0;
// // 	inter = calc_x_vector(west, angle);
// // 	printf("inter vector west 0rad %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	inter = calc_x_vector(est, angle);
// // 	printf("inter vector est 0rad %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	// Angled to the right/est
// // 	angle = (double)FOV / 2;
// // 	inter = calc_x_vector(west, angle);
// // 	printf("inter vector west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	inter = calc_x_vector(est, angle);
// // 	printf("inter vector est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	// Angled to the left/west
// // 	angle = (M_PI * 2) - (double)FOV / 2;
// // 	inter = calc_x_vector(west, angle);
// // 	printf("inter vector west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	inter = calc_x_vector(est, angle);
// // 	printf("inter vector est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	// Angled to the right/est but faster on x than y
// // 	angle = 1;
// // 	inter = calc_x_vector(west, angle);
// // 	printf("inter vector west %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// // 	inter = calc_x_vector(est, angle);
// // 	printf("inter vector est %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// 	return (0);
// }
