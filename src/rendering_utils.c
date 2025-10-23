/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 07:56:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/23 11:26:49 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

void	draw_pixel(t_img_data *img_data, t_pixel pixel)
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
