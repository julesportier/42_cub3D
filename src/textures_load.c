/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_load.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/10 15:01:28 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "../libft/src/libft.h"
#include "../minilibx/mlx.h"

int	cache_texture(t_mlx_data *mlx_data, t_texture *texture)
{
	texture->img = mlx_xpm_file_to_image(
			mlx_data->mlx,
			texture->filename,
			&texture->width,
			&texture->height);
	if (!texture->img)
	{
		ft_putendl_fd("error loading texture\n", 2);
		return (-1);
	}
	return (0);
}

int	load_textures(t_mlx_data *mlx_data, t_textures *textures)
{
	if (cache_texture(mlx_data, &textures->north))
		return (-1);
	if (cache_texture(mlx_data, &textures->south))
		return (-1);
	if (cache_texture(mlx_data, &textures->est))
		return (-1);
	if (cache_texture(mlx_data, &textures->west))
		return (-1);
	return (0);
}
