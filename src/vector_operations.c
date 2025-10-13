/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/09 10:31:30 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <math.h>
#include <stdio.h>

void	print_point(char *name, t_point point)
{
	printf("%s .x == %d ; .y == %d\n", name, point.x, point.y);
}
void	print_vec(char *name, t_vec vec)
{
	printf("%s .x == %f ; .y == %f\n", name, vec.x, vec.y);
}

t_vec	negate_vec(t_vec vec)
{
	vec.x *= -1;
	vec.y *= -1;
	return (vec);
}

t_vec	normalize_vec(t_vec vec)
{
	double	magnitude;

	magnitude = hypot(vec.x, vec.y);
	vec.x /= magnitude;
	vec.y /= magnitude;
	return (vec);
}

// t_vec	calc_dir_vec(double angle)
// {
// 	t_direction	direction;
// 	t_vec		vec;
//
// 	direction = calc_direction(angle);
// 	vec.x = 1;
// 	vec.y = tan(scale_angle(angle));
// 	if (vec.y > 10)
// 		vec.y = 10;
// // #include <stdio.h>
// // 	printf("dir vec.y == %f\n", vec.y);
// 	if (direction.x == west)
// 		vec.x *= -1;
// 	if (direction.y == north)
// 		vec.y *= -1;
// 	return (normalize_vec(vec));
// }

// Apply the 2d rotation matrix to the vector
t_vec	rotate_vec(t_vec vec, double angle)
{
	vec.x = vec.x * cos(angle) - (vec.y * sin(angle));
	vec.y = vec.x * sin(angle) + (vec.y * cos(angle));
	return (vec);
}

t_vec	add_vec(t_vec vec_a, t_vec vec_b)
{
	t_vec	add;

	add.x = vec_a.x + vec_b.x;
	add.y = vec_a.y + vec_b.y;
	return (add);
}

t_vec	d_mul_vec(t_vec vec, double mul)
{
	vec.x *= mul;
	vec.y *= mul;
	return (vec);
}
