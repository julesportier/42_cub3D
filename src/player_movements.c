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

t_vec	move_forward(t_vec pos, t_vec dir_vec, t_map_data *map_data)
{
	t_vec	add;

	add.x = pos.x + (dir_vec.x * SPEED);
	add.y = pos.y + (dir_vec.y * SPEED);
	add = limit_displacement(add, dir_vec, pos, map_data);
	return (add);
}

t_vec	move_backward(t_vec pos, t_vec dir_vec, t_map_data *map_data)
{
	t_vec	add;

	add.x = pos.x - (dir_vec.x * SPEED);
	add.y = pos.y - (dir_vec.y * SPEED);
	add = limit_displacement(add, negate_vec(dir_vec), pos, map_data);
	return (add);
}

t_vec	strafe_left(t_vec pos, t_vec dir_vec, t_map_data *map_data)
{
	t_vec	strafe;
	t_vec	add;

	strafe.x = dir_vec.y;
	strafe.y = -dir_vec.x;
	add.x = pos.x + (strafe.x * SPEED);
	add.y = pos.y + (strafe.y * SPEED);
	add = limit_displacement(add, strafe, pos, map_data);
	return (add);
}

t_vec	strafe_right(t_vec pos, t_vec dir_vec, t_map_data *map_data)
{
	t_vec	strafe;
	t_vec	add;

	strafe.x = -dir_vec.y;
	strafe.y = dir_vec.x;
	add.x = pos.x + (strafe.x * SPEED);
	add.y = pos.y + (strafe.y * SPEED);
	add = limit_displacement(add, strafe, pos, map_data);
	return (add);
}
