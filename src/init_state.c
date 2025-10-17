/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 08:26:38 by juportie          #+#    #+#             */
/*   Updated: 2025/10/16 09:28:32 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <stdio.h>

static t_player	init_player(void)
{
	t_player	player;

	player.pos.x = 2.5;
	player.pos.y = 5.5;
	player.dir.x = -1;
	player.dir.y = 0;
	player.plane.x = 0;
	player.plane.y = 0.6;
	return (player);
}

int	init_state(t_state	*state)
{
	if (allocate_mlx(&state->mlx, "cub3D"))
		return (-1);
	char	**test_map = alloc_map();
	t_map_data map_data = {5, 10, test_map};
	state->map = map_data;
	// FOR TESTING
	state->textures.north.filename = "textures/Wooden_Shingles_SPEC.xpm";
	state->textures.south.filename = "textures/Mossy_Cobblestone_DIFF2.xpm";
	state->textures.est.filename = "textures/Potato_normal.xpm";
	state->textures.west.filename = "textures/Planks_01_DISP.xpm";
	if (load_textures(&state->mlx, &state->textures))
	{
		free_mlx(&state->mlx);
		return (-1);
	}
	printf("north texture width : %d\n", state->textures.north.width);
	printf("north texture color at 0 0 : %d\n", get_texture_color(&state->textures.north, 0, 0));
	state->player = init_player();
	return (0);
}
