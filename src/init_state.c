/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 08:26:38 by juportie          #+#    #+#             */
/*   Updated: 2025/10/20 15:11:56 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

static t_player	init_player(t_player_init player_init)
{
	t_player	player;

	player.pos.x = player_init.column + 0.5;
	player.pos.y = player_init.row + 0.5;
	// HANDLE PLAYER.DIR
	player.dir.x = 1;
	player.dir.y = 0;
	player.plane.x = 0;
	player.plane.y = 0.6;
	return (player);
}

static int	rgb_to_int(t_rgb rgb)
{
	int	color;

	color = 0;
	color = rgb.r;
	color = color << 8;
	color |= rgb.g;
	color = color << 8;
	color |= rgb.b;
	return (color);
}

static t_colors	init_colors(t_config config)
{
	t_colors	colors;

	colors.ceiling = rgb_to_int(config.ceil_rgb);
	colors.floor = rgb_to_int(config.floor_rgb);
	return (colors);
}

int	init_state(t_state	*state, t_parsed *parsed)
{
	if (allocate_mlx(&state->mlx_data, "cub3D"))
		return (-1);
	state->map.map = parsed->map.grid;
	state->map.width = parsed->map.columns;
	state->map.height = parsed->map.rows;
	state->textures.north.filename = parsed->config.no;
	state->textures.south.filename = parsed->config.so;
	state->textures.est.filename = parsed->config.we;
	state->textures.west.filename = parsed->config.ea;
	if (load_textures(&state->mlx_data, &state->textures))
	{
		parsing_free(parsed);
		free_mlx(&state->mlx_data);
		return (-1);
	}
	state->player = init_player(parsed->map.player);
	state->colors = init_colors(parsed->config);
	return (0);
}
