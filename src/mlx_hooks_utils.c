/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:10:20 by juportie          #+#    #+#             */
/*   Updated: 2025/09/04 12:24:46 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "../minilibx/mlx.h"

int	end_loop_mouse(t_mlx_data *mlx_data)
{
	mlx_loop_end(mlx_data->mlx);
	return (0);
}
