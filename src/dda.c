/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/10/15 12:43:41 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "rendering.h"
#include "../minilibx/mlx.h"
#include <math.h>
		#include <stdio.h>

static void	dda(
	t_map_data	map_data,
	t_ray		*ray,
	t_vec		dist,
	t_point		cell,
	t_vec		step,
	t_point		inc)
{
	char	side;

	// Add protection (normally useless) to avoid segfaults with array access
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
		ray->length = dist.x - step.x;
		ray->side = 'x';
	}
	else
	{
		ray->length = dist.y - step.y;
		ray->side = 'y';
	}
}

void	calc_ray(
	t_map_data	map_data,
	t_vec		pos,
	t_direction	dir,
	t_ray		*ray)
{
	t_point	cell;
	t_vec	step;
	t_vec	dist;
	t_point	inc;

	cell.x = pos.x;
	// printf("cell.x == %d\n", cell.x);
	cell.y = pos.y;
	// printf("cell.y == %d\n", cell.y);
	step = calc_steps(ray->vec);
	// print_vec("steps", step);
	// dist = calc_initial_dev(pos, dir);
	if (dir.x == est)
	{
		inc.x = 1;
		dist.x = (cell.x + 1 - pos.x) * step.x;
	}
	else
	{
		inc.x = -1;
		dist.x = (pos.x - cell.x) * step.x;
	}
	if (dir.y == south)
	{
		inc.y = 1;
		dist.y = (cell.y + 1 - pos.y) * step.y;
	}
	else
	{
		inc.y = -1;
		dist.y = (pos.y - cell.y) * step.y;
	}
	dda(map_data, ray, dist, cell, step, inc);
}


void	cast_rays(t_state *state)
{
	int		i;
	t_direction	dir;
	t_ray	ray;
	double	aim_pos;
	t_texture	*texture;

	i = 0;
	while (i < WIN_WIDTH)
	{
		aim_pos = i * 2 / (double)WIN_WIDTH - 1;
		// ray_vec = d_mul_vec(add_vec(player_dir, plane_vec), hit_pos);
		ray.vec = add_vec(state->player.dir, d_mul_vec(state->player.plane, aim_pos));
		// print_vec("ray.vec", ray.vec);
		dir = calc_direction(ray.vec);
		// printf("dir x == %d y == %d\n", dir.x, dir.y);
		calc_ray(state->map, state->player.pos, dir, &ray);
		// printf("ray_length == %f\n", ray.length);
		// printf("line_height == %d\n", calc_line_height(ray_length));
		if (ray.side == 'x')
		{
			if (dir.x == est)
				texture = &state->textures.est;
			else
				texture = &state->textures.west;
		}
		else
		{
			if (dir.y == north)
				texture = &state->textures.north;
			else
				texture = &state->textures.south;
		}
		draw_column(&(state->mlx.img), i, &ray, &state->player.pos, texture, state->colors.ceiling, state->colors.floor);
		++i;
	}
	mlx_put_image_to_window(state->mlx.mlx, state->mlx.win, state->mlx.img.img, 0, 0);
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
