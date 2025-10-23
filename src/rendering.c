/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 07:56:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/23 11:32:54 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

static void	draw_ceilling(t_img_data *img_data, int wall_height, t_pixel pixel)
{
	int	end;

	end = (WIN_HEIGHT - wall_height) * 0.5;
	pixel.pos.y = 0;
	draw_vert_seg(img_data, pixel, end);
}

static void	draw_floor(t_img_data *img_data, int wall_height, t_pixel pixel)
{
	pixel.pos.y = WIN_HEIGHT - ((WIN_HEIGHT - wall_height) * 0.5);
	draw_vert_seg(img_data, pixel, WIN_HEIGHT);
}

static void	calc_wall_start(
	t_wall_data *wall,
	t_pixel *pixel,
	int *texture_start)
{
	if (wall->height < WIN_HEIGHT)
	{
		pixel->pos.y = (WIN_HEIGHT - wall->height) * 0.5;
		*texture_start = 0;
	}
	else
	{
		pixel->pos.y = 0;
		*texture_start = (1 - (double)WIN_HEIGHT / wall->height)
			* 0.5 * wall->texture->height;
	}
}

static void	draw_wall(t_img_data *img_data, t_wall_data *wall)
{
	int		i;
	double	inc;
	int		texture_start;
	t_pixel	pixel;

	inc = (double)WIN_HEIGHT / wall->height * wall->texture->height_ratio;
	pixel.pos.x = wall->x_pos;
	calc_wall_start(wall, &pixel, &texture_start);
	i = 0;
	while (i < wall->height && i < WIN_HEIGHT)
	{
		pixel.color = get_texture_color(
				wall->texture, wall->texture_x, texture_start + i * inc);
		draw_pixel(img_data, pixel);
		++i;
		++pixel.pos.y;
	}
}

void	draw_column(t_state *state, t_wall_data *wall)
{
	if (wall->height < WIN_HEIGHT)
		draw_ceilling(
			&state->mlx_data.img_data,
			wall->height,
			(t_pixel){.pos.x = wall->x_pos, .color = state->colors.ceiling});
	draw_wall(&state->mlx_data.img_data, wall);
	if (wall->height < WIN_HEIGHT)
		draw_floor(
			&state->mlx_data.img_data,
			wall->height,
			(t_pixel){.pos.x = wall->x_pos, .color = state->colors.floor});
}
