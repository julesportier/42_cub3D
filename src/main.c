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

int	main(void)
{
	t_mlx_data	mlx_data;

	if (allocate_mlx(&mlx_data, "cub3D"))
		return (-1);
	mlx_hook(mlx_data.win, ON_DESTROY, 1L << 3, end_loop_mouse, &mlx_data);
	mlx_key_hook(mlx_data.win, end_loop_esc, &mlx_data);

	t_point	pos = {.x = 512 * 3 + 256, .y = 512 * 4 + 256};
	char	**test_map = alloc_map();
	t_map_data map_data = {5, 6, test_map};
	double	angle = TURN_45;
	cast_rays(&mlx_data, map_data, pos, angle);

	mlx_loop(mlx_data.mlx);
	free_mlx(&mlx_data);
	return (0);
}
