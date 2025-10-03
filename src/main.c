/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/09/04 12:24:32 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "cube.h"
#include "../minilibx/mlx.h"
#include <math.h>

static t_vec	normalize_vec(t_vec vec)
{
	double	magnitude;

	magnitude = hypot(vec.x, vec.y);
	vec.x /= magnitude;
	vec.y /= magnitude;
	return (vec);
}

#include <stdio.h>
static t_vec	calc_dir_vec(double angle)
{
	t_direction	direction;
	t_vec		vec;

	direction = calc_direction(angle);
	vec.x = 1;
	vec.y = tan(scale_angle(angle));
	if (vec.y > 10)
		vec.y = 10;
	printf("dir vec.y == %f\n", vec.y);
	if (direction.x == west)
		vec.x *= -1;
	if (direction.y == north)
		vec.y *= -1;
	return (normalize_vec(vec));
}

// static t_vec	add_vec(t_vec vec_a, t_vec vec_b)
// {
// 	t_vec	add;
//
// 	add.x = vec_a.x + vec_b.x;
// 	add.y = vec_a.y + vec_b.y;
// 	return (add);
// }
//
static t_point	move_forward(t_point pos, t_vec vec_b)
{
	t_point	add;

	add.x = pos.x + (vec_b.x * SPEED);
	add.y = pos.y + (vec_b.y * SPEED);
	return (add);
}

static t_point	move_backward(t_point pos, t_vec vec_b)
{
	t_point	add;

	add.x = pos.x - (vec_b.x * SPEED);
	add.y = pos.y - (vec_b.y * SPEED);
	return (add);
}

// // #include <stdio.h>
// #define RAD_TO_DEG(rad) (rad * (180 / M_PI))
// static int	test_print(int keycode, t_mlx_data *mlx_data)
// {
// 	static t_point	pos = {.x = 512 * 2 + 256, .y = 512 * 6 + 256};
// 	char	**test_map = alloc_map();
// 	t_map_data map_data = {5, 10, test_map};
// 	// 90DEG PLAYER ANGLE CAUSE THE HEIGHT OF THE CENTER COLUMN BE INACCURATE (TOO SMALL)
// 	static double	player_angle = TURN_90 + 0.000001;
//
// 	if (keycode == ESC)
// 		mlx_loop_end(mlx_data->mlx);
// 	else if (keycode == A)
// 		player_angle = fmod((player_angle + 0.1), TURN_360);
// 	else if (keycode == D)
// 	{
// 		player_angle = fmod((player_angle - 0.1), TURN_360);
// 		if (player_angle < 0)
// 			player_angle = TURN_360;
// 	}
// 	else if (keycode == W)
// 		pos = move_forward(pos, calc_dir_vec(player_angle));
// 	else if (keycode == S)
// 		pos = move_backward(pos, calc_dir_vec(player_angle));
// 	printf("player angle == %fdeg\n", RAD_TO_DEG(player_angle));
// 	t_vec	dir_vec = calc_dir_vec(player_angle);
// 	printf("dir_vec .x == %f ; .y == %f\n", dir_vec.x, dir_vec.y);
// 	draw_ceiling_and_floor(&(mlx_data->img), 0x008db5bf, 0x00000000);
// 	cast_rays(mlx_data, map_data, pos, player_angle);
// 	return (0);
// }

