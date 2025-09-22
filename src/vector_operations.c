/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:53:08 by juportie          #+#    #+#             */
/*   Updated: 2025/09/18 14:47:04 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector_operations.h"
#include <stdlib.h>

#include <stdio.h>
t_point	add_vec(t_point vec1, t_point vec2)
{
	t_point	res_vec;

	res_vec.x = vec1.x + vec2.x;
	printf("vec2.x == %d ; res_vec.x == %d\n", vec2.x, res_vec.x);
	res_vec.y = vec1.y + vec2.y;
	printf("vec2.y == %d ; res_vec.y == %d\n", vec2.y, res_vec.y);
	return (res_vec);
}

t_point	sub_vec(t_point vec1, t_point vec2)
{
	t_point	res_vec;

	res_vec.x = vec1.x - vec2.x;
	res_vec.y = vec1.y - vec2.y;
	return (res_vec);
}

t_point calc_dist(t_point vec1, t_point vec2)
{
	t_point	dist;

	dist = sub_vec(vec2, vec1);
	dist.x = abs(dist.x);
	dist.y = abs(dist.y);
	return (dist);
}

int	is_inferior(t_point a, t_point b)
{
	if (abs(a.x) < abs(b.x) || abs(a.y) < abs(b.y))
		return (1);
	else
		return (0);
}
