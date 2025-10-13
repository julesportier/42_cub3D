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
	char	**test_map = alloc_map();
	t_map_data map_data = {5, 10, test_map};
	// FACE NORTH ?
	static t_vec	player_pos = {.x = 2, .y = 6};
	static t_vec	player_dir = { .x = -1, .y = 0 };
	static t_vec	plane_vec = { .x = 0, .y = 0.6 };

	if (keycode == ESC)
		mlx_loop_end(mlx_data->mlx);
	else if (keycode == A)
	{
		player_dir = rotate_vec(player_dir, ROT_SPEED);
		plane_vec = rotate_vec(plane_vec, ROT_SPEED);
	}
	else if (keycode == D)
	{
		player_dir = rotate_vec(player_dir, -ROT_SPEED);
		plane_vec = rotate_vec(plane_vec, -ROT_SPEED);
	}
	// else if (keycode == W)
	// 	player_pos = move_forward(player_pos, player_dir, &map_data);
	// else if (keycode == S)
	// 	player_pos = move_backward(player_pos, player_dir, &map_data);
# include <stdio.h>
	// printf("player angle == %fdeg\n", RAD_TO_DEG(player_angle));
	// t_vec	dir_vec = calc_dir_vec(player_angle);
	// print_vec("player_dir", player_dir);
	print_vec("plane_vec", plane_vec);
	draw_ceiling_and_floor(&(mlx_data->img), 0x008db5bf, 0x00000000);
	cast_rays(mlx_data, map_data, player_pos, player_dir, plane_vec);
	return (0);
}

int	main(void)
{
	t_mlx_data	mlx_data;

	if (allocate_mlx(&mlx_data, "cub3D"))
		return (-1);
	mlx_hook(mlx_data.win, ON_DESTROY, 1L << 3, end_loop_mouse, &mlx_data);
	mlx_key_hook(mlx_data.win, end_loop_esc, &mlx_data);

	mlx_key_hook(mlx_data.win, test_print, &mlx_data);

	mlx_loop(mlx_data.mlx);
	free_mlx(&mlx_data);
	return (0);
}