// #define RAD_TO_DEG(rad) (rad * (180 / M_PI))
// static int	register_keypress(int keycode, t_keys_state *keys)
// {
// 	// printf("keypress\n");
// 	keys->changed = 1;
// 	if (keycode == ESC)
// 	{
// 		keys->esc = 1;
// 		printf("press esc\n");
// 		printf("keys->esc == %d\n", keys->esc);
// 	}
// 	else if (keycode == W)
// 	{
// 		keys->w = 1;
// 		printf("press w\n");
// 	}
// 	else if (keycode == A)
// 	{
// 		keys->a = 1;
// 		printf("press a\n");
// 	}
// 	else if (keycode == S)
// 	{
// 		keys->s = 1;
// 		printf("press s\n");
// 	}
// 	else if (keycode == D)
// 	{
// 		keys->d = 1;
// 		printf("press d\n");
// 	}
// 	return (0);
// }
//
// static int	register_keyrelease(int keycode, t_keys_state *keys)
// {
// 	// printf("keyrelease\n");
// 	keys->changed = 1;
// 	if (keycode == ESC)
// 	{
// 		keys->esc = 0;
// 		printf("release esc\n");
// 		printf("keys->esc == %d\n", keys->esc);
// 	}
// 	else if (keycode == W)
// 	{
// 		keys->w = 0;
// 		printf("release w\n");
// 	}
// 	else if (keycode == A)
// 	{
// 		keys->a = 0;
// 		printf("release a\n");
// 	}
// 	else if (keycode == S)
// 	{
// 		keys->s = 0;
// 		printf("release s\n");
// 	}
// 	else if (keycode == D)
// 	{
// 		keys->d = 0;
// 		printf("release d\n");
// 	}
// 	// if (keycode == ESC)
// 	// 	keys->esc = 0;
// 	// else if (keycode == W)
// 	// 	keys->w = 0;
// 	// else if (keycode == A)
// 	// 	keys->a = 0;
// 	// else if (keycode == S)
// 	// 	keys->s = 0;
// 	// else if (keycode == D)
// 	// 	keys->d = 0;
// 	return (0);
// }

// static int	loop_routine(t_prog_state *state)
// {
// 	// TESTING
// 	static t_point	pos = {.x = 512 * 2 + 256, .y = 512 * 6 + 256};
// 	char	**test_map = alloc_map();
// 	t_map_data map_data = {5, 10, test_map};
// 	// 90DEG PLAYER ANGLE CAUSE THE HEIGHT OF THE CENTER COLUMN BE INACCURATE (TOO SMALL)
// 	static double	player_angle = TURN_90 + 0.000001;
//
// 	// printf("state.keys.changed == %d\n", state->keys.changed);
// 	// printf("state.keys.w == %d\n", state->keys.w);
// 	// static int	i = -1;
// 	// if (!(++i % 1000))
// 	// 	printf("state.keys.esc == %d\n", state->keys.esc);
// 	if (state->keys.esc)
// 	{
// 		mlx_loop_end(state->mlx_data->mlx);
// 		return (0);
// 	}
// 	if (state->keys.a)
// 	{
// 		printf("a\n");
// 		player_angle = fmod((player_angle + ROT_SPEED), TURN_360);
// 	}
// 	if (state->keys.d)
// 	{
// 		printf("d\n");
// 		player_angle = fmod((player_angle - ROT_SPEED), TURN_360);
// 		if (player_angle < 0)
// 			player_angle = TURN_360;
// 	}
// 	if (state->keys.w)
// 	{
// 		printf("w\n");
// 		pos = move_forward(pos, calc_dir_vec(player_angle));
// 	}
// 	if (state->keys.s)
// 	{
// 		printf("s\n");
// 		pos = move_backward(pos, calc_dir_vec(player_angle));
// 	}
// 	if (state->keys.changed)
// 	{
// 		printf("player angle == %fdeg\n", RAD_TO_DEG(player_angle));
// 		t_vec	dir_vec = calc_dir_vec(player_angle);
// 		printf("dir_vec .x == %f ; .y == %f\n", dir_vec.x, dir_vec.y);
// 		draw_ceiling_and_floor(&(state->mlx_data->img), 0x008db5bf, 0x00000000);
// 		cast_rays(state->mlx_data, map_data, pos, player_angle);
// 		state->keys.changed = 0;
// 	}
// 	return (0);
// }

