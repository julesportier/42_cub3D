/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 07:56:03 by juportie          #+#    #+#             */
/*   Updated: 2025/09/23 09:28:37 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <math.h>

int	calc_wall_height(double distance)
{
	static double	factor = (double)WIN_WIDTH * 0.83;

	return (round(factor / distance));
}

static void draw_pixel(t_img_data *img_data, t_pixel pixel)
{
	char	*pix_addr;

	if (pixel.pos.y > WIN_HEIGHT || pixel.pos.y < 0
			|| pixel.pos.x > WIN_WIDTH || pixel.pos.x < 0)
		return ;
	pix_addr = img_data->data_addr + (
			pixel.pos.y * img_data->line_length
			+ pixel.pos.x * (img_data->bits_per_pixel / 8)
			);
	*(unsigned int *)pix_addr = pixel.color;
}

void	draw_vert_seg(t_img_data *img_data, t_pixel pixel, int end)
{
	if (pixel.pos.y > WIN_HEIGHT)
		return ;
	while (pixel.pos.y < end)
	{
		draw_pixel(img_data, pixel);
		++pixel.pos.y;
	}
}


static void	draw_ceilling(
	t_img_data *img_data,
	int wall_height,
	t_pixel	pixel)
{
	int	end;

	end = (WIN_HEIGHT - wall_height) * 0.5;
	pixel.pos.y = 0;
	draw_vert_seg(img_data, pixel, end);
}

static void	draw_floor(
	t_img_data *img_data,
	int wall_height,
	t_pixel	pixel)
{
	pixel.pos.y = WIN_HEIGHT - ((WIN_HEIGHT - wall_height) * 0.5);
	draw_vert_seg(img_data, pixel, WIN_HEIGHT);
}

static void	draw_wall(
	t_img_data	*img_data,
	int			wall_height,
	t_pixel		pixel,
	t_texture	*texture,
	t_ray		*ray,
	t_vec		*player_pos)
{
	int		i;
	double	wall_ratio;
	double	inc;
	int		texture_x;
	int		texture_start;

	texture_x = get_texture_x(ray, player_pos, texture);
	wall_ratio = (double)WIN_HEIGHT / wall_height;
	inc = wall_ratio * texture->height_ratio;
	if (wall_height < WIN_HEIGHT)
	{
		pixel.pos.y = (WIN_HEIGHT - wall_height) * 0.5;
		texture_start = 0;
	}
	else
	{
		pixel.pos.y = 0;
		texture_start = (1 - wall_ratio) * 0.5 * texture->height;
	}
	i = 0;
	while (i < wall_height && i < WIN_HEIGHT)
	{
		pixel.color = get_texture_color(texture, texture_x, texture_start + i * inc);
		draw_pixel(img_data, pixel);
		++i;
		++pixel.pos.y;
	}
};

void	draw_column(
	t_img_data	*img_data,
	int			x_pos,
	t_ray		*ray,
	t_vec		*player_pos,
	t_texture	*texture,
	int			ceiling_color,
	int			floor_color)
{
	int		wall_height;

 	wall_height = calc_wall_height(ray->length);
	if (wall_height < WIN_HEIGHT)
		draw_ceilling(img_data, wall_height, (t_pixel){ .pos.x = x_pos, .color = ceiling_color });

	draw_wall(
		img_data,
		wall_height,
		(t_pixel){.pos.x = x_pos, .color = 0x00FFFFFF},
		texture,
		ray,
		player_pos);

	if (wall_height < WIN_HEIGHT)
		draw_floor(img_data, wall_height, (t_pixel){ .pos.x = x_pos, .color = floor_color });
}
