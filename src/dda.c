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

#include "rendering.h"
#include "../minilibx/mlx.h"

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
	cell.y = pos.y;
	step = calc_steps(ray->vec);
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
		ray.vec = add_vec(state->player.dir, d_mul_vec(state->player.plane, aim_pos));
		dir = calc_direction(ray.vec);
		calc_ray(state->map, state->player.pos, dir, &ray);
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
		draw_column(&(state->mlx_data.img_data), i, &ray, &state->player.pos, texture, state->colors.ceiling, state->colors.floor);
		++i;
	}
	mlx_put_image_to_window(state->mlx_data.mlx, state->mlx_data.win, state->mlx_data.img_data.img, 0, 0);
}
