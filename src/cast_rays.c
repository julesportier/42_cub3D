/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/10/23 11:45:17 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "../minilibx/mlx.h"
#include <math.h>

static t_texture	*get_texture(t_state *state, t_ray *ray, t_direction *dir)
{
	t_texture	*texture;

	if (ray->side == 'x')
	{
		if (dir->x == est)
			texture = &state->textures.est;
		else
			texture = &state->textures.west;
	}
	else
	{
		if (dir->y == north)
			texture = &state->textures.north;
		else
			texture = &state->textures.south;
	}
	return (texture);
}

static t_direction	calc_direction(t_vec vec)
{
	t_direction	direction;

	if (vec.y < 0)
		direction.y = north;
	else
		direction.y = south;
	if (vec.x < 0)
		direction.x = west;
	else
		direction.x = est;
	return (direction);
}

static int	calc_wall_height(double distance)
{
	static double	factor = (double)WIN_WIDTH * 0.83;

	return (round(factor / distance));
}

void	cast_rays(t_state *state)
{
	t_direction	dir;
	t_ray		ray;
	double		aim_pos;
	t_wall_data	wall;

	wall.x_pos = 0;
	while (wall.x_pos < WIN_WIDTH)
	{
		aim_pos = wall.x_pos * 2 / (double)WIN_WIDTH - 1;
		ray.vec = add_vec(
				state->player.dir,
				d_mul_vec(state->player.plane, aim_pos));
		dir = calc_direction(ray.vec);
		ray = calc_ray(state, dir, &ray);
		wall.texture = get_texture(state, &ray, &dir);
		wall.texture_x = get_texture_x(&ray, &state->player.pos, wall.texture);
		wall.height = calc_wall_height(ray.length);
		draw_column(state, &wall);
		++wall.x_pos;
	}
	mlx_put_image_to_window(
		state->mlx_data.mlx, state->mlx_data.win,
		state->mlx_data.img_data.img, 0, 0);
}
