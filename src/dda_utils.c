/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/29 13:02:34 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include <math.h>
 
t_vec	calc_steps(t_vec ray_vec)
{
	t_vec	steps;

	// steps.x = sqrt(1 + ((ray_vec.x * ray_vec.x) / (ray_vec.y * ray_vec.y)));
	// steps.y = sqrt(1 + ((ray_vec.y * ray_vec.y) / (ray_vec.x * ray_vec.x)));
	steps.x = fabs(1 / ray_vec.x);
	steps.y = fabs(1 / ray_vec.y);
	return (steps);
}
//
// t_vec	calc_initial_dev(t_point pos, t_direction dir)
// {
// 	t_vec	dev;
//
// 	if (dir.x == est)
// 		dev.x = 1 - (pos.x % CUBE_SIZE / (double)CUBE_SIZE);
// 	else
// 		dev.x = pos.x % CUBE_SIZE / (double)CUBE_SIZE;
// 	if (dir.y == south)
// 		dev.y = 1 - (pos.y % CUBE_SIZE / (double)CUBE_SIZE);
// 	else
// 		dev.y = 1 - (pos.y % CUBE_SIZE / (double)CUBE_SIZE);
// 	return (dev);
// }
