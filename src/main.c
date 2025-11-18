/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/11/18 16:30:03 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "parsing.h"
#include "../minilibx/mlx.h"

static int	keys_actions(int keycode, t_state *state)
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
		state->player.pos = move_forward(state);
	else if (keycode == S)
		state->player.pos = move_backward(state);
	else if (keycode == A)
		state->player.pos = strafe_left(state);
	else if (keycode == D)
		state->player.pos = strafe_right(state);
	cast_rays(state);
	return (0);
}

int	main(int argc, char *argv[])
{
	const char	*path;
	t_parsed	parsed;
	t_state		state;

	if (argc != 2)
	{
		ft_putendl_fd("Error\nInvalid number of arguments", 2);
		return (EXIT_FAILURE);
	}
	if (validate_params(argv, &path))
		return (EXIT_FAILURE);
	if (parsing_load(argv[1], &parsed))
		return (EXIT_FAILURE);
	if (init_state(&state, &parsed))
		return (-1);
	cast_rays(&state);
	mlx_hook(
		state.mlx_data.win, ON_DESTROY, 0, end_loop_mouse, &state.mlx_data);
	mlx_hook(
		state.mlx_data.win, ON_KEYPRESS, MASK_KEYPRESS, keys_actions, &state);
	mlx_loop(state.mlx_data.mlx);
	free_state(&state, &parsed);
	return (0);
}
