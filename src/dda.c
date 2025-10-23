/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/10/15 12:43:41 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <math.h>

static t_vec	calc_steps(t_vec ray_vec)
{
	t_vec	steps;

	steps.x = fabs(1 / ray_vec.x);
	steps.y = fabs(1 / ray_vec.y);
	return (steps);
}

static double	calc_dda_start_dist(
	t_cardinal dir,
	double cell,
	double pos,
	double step)
{
	if (dir == est || dir == south)
		return ((cell + 1 - pos) * step);
	else
		return ((pos - cell) * step);
}

static t_ray	calc_ray_len(char side, t_ray *ray, t_dda_data *dda_data)
{
	if (side == 'x')
	{
		ray->length = dda_data->dist.x - dda_data->step.x;
		ray->side = 'x';
	}
	else
	{
		ray->length = dda_data->dist.y - dda_data->step.y;
		ray->side = 'y';
	}
	return (*ray);
}

static t_ray	dda(t_map_data map_data, t_ray *ray, t_dda_data *dda_data)
{
	char	side;

	while (map_data.map[dda_data->cell.y][dda_data->cell.x] != '1')
	{
		if (dda_data->dist.x < dda_data->dist.y)
		{
			dda_data->dist.x += dda_data->step.x;
			dda_data->cell.x += dda_data->inc.x;
			side = 'x';
		}
		else
		{
			dda_data->dist.y += dda_data->step.y;
			dda_data->cell.y += dda_data->inc.y;
			side = 'y';
		}
	}
	return (calc_ray_len(side, ray, dda_data));
}

t_ray	calc_ray(t_state *state, t_direction dir, t_ray *ray)
{
	t_dda_data	dda_data;

	dda_data.cell.x = state->player.pos.x;
	dda_data.cell.y = state->player.pos.y;
	dda_data.step = calc_steps(ray->vec);
	if (dir.x == est)
		dda_data.inc.x = 1;
	else
		dda_data.inc.x = -1;
	if (dir.y == south)
		dda_data.inc.y = 1;
	else
		dda_data.inc.y = -1;
	dda_data.dist.x = calc_dda_start_dist(
			dir.x, dda_data.cell.x, state->player.pos.x, dda_data.step.x);
	dda_data.dist.y = calc_dda_start_dist(
			dir.y, dda_data.cell.y, state->player.pos.y, dda_data.step.y);
	return (dda(state->map, ray, &dda_data));
}
