/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:36:27 by juportie          #+#    #+#             */
/*   Updated: 2025/10/23 13:28:09 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "../libft/src/libft.h"
#include "../minilibx/mlx.h"
#include <unistd.h>

static int	init(t_mlx_data *mlx_data)
{
	mlx_data->mlx = mlx_init();
	if (mlx_data->mlx == NULL)
	{
		ft_putendl_fd("Error:\nmlx_init error", 2);
		return (-1);
	}
	return (0);
}

static int	new_window(t_mlx_data *mlx_data, char *title)
{
	mlx_data->win = mlx_new_window(mlx_data->mlx, WIN_WIDTH, WIN_HEIGHT, title);
	if (mlx_data->win == NULL)
	{
		perror("Error:\nmlx_new_window");
		free_mlx(mlx_data);
		return (-1);
	}
	return (0);
}

static int	new_image(t_mlx_data *mlx_data)
{
	mlx_data->img_data.img = mlx_new_image(
			mlx_data->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (mlx_data->img_data.img == NULL)
	{
		perror("Error:\nmlx_new_image");
		free_mlx(mlx_data);
		return (-1);
	}
	return (0);
}

static int	get_data_addr(t_mlx_data *mlx_data)
{
	mlx_data->img_data.data_addr = mlx_get_data_addr(
			mlx_data->img_data.img, &mlx_data->img_data.bits_per_pixel,
			&mlx_data->img_data.line_length, &mlx_data->img_data.endian);
	if (mlx_data->img_data.data_addr == NULL)
	{
		perror("Error:\nmlx_get_data_addr");
		free_mlx(mlx_data);
		return (-1);
	}
	return (0);
}

int	allocate_mlx(t_mlx_data *mlx_data, char *title)
{
	if (init(mlx_data))
		return (-1);
	if (new_window(mlx_data, title))
		return (-1);
	if (new_image(mlx_data))
		return (-1);
	if (get_data_addr(mlx_data))
		return (-1);
	return (0);
}
