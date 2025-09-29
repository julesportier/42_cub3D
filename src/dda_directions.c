/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_directions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/29 12:56:56 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <math.h>
 
t_direction     calc_direction(double angle)
{
	t_direction	direction;

	if (angle < TURN_180)
		direction.y = north;
	else
		direction.y = south;
	if (angle > TURN_90 && angle < TURN_270)
		direction.x = west;
	else
		direction.x = est;
	return (direction);
}

double	scale_angle(double angle)
{
	if (angle >= TURN_90 && angle < TURN_180)
		angle = TURN_180 - angle;
	else if (angle >= TURN_180 && angle < TURN_270)
		angle = angle - TURN_180;
	else if (angle >= TURN_270 && angle < TURN_360)
		angle = TURN_360 - angle;
	return (angle);
}
