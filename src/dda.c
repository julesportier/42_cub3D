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
	t_vec		dist,
	t_point		cell,
	t_vec		step,
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
	t_vec		pos,
	t_direction	dir,
	t_vec		ray_vec)
{
	t_point	cell;
	t_vec	step;
	t_vec	dist;
	t_point	inc;

	cell.x = pos.x;
	cell.y = pos.y;
	step = calc_steps(ray_vec);
	print_vec("steps", step);
	// dist = calc_initial_dev(pos, dir);
	if (dir.x == est)
	{
		inc.x = 1;
		dist.x = cell.x + 1 - pos.x * step.x;
	}
	else
	{
		inc.x = -1;
		dist.x = pos.x - cell.x * step.x;
	}
	if (dir.y == south)
	{
		inc.y = 1;
		dist.y = cell.y + 1 - pos.y * step.y;
	}
	else
	{
		inc.y = -1;
		dist.y = pos.y - cell.y * step.y;
	}
	return (dda(map_data, dist, cell, step, inc));
}


#include <stdio.h>
void	cast_rays(
	t_mlx_data *mlx_data,
	t_map_data map_data,
	t_vec pos,
	t_vec player_dir,
	t_vec plane_vec)
{
	int		i;
	t_direction	dir;
	t_ray	ray;
	double	hit_pos;
	t_vec	ray_vec;

	i = 0;
	while (i <= WIN_WIDTH)
	{
		hit_pos = i * 2 / (double)WIN_WIDTH - 1;
		ray_vec = d_mul_vec(add_vec(player_dir, plane_vec), hit_pos);
		print_vec("ray_vec", ray_vec);
		dir = calc_direction(ray_vec);
		// printf("dir x == %d y == %d\n", dir.x, dir.y);
		ray = calc_ray_length(map_data, pos, dir, ray_vec);
		// printf("ray len == %d\n", ray.length);
		// Remove fishbowl effect
		// ray.length = round(ray.length * cos(angle - player_angle));
		// printf("ray_length == %d\n", ray_length);
		// printf("line_height == %d\n", calc_line_height(ray_length));
		int	color;
		if (ray.side == 'x')
		{
			if (dir.x == est)
				color = GREEN;
			else
				color = BLUE;
		}
		else
		{
			if (dir.y == north)
				color = WHITE;
			else
				color = RED;
		}
		draw_column(&(mlx_data->img), (t_pixel){{i, 0}, color}, ray.length);
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
	int h = 10;
	char	**map = malloc(sizeof(char *) * h);
	for (int i = 0; i < h; ++i)
		map[i] = malloc(sizeof(char) * w);

	// arrays size is w
	char line_0[] = {'1','1','1','1','1'};
	char line_1[] = {'1','1','0','0','1'};
	char line_2[] = {'1','0','1','0','1'};
	char line_3[] = {'1','0','0','0','1'};
	char line_4[] = {'1','0','0','0','1'};
	char line_5[] = {'1','0','0','0','1'};
	char line_6[] = {'1','0','0','0','1'};
	char line_7[] = {'1','0','0','0','1'};
	char line_8[] = {'1','0','0','0','1'};
	char line_9[] = {'1','1','1','1','1'};
	fill_map_line(map, line_0, w, 0);
	fill_map_line(map, line_1, w, 1);
	fill_map_line(map, line_2, w, 2);
	fill_map_line(map, line_3, w, 3);
	fill_map_line(map, line_4, w, 4);
	fill_map_line(map, line_5, w, 5);
	fill_map_line(map, line_6, w, 6);
	fill_map_line(map, line_7, w, 7);
	fill_map_line(map, line_8, w, 8);
	fill_map_line(map, line_9, w, 9);
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
