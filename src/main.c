/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/16 09:49:49 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "cube.h"
#include "../minilibx/mlx.h"
#include <math.h>

// #include <stdio.h>
#define RAD_TO_DEG(rad) (rad * (180 / M_PI))
static int	test_print(int keycode, t_state *state)
{
	if (keycode == ESC)
		mlx_loop_end(state->mlx.mlx);
	else if (keycode == LEFT)
	{
		state->player.dir = rotate_vec(state->player.dir, ROT_SPEED);
		state->player.plane = rotate_vec(state->player.plane, ROT_SPEED);
	}
	else if (keycode == RIGHT)
	{
		state->player.dir = rotate_vec(state->player.dir, -ROT_SPEED);
		state->player.plane = rotate_vec(state->player.plane, -ROT_SPEED);
	}
	else if (keycode == W)
		state->player.pos = move_forward(state->player.pos, state->player.dir, &state->map);
	else if (keycode == S)
		state->player.pos = move_backward(state->player.pos, state->player.dir, &state->map);
	else if (keycode == A)
		state->player.pos = strafe_left(state->player.pos, state->player.dir, &state->map);
	else if (keycode == D)
		state->player.pos = strafe_right(state->player.pos, state->player.dir, &state->map);

	// printf("player angle == %fdeg\n", RAD_TO_DEG(player_angle));
	// t_vec	dir_vec = calc_dir_vec(player_angle);
	// print_vec("state->player.dir", state->player.dir);
	// print_vec("state->player.plane", state->player.plane);
	// draw_ceiling_and_floor(&(state->mlx.img), 0x008db5bf, 0x00000000);
	cast_rays(&state->mlx, state->map, state->player.pos, state->player.dir, state->player.plane, &state->textures);
	return (0);
}

int	main(void)
{
	t_state		state;

	if (init_state(&state))
		return (-1);
	mlx_hook(state.mlx.win, ON_DESTROY, 1L << 3, end_loop_mouse, &state.mlx);
	mlx_key_hook(state.mlx.win, end_loop_esc, &state.mlx);

	mlx_key_hook(state.mlx.win, test_print, &state);

	mlx_loop(state.mlx.mlx);
	free_mlx(&state.mlx);
	return (0);
}
