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

static void	free_textures(t_state *state)
{
	if (state->textures.north.img_data.img)
		mlx_destroy_image(
			state->mlx_data.mlx, state->textures.north.img_data.img);
	if (state->textures.south.img_data.img)
		mlx_destroy_image(
			state->mlx_data.mlx, state->textures.south.img_data.img);
	if (state->textures.est.img_data.img)
		mlx_destroy_image(
			state->mlx_data.mlx, state->textures.est.img_data.img);
	if (state->textures.west.img_data.img)
		mlx_destroy_image(
			state->mlx_data.mlx, state->textures.west.img_data.img);
}

void	free_state(t_state *state, t_parsed *parsed)
{
	free_textures(state);
	parsing_free(parsed);
	free_mlx(&state->mlx_data);
}
