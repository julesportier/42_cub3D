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
#include "parsing.h"
#include "../minilibx/mlx.h"
#include <math.h>

// #include <stdio.h>
#define RAD_TO_DEG(rad) (rad * (180 / M_PI))
static int	test_print(int keycode, t_state *state)
{
	if (keycode == ESC)
		mlx_loop_end(state->mlx_data.mlx);
	else if (keycode == LEFT)
	{
		state->player.dir = rotate_vec(state->player.dir, -ROT_SPEED);
		state->player.plane = rotate_vec(state->player.plane, -ROT_SPEED);
	}
	else if (keycode == RIGHT)
	{
		state->player.dir = rotate_vec(state->player.dir, ROT_SPEED);
		state->player.plane = rotate_vec(state->player.plane, ROT_SPEED);
	}
	else if (keycode == W)
		state->player.pos = move_forward(state->player.pos, state->player.dir, &state->map);
	else if (keycode == S)
		state->player.pos = move_backward(state->player.pos, state->player.dir, &state->map);
	else if (keycode == A)
		state->player.pos = strafe_left(state->player.pos, state->player.dir, &state->map);
	else if (keycode == D)
		state->player.pos = strafe_right(state->player.pos, state->player.dir, &state->map);
	cast_rays(state);
	return (0);
}

int	main(int argc, char *argv[])
{
	if (argc != 2)
	{
        print_perr(PERR_ARGC, NULL);
        return (EXIT_FAILURE);
	}

	t_parsed	parsed;
	t_perr		err;

	// PRINT THE ERROR 
	err = parsing_load(argv[1], &parsed);
    if (err != PERR_OK)
	{
        print_perr(err, argv[1]);
        return (EXIT_FAILURE);
    }

	t_state		state;

	if (init_state(&state, &parsed))
		return (-1);
	mlx_hook(state.mlx_data.win, ON_DESTROY, MASK_ESC, end_loop_mouse, &state.mlx_data);
	mlx_hook(state.mlx_data.win, ON_KEYPRESS, MASK_KEYPRESS, test_print, &state);
	mlx_loop(state.mlx_data.mlx);
	free_state(&state, &parsed);
	return (0);
}
