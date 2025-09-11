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

// #include "cube.h"
#include "rendering.h"
#include <math.h>

double	calc_angle_increment(int screen_width, double fov)
{
	return (fov / screen_width);
}

t_point	calc_first_intersection(
	t_point pos,
	t_direction direction,
	double angle
)
{
	t_point	intersection;

	// equivalent to: floor(pos.y / 512) * 512 - 1
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
}
//
// #include <stdio.h>
//
// int	main(void)
// {
// 	t_point	pos = {.x = 256, .y = 1024 + 256};
// 	// Angled to the left/west
// 	double	angle = (M_PI * 2) - (double)FOV / 2;
// 	t_point	inter = calc_first_intersection(pos, north, angle);
// 	printf("inter north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// 	inter = calc_first_intersection(pos, south, angle);
// 	printf("inter south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// 	// Angled to the right/est
// 	angle = (double)FOV / 2;
// 	inter = calc_first_intersection(pos, north, angle);
// 	printf("inter north %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// 	inter = calc_first_intersection(pos, south, angle);
// 	printf("inter south %ddeg x = %d; y = %d\n", (int)(angle * (180 / M_PI)), inter.x, inter.y);
// 	return (0);
// }
