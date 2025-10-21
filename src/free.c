/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:34:05 by juportie          #+#    #+#             */
/*   Updated: 2025/09/04 11:01:06 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "parsing.h"
#include "rendering.h"
#include "../minilibx/mlx.h"
#include <stdlib.h>

void	free_mlx(t_mlx_data *mlx_data)
{
	if (mlx_data->img_data.img != NULL)
		mlx_destroy_image(mlx_data->mlx, mlx_data->img_data.img);
	if (mlx_data->win != NULL)
		mlx_destroy_window(mlx_data->mlx, mlx_data->win);
	if (mlx_data->mlx != NULL)
	{
		mlx_destroy_display(mlx_data->mlx);
		free(mlx_data->mlx);
	}
}
