/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/23 08:25:53 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "rendering.h"
#include "../minilibx/mlx.h"
#include <math.h>

double	calc_angle_increment(int screen_width, double fov)
{
	return (fov / screen_width);
}

int	calc_ray_length(
	t_map_data	map_data,
	t_point		pos,
	t_direction	dir,
	t_point		vector_x,
	t_point		vector_y,
	double		angle)
	// double		player_angle)
{
	// printf("pos.x == %d, pos.y == %d\n", pos.x, pos.y);
	int	map_x = pos.x >> SHIFT_OP_512;
	int	map_y = pos.y >> SHIFT_OP_512;
	printf("map_x == %d, map_y == %d\n", map_x, map_y);
	double	delta_x = hypot(vector_x.x, vector_x.y);
	double	delta_y = hypot(vector_y.x, vector_y.y);
	// printf("delta_x == %f, delta_y == %f\n", delta_x, delta_y);
	int	dist_x;
	int	dist_y;
	int	step_x;
	int	step_y;
	char	side;

	dist_x = calc_line_dist(pos, calc_first_vertical_inter(pos, dir, angle));
	if (dir.x == est)
		step_x = 1;
	else
		step_x = -1;
	dist_y = calc_line_dist(pos, calc_first_horizontal_inter(pos, dir, angle));
	if (dir.y == south)
		step_y = 1;
	else
		step_y = -1;


	while (1)
	{
		if (dist_x < dist_y)
		{
			dist_x += delta_x;
			map_x += step_x;
			side = 'x';
		}
		else
		{
			dist_y += delta_y;
			map_y += step_y;
			side = 'y';
		}

		if (map_data.map[map_y][map_x] == '1')
		{
			// printf("is wall\n");
			if (side == 'x')
				return (dist_x);
			else
				return (dist_y);
		}
	}
}

#include <stdio.h>
void	cast_rays(
	t_mlx_data *mlx_data,
	t_map_data map_data,
	t_point pos,
	double player_angle)
{
	double	angle_increment;
	double	angle;
	int		i;
	t_direction	dir;
	int	ray_length;

	angle_increment = FOV / WIN_WIDTH;
	angle = player_angle - (FOV / 2);
	i = 0;
	while (i < WIN_WIDTH)
	{
		dir = calc_direction(angle);
		ray_length = calc_ray_length(
				map_data, pos, dir,
				calc_vertical_vector(dir, angle),
				calc_horizontal_vector(dir, angle),
				angle);
		// Remove fishbowl effect
		ray_length = round(ray_length * cos(angle - player_angle));
		printf("ray_length == %d\n", ray_length);
		printf("line_height == %d\n", calc_line_height(ray_length));
		draw_line(&(mlx_data->img), (t_pixel){{i, 0}, 0x00FFFFFF}, ray_length);
		angle += angle_increment;
		++i;
	}
	mlx_put_image_to_window(mlx_data->mlx, mlx_data->win, mlx_data->img.img, 0, 0);
}
#include <stdlib.h>

static void	fill_map_line(char **map, char *line, int width, int line_nbr)
{
	for (int i = 0; i < width; ++i)
		map[line_nbr][i] = *(line + i);
}

char	**alloc_map(void)
{
	int	w = 5;
	int h = 6;
	char	**map = malloc(sizeof(char *) * h);
	for (int i = 0; i < h; ++i)
		map[i] = malloc(sizeof(char) * w);

	// arrays size is w
	char line_0[] = {'1','1','1','1','1'};
	char line_1[] = {'1','0','1','0','1'};
	char line_2[] = {'1','0','0','0','1'};
	char line_3[] = {'1','0','0','0','1'};
	char line_4[] = {'1','0','0','0','1'};
	char line_5[] = {'1','1','1','1','1'};
	fill_map_line(map, line_0, w, 0);
	fill_map_line(map, line_1, w, 1);
	fill_map_line(map, line_2, w, 2);
	fill_map_line(map, line_3, w, 3);
	fill_map_line(map, line_4, w, 4);
	fill_map_line(map, line_5, w, 5);
	return (map);
}

//
// int	main(void)
// {
// 	t_point	pos = {.x = 1024 + 256, .y = 1024 + 256};
//
// 	char	**test_map = alloc_map();
// 	t_map_data map_data = {5, 5, test_map};
//
// 	// // Angled to the right/est
// 	// double	angle = TURN_45;
// 	// // t_point	first_wall = calc_first_wall(map_data, pos, angle);
// 	// t_direction dir = calc_direction(angle);
// 	// int	ray_length = calc_ray_length(map_data, pos, dir, calc_x_vector(dir, angle), calc_y_vector(dir, angle));
// 	// printf("ray_length == %d\n", ray_length);
// 	// // Angled to the left/west
// 	// angle = TURN_135 + 0.01;
// 	// // t_point	first_wall = calc_first_wall(map_data, pos, angle);
// 	// dir = calc_direction(angle);
// 	// ray_length = calc_ray_length(map_data, pos, dir, calc_x_vector(dir, angle), calc_y_vector(dir, angle));
// 	// printf("ray_length == %d\n", ray_length);
// 	cast_rays(map_data, pos, TURN_45);
//
//
//
// 	return (0);
// }
