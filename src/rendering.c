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
#include "cube.h"
#include <math.h>

int	calc_line_height(int distance)
{
	return (round((double)CUBE_SIZE / distance * (WIN_HEIGHT * 1)));
}

static int	calc_line_start(int line_height)
{
	if (line_height >= WIN_HEIGHT)
		return (0);
	else
		return ((WIN_HEIGHT - line_height) / 2);
}

static void draw_pixel(t_img_data *img_data, t_pixel pixel)
{
	char	*pix_addr;

	if (pixel.pos.y > WIN_HEIGHT || pixel.pos.y < 0
			|| pixel.pos.x > WIN_WIDTH || pixel.pos.x < 0)
		return ;
	pix_addr = img_data->addr + (
			pixel.pos.y * img_data->line_length
			+ pixel.pos.x * (img_data->bits_per_pixel / 8)
			);
	*(unsigned int *)pix_addr = pixel.color;
}

void	draw_column(
	t_img_data	*img_data,
	t_pixel		pixel,
	int			distance)
{
	int	i;
	int	line_pos;
	int	line_height;

	line_height = calc_line_height(distance);
	line_pos = calc_line_start(line_height);
	i = 0;
	while (i < line_height && i + line_pos < WIN_HEIGHT)
	{
		pixel.pos.y = line_pos + i;
		draw_pixel(img_data, pixel);
		++i;
	}
}