static void	init_keys_state(t_keys_state *keys)
{

	keys->w = 0;
	keys->a = 0;
	keys->s = 0;
	keys->d = 0;
	keys->esc = 0;
	keys->changed = 1;
}

static void	init_prog_state(t_prog_state *state, t_mlx_data *mlx_data)
{
	state->mlx_data = mlx_data;
	init_keys_state(&(state->keys));
}

static void	test_player_map_state(t_prog_state *state)
{
	state->map_data.width = 5;
	state->map_data.height = 10;
	state->map_data.map = alloc_map();
	state->player.pos.x = 512 * 2 + 256;
	state->player.pos.y = 512 * 6 + 256;
	// 90DEG PLAYER ANGLE CAUSE THE HEIGHT OF THE CENTER COLUMN BE INACCURATE (TOO SMALL)
	state->player.angle = TURN_90 + 0.000001;
}

#define RAD_TO_DEG(rad) (rad * (180 / M_PI))
static int	register_keypress(int keycode, t_prog_state *state)
{
	// printf("keypress\n");
	state->keys.changed = 1;
	if (keycode == ESC)
	{
		mlx_loop_end(state->mlx_data->mlx);
		return (0);
	}
	else if (keycode == W)
	{
		printf("press w\n");
		state->player.pos = move_forward(state->player.pos, calc_dir_vec(state->player.angle));
	}
	else if (keycode == A)
	{
		printf("press a\n");
		state->player.angle = fmod((state->player.angle + ROT_SPEED), TURN_360);
	}
	else if (keycode == S)
	{
		printf("press s\n");
		state->player.pos = move_backward(state->player.pos, calc_dir_vec(state->player.angle));
	}
	else if (keycode == D)
	{
		printf("press d\n");
		state->player.angle = fmod((state->player.angle - ROT_SPEED), TURN_360);
		if (state->player.angle < 0)
			state->player.angle = TURN_360;
	}
	printf("player angle == %fdeg\n", RAD_TO_DEG(state->player.angle));
	t_vec dir_vec = calc_dir_vec(state->player.angle);
	printf("dir_vec .x == %f ; .y == %f\n", dir_vec.x, dir_vec.y);
	draw_ceiling_and_floor(&(state->mlx_data->img), 0x008db5bf, 0x00000000);
	cast_rays(
			state->mlx_data,
			state->map_data,
			state->player.pos,
			state->player.angle);
	return (0);
}

int	main(void)
{
	t_mlx_data	mlx_data;

	if (allocate_mlx(&mlx_data, "cub3D"))
		return (-1);
	t_prog_state	state;

	init_prog_state(&state, &mlx_data);
	test_player_map_state(&state);
	printf("state.keys.changed == %d\n", state.keys.changed);
	// mlx_key_hook(mlx_data.win, end_loop_esc, &mlx_data);

	// t_point	pos = {.x = 512 * 3 + 256, .y = 512 * 4 + 256};
	// char	**test_map = alloc_map();
	// t_map_data map_data = {5, 6, test_map};
	// double	player_angle = TURN_90;
	// mlx_key_hook(mlx_data.win, test_print, &mlx_data);
	// cast_rays(&mlx_data, map_data, pos, player_angle);


	mlx_hook(mlx_data.win, ON_DESTROY, MASK_ESC, end_loop_mouse, &mlx_data);
	mlx_hook(mlx_data.win, ON_KEYPRESS, MASK_KEYPRESS, register_keypress, &state);
	// mlx_hook(mlx_data.win, ON_KEYPRESS, MASK_KEYPRESS, register_keypress, &state);
	// mlx_hook(mlx_data.win, ON_KEYRELEASE, MASK_KEYRELEASE, register_keyrelease, &state.keys);
	// mlx_loop_hook(mlx_data.mlx, loop_routine, &state);
	mlx_loop(mlx_data.mlx);
	free_mlx(&mlx_data);
	return (0);
}
