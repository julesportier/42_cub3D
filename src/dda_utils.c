/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/29 13:02:34 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <math.h>
 
int	calc_x_step(double angle)
{
	angle = scale_angle(angle);
	return (roundf(CUBE_SIZE / cos(angle)));
}

int	calc_y_step(double angle)
{
	angle = scale_angle(angle);
	return (roundf((double)CUBE_SIZE / sin(angle)));
}

int	calc_x_dev(t_point pos, t_direction dir, int x_step)
{
	double	first_x_inter;

	first_x_inter = (pos.x >> MUL_512 << MUL_512);
	if (dir.x == est)
	{
		first_x_inter += CUBE_SIZE;
		return (roundf((first_x_inter - pos.x) / CUBE_SIZE * x_step));
	}
	else
		return (roundf((pos.x - first_x_inter) / CUBE_SIZE * x_step));
}

int	calc_y_dev(t_point pos, t_direction dir, int y_step)
{
	double	first_y_inter;

	first_y_inter = (pos.y >> MUL_512 << MUL_512);
	if (dir.y == south)
	{
		first_y_inter += CUBE_SIZE;
		return (roundf((first_y_inter - pos.y) / CUBE_SIZE * y_step));
	}
	else
		return (roundf((pos.y - first_y_inter) / CUBE_SIZE * y_step));
}
//
// #define RAD_TO_DEG(rad) (rad * (180 / M_PI))
// #include <stdio.h>
//
// static void	test_calc_x_step(double angle)
// {
// 	printf("calc_x_step(%f) == %d\n", RAD_TO_DEG(angle), calc_x_step(angle));
// }
// static void	test_calc_y_step(double angle)
// {
// 	printf("calc_y_step(%f) == %d\n", RAD_TO_DEG(angle), calc_y_step(angle));
// }
// static void	test_calc_x_dev(t_point pos, double angle)
// {
// 	int x_step = calc_x_step(angle);
// 	t_direction direction = calc_direction(angle);
// 	printf("calc_x_dev(%f) == %d\n", RAD_TO_DEG(angle), calc_x_dev(pos, direction, x_step));
// }
// static void	test_calc_y_dev(t_point pos, double angle)
// {
// 	int y_step = calc_y_step(angle);
// 	t_direction direction = calc_direction(angle);
// 	printf("calc_y_dev(%f) == %d\n", RAD_TO_DEG(angle), calc_y_dev(pos, direction, y_step));
// }
// static void	tests(t_point pos, double angle)
// {
// 	printf("---- %fdeg ----\n", RAD_TO_DEG(angle));
// 	test_calc_x_step(angle);
// 	test_calc_x_dev(pos, angle);
// 	test_calc_y_step(angle);
// 	test_calc_y_dev(pos, angle);
// }
//
// int	main(void)
// {
//
// 	t_point pos = {.x = 512 + 256, .y = 1024 + 256};
// 	printf("=========\nstart pos: x = %d ; y = %d\n", pos.x, pos.y);
//
// 	printf("-------- SYMMETRY\n");
// 	tests(pos, TURN_45 + 0.001);
// 	tests(pos, TURN_45 - 0.001);
// 	tests(pos, TURN_135 + 0.001);
// 	tests(pos, TURN_135 - 0.001);
// 	tests(pos, TURN_225 + 0.001);
// 	tests(pos, TURN_225 - 0.001);
// 	tests(pos, TURN_315 + 0.001);
// 	tests(pos, TURN_315 - 0.001);
// 	printf("-------- OTHERS\n");
// 	tests(pos, 0.01);
// 	tests(pos, TURN_90 - 0.01);
// 	tests(pos, 0.1);
// 	tests(pos, TURN_90 + 0.1);
// 	tests(pos, TURN_180 + 0.1);
// 	tests(pos, TURN_270 + 0.1);
// 	printf("-------- ZERO DIVISION\n");
// 	tests(pos, TURN_90);
// 	tests(pos, 0);
//
// 	return (0);
// }
