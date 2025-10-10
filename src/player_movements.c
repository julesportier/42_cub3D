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

static t_point	limit_displacement(
	t_point new_pos,
	t_vec	dir_vec,
	t_point last_pos,
	t_map_data *map_data)
{
	int		collision;
	t_point	grid_pos;

	grid_pos.x = new_pos.x >> MUL_512;
	grid_pos.y = new_pos.y >> MUL_512;
	collision = COLLISION;
	if (dir_vec.x < 0)
		collision *= -1;
	collision = (new_pos.x + collision) >> MUL_512;
	if (map_data->map[grid_pos.y][collision] == '1')
		new_pos.x = last_pos.x;
	collision = COLLISION;
	if (dir_vec.y < 0)
		collision *= -1;
	collision = (new_pos.y + collision) >> MUL_512;
	if (map_data->map[collision][grid_pos.x] == '1')
		new_pos.y = last_pos.y;
	return (new_pos);
}

t_point	move_forward(t_point pos, t_vec dir_vec, t_map_data *map_data)
{
	t_point	add;

	add.x = pos.x + (dir_vec.x * SPEED);
	add.y = pos.y + (dir_vec.y * SPEED);
	add = limit_displacement(add, dir_vec, pos, map_data);
	return (add);
}

t_point	move_backward(t_point pos, t_vec dir_vec, t_map_data *map_data)
{
	t_point	add;

	add.x = pos.x - (dir_vec.x * SPEED);
	add.y = pos.y - (dir_vec.y * SPEED);
	add = limit_displacement(add, dir_vec, pos, map_data);
	return (add);
}
