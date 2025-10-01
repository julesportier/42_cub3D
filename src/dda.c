/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/23 08:25:53 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "rendering.h"
#include "../minilibx/mlx.h"
#include <math.h>

static t_ray	dda(
	t_map_data	map_data,
	t_point		dist,
	t_point		cell,
	t_point		step,
	t_point		inc)
{
	char	side;
	t_ray	ray;

	while (map_data.map[cell.y][cell.x] != '1')
	{
		if (dist.x < dist.y)
		{
			dist.x += step.x;
			cell.x += inc.x;
			side = 'x';
		}
		else
		{
			dist.y += step.y;
			cell.y += inc.y;
			side = 'y';
		}
	}
	if (side == 'x')
	{
		ray.length = dist.x - step.x;
		ray.side = 'x';
	}
	else
	{
		ray.length = dist.y - step.y;
		ray.side = 'y';
	}
	return (ray);
}

t_ray	calc_ray_length(
	t_map_data	map_data,
	t_point		pos,
	t_direction	dir,
	double		angle)
{
	t_point	cell;
	t_point	step;
	t_point	dist;
	t_point	inc;

	cell.x = pos.x >> SHIFT_OP_512;
	cell.y = pos.y >> SHIFT_OP_512;
	step.x = calc_x_step(angle);
	step.y = calc_y_step(angle);
	dist.x = calc_x_dev(pos, dir, step.x);
	if (dir.x == est)
		inc.x = 1;
	else
		inc.x = -1;
	dist.y = calc_y_dev(pos, dir, step.y);
	if (dir.y == south)
		inc.y = 1;
	else
		inc.y = -1;
	return (dda(map_data, dist, cell, step, inc));
}


#include <stdio.h>
void	cast_rays(
	t_mlx_data *mlx_data,
	t_map_data map_data,
	t_point pos,
	double player_angle)
{
	double	angle_increment;
	double	angle;
	int		i;
	t_direction	dir;
	t_ray	ray;

	angle_increment = FOV / WIN_WIDTH;
	angle = player_angle - (FOV / 2);
	i = 0;
	while (i < WIN_WIDTH)
	{
		dir = calc_direction(angle);
		ray = calc_ray_length(map_data, pos, dir, angle);
		// Remove fishbowl effect
		ray.length = round(ray.length * cos(angle - player_angle));
		// printf("ray_length == %d\n", ray_length);
		// printf("line_height == %d\n", calc_line_height(ray_length));
		int	color;
		if (ray.side == 'x')
		{
			if (dir.x == est)
				color = 0x0000FF00;
			else
				color = 0x000000FF;
		}
		else
		{
			if (dir.y == north)
				color = 0x00FFFFFF;
			else
				color = 0x00FF0000;
		}
		draw_column(&(mlx_data->img), (t_pixel){{i, 0}, color}, ray.length);
		angle -= angle_increment;
		++i;
	}
	mlx_put_image_to_window(mlx_data->mlx, mlx_data->win, mlx_data->img.img, 0, 0);
}
#include <stdlib.h>

static void	fill_map_line(char **map, char *line, int width, int line_nbr)
{
	for (int i = 0; i < width; ++i)
		map[line_nbr][i] = line[i];
}

char	**alloc_map(void)
{
	int	w = 5;
	int h = 6;
	char	**map = malloc(sizeof(char *) * h);
	for (int i = 0; i < h; ++i)
		map[i] = malloc(sizeof(char) * w);

	// arrays size is w
	char line_0[] = {'1','1','1','1','1'};
	char line_1[] = {'1','0','0','0','1'};
	char line_2[] = {'1','0','0','0','1'};
	char line_3[] = {'1','0','0','0','1'};
	char line_4[] = {'1','0','0','0','1'};
	char line_5[] = {'1','1','1','1','1'};
	fill_map_line(map, line_0, w, 0);
	fill_map_line(map, line_1, w, 1);
	fill_map_line(map, line_2, w, 2);
	fill_map_line(map, line_3, w, 3);
	fill_map_line(map, line_4, w, 4);
	fill_map_line(map, line_5, w, 5);
	return (map);
}

//
// int	main(void)
// {
// 	t_point	pos = {.x = 1024 + 256, .y = 1024 + 256};
//
// 	char	**test_map = alloc_map();
// 	t_map_data map_data = {5, 5, test_map};
//
// 	// // Angled to the right/est
// 	// double	angle = TURN_45;
// 	// // t_point	first_wall = calc_first_wall(map_data, pos, angle);
// 	// t_direction dir = calc_direction(angle);
// 	// int	ray_length = calc_ray_length(map_data, pos, dir, calc_x_vector(dir, angle), calc_y_vector(dir, angle));
// 	// printf("ray_length == %d\n", ray_length);
// 	// // Angled to the left/west
// 	// angle = TURN_135 + 0.01;
// 	// // t_point	first_wall = calc_first_wall(map_data, pos, angle);
// 	// dir = calc_direction(angle);
// 	// ray_length = calc_ray_length(map_data, pos, dir, calc_x_vector(dir, angle), calc_y_vector(dir, angle));
// 	// printf("ray_length == %d\n", ray_length);
// 	cast_rays(map_data, pos, TURN_45);
//
//
//
// 	return (0);
// }
