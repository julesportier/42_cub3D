/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/16 09:45:46 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "../libft/src/libft.h"
#include "../minilibx/mlx.h"
#include <math.h>
#include <stdio.h>

int	texture_to_image(t_mlx_data *mlx_data, t_texture *texture)
{
	texture->img = mlx_xpm_file_to_image(
			mlx_data->mlx,
			texture->filename,
			&texture->width,
			&texture->height);
	if (!texture->img)
	{
		ft_putendl_fd("Error:\nloading texture failed\n", 2);
		return (-1);
	}
	texture->img->addr = mlx_get_data_addr(
			&texture->img->img, &texture->img->bits_per_pixel,
			&texture->img->line_length, &texture->img->endian);
	return (0);
}

int	load_textures(t_mlx_data *mlx_data, t_textures *textures)
{
	if (texture_to_image(mlx_data, &textures->north))
		return (-1);
	if (texture_to_image(mlx_data, &textures->south))
		return (-1);
	if (texture_to_image(mlx_data, &textures->est))
		return (-1);
	if (texture_to_image(mlx_data, &textures->west))
		return (-1);
	return (0);
}

int	get_texture_color(t_texture *texture, int x, int y)
{
	if (y > texture->height || y < 0
			|| x > texture->width || x < 0)
		return (-1);
	return (*(int *)(texture->img->addr + (
			y * texture->img->line_length
			+ x * (texture->img->bits_per_pixel / 8))));
}

int	get_texture_x(t_ray *ray, t_vec *player_pos, t_texture *texture)
{
	double	hit_point;
	int 	texture_x_pos;

	if (ray->side == 'x')
		hit_point = player_pos->y + ray->length * ray->vec.y;
	else
		hit_point = player_pos->x + ray->length * ray->vec.x;
	// print_vec("ray->vec", ray->vec);
	// printf("ray->length == %f\n", ray->length);
	// printf("hit_point == %f ; ", hit_point);
	hit_point -= floor(hit_point);
	// printf("hit_point floored == %f\n", hit_point);
	// Calculate position on texture
	texture_x_pos = hit_point * texture->width;
	if ((ray->side == 'x' && ray->vec.x > 0)
			|| (ray->side == 'y' && ray->vec.y < 0))
		texture_x_pos = texture->width - texture_x_pos - 1;
	return (texture_x_pos);
}
