/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/09 10:31:14 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "cube.h"
#include "../minilibx/mlx.h"
#include <math.h>

// #include <stdio.h>
#define RAD_TO_DEG(rad) (rad * (180 / M_PI))
static int	test_print(int keycode, t_mlx_data *mlx_data)
{
	static t_point	pos = {.x = 512 * 2 + 256, .y = 512 * 6 + 256};
	char	**test_map = alloc_map();
	t_map_data map_data = {5, 10, test_map};
	// 90DEG PLAYER ANGLE CAUSE THE HEIGHT OF THE CENTER COLUMN BE INACCURATE (TOO SMALL)
	static double	player_angle = TURN_90 + 0.000001;

	if (keycode == ESC)
		mlx_loop_end(mlx_data->mlx);
	else if (keycode == A)
		player_angle = fmod((player_angle + 0.1), TURN_360);
	else if (keycode == D)
	{
		player_angle = fmod((player_angle - 0.1), TURN_360);
		if (player_angle < 0)
			player_angle = TURN_360;
	}
	else if (keycode == W)
		pos = move_forward(pos, calc_dir_vec(player_angle), &map_data);
	else if (keycode == S)
		pos = move_backward(pos, calc_dir_vec(player_angle), &map_data);
# include <stdio.h>
	printf("player angle == %fdeg\n", RAD_TO_DEG(player_angle));
	t_vec	dir_vec = calc_dir_vec(player_angle);
	printf("dir_vec .x == %f ; .y == %f\n", dir_vec.x, dir_vec.y);
	draw_ceiling_and_floor(&(mlx_data->img), 0x008db5bf, 0x00000000);
	cast_rays(mlx_data, map_data, pos, player_angle);
	return (0);
}

int	main(void)
{
	t_mlx_data	mlx_data;

	if (allocate_mlx(&mlx_data, "cub3D"))
		return (-1);
	mlx_hook(mlx_data.win, ON_DESTROY, 1L << 3, end_loop_mouse, &mlx_data);
	mlx_key_hook(mlx_data.win, end_loop_esc, &mlx_data);

	// t_point	pos = {.x = 512 * 3 + 256, .y = 512 * 4 + 256};
	// char	**test_map = alloc_map();
	// t_map_data map_data = {5, 6, test_map};
	// double	player_angle = TURN_90;
	mlx_key_hook(mlx_data.win, test_print, &mlx_data);
	// cast_rays(&mlx_data, map_data, pos, player_angle);

	mlx_loop(mlx_data.mlx);
	free_mlx(&mlx_data);
	return (0);
}
