/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movements.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:36:03 by juportie          #+#    #+#             */
/*   Updated: 2025/10/09 10:26:32 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

static t_vec	limit_displacement(
	t_vec new_pos,
	t_vec dir_vec,
	t_vec last_pos,
	t_map_data *map_data)
{
	double	collision;

	collision = COLLISION;
	if (dir_vec.x < 0)
		collision *= -1;
	if (map_data->map[(int)new_pos.y][(int)(new_pos.x + collision)] == '1')
		new_pos.x = last_pos.x;
	collision = COLLISION;
	if (dir_vec.y < 0)
		collision *= -1;
	if (map_data->map[(int)(new_pos.y + collision)][(int)new_pos.x] == '1')
		new_pos.y = last_pos.y;
	return (new_pos);
}

t_vec	move_forward(t_state *state)
{
	t_vec	add;

	add.x = state->player.pos.x + (state->player.dir.x * SPEED);
	add.y = state->player.pos.y + (state->player.dir.y * SPEED);
	add = limit_displacement(
			add, state->player.dir, state->player.pos, &state->map);
	return (add);
}

t_vec	move_backward(t_state *state)
{
	t_vec	add;

	add.x = state->player.pos.x - (state->player.dir.x * SPEED);
	add.y = state->player.pos.y - (state->player.dir.y * SPEED);
	add = limit_displacement(
			add, negate_vec(state->player.dir), state->player.pos, &state->map);
	return (add);
}

t_vec	strafe_left(t_state *state)
{
	t_vec	strafe;
	t_vec	add;

	strafe.x = state->player.dir.y;
	strafe.y = -state->player.dir.x;
	add.x = state->player.pos.x + (strafe.x * SPEED);
	add.y = state->player.pos.y + (strafe.y * SPEED);
	add = limit_displacement(add, strafe, state->player.pos, &state->map);
	return (add);
}

t_vec	strafe_right(t_state *state)
{
	t_vec	strafe;
	t_vec	add;

	strafe.x = -state->player.dir.y;
	strafe.y = state->player.dir.x;
	add.x = state->player.pos.x + (strafe.x * SPEED);
	add.y = state->player.pos.y + (strafe.y * SPEED);
	add = limit_displacement(add, strafe, state->player.pos, &state->map);
	return (add);
}
