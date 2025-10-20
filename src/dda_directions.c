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
 
t_direction     calc_direction(t_vec vec)
{
	t_direction	direction;

	if (vec.y < 0)
		direction.y = north;
	else
		direction.y = south;
	if (vec.x < 0)
		direction.x = west;
	else
		direction.x = est;
	return (direction);
}
