/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/23 07:40:27 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "rendering.h"
#include "vector_operations.h"
#include <math.h>

double	calc_angle_increment(int screen_width, double fov)
{
	return (fov / screen_width);
}

int	is_inside_map(t_point pos, t_map_data map_data)
{
	pos.x >>= SHIFT_OP_512;
	pos.y >>= SHIFT_OP_512;
	if (pos.x < 0 || pos.x >= map_data.width
			|| pos.y < 0 || pos.y >= map_data.height)
		return (0);
	else
		return (1);
}

#include <stdio.h>
int	is_wall(char *map[], t_point pos)
{
	printf("pos.x >> SHIFT_OP_512 == %d\n", pos.x >> SHIFT_OP_512);
	printf("pos.y >> SHIFT_OP_512 == %d\n", pos.y >> SHIFT_OP_512);
	return (map[pos.y >> SHIFT_OP_512][pos.x >> SHIFT_OP_512] == '1');
}

int	calc_ray_length(
	t_map_data	map_data,
	t_point		pos,
	t_direction	dir,
	t_point		vector_x,
	t_point		vector_y)
{
	// printf("pos.x == %d, pos.y == %d\n", pos.x, pos.y);
	int	map_x = pos.x >> SHIFT_OP_512;
	int	map_y = pos.y >> SHIFT_OP_512;
	double	delta_x = hypot(vector_x.x, vector_x.y);
	double	delta_y = hypot(vector_y.x, vector_y.y);
	// printf("delta_x == %f, delta_y == %f\n", delta_x, delta_y);
	int	side_dist_x;
	int	side_dist_y;
	int	step_x;
	int	step_y;
	char	side;
	if (dir.x == est)
	{
		side_dist_x = ((double)CUBE_SIZE
				/ ((pos.x >> SHIFT_OP_512 << SHIFT_OP_512) + CUBE_SIZE - pos.x))
				* delta_x;
		step_x = 1;
	}
	else
	{
		side_dist_x = ((double)CUBE_SIZE
				/ (pos.x - (pos.x >> SHIFT_OP_512 << SHIFT_OP_512)))
				* delta_x;
		step_x = -1;
	}
	if (dir.y == south)
	{
		side_dist_y = ((double)CUBE_SIZE
				/ ((pos.y >> SHIFT_OP_512 << SHIFT_OP_512) + CUBE_SIZE - pos.y))
				* delta_y;
		step_y = 1;
	}
	else
	{
		side_dist_y = ((double)CUBE_SIZE
				/ (pos.y - (pos.y >> SHIFT_OP_512 << SHIFT_OP_512)))
				* delta_y;
		step_y = -1;
	}


	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_x;
			map_x += step_x;
			side = 'x';
		}
		else
		{
			side_dist_y += delta_y;
			map_y += step_y;
			side = 'y';
		}

		if (map_data.map[map_y][map_x] == '1')
		{
			// printf("is wall\n");
			if (side == 'x')
			{
				// TO REMOVE THE FISHBOWL EFFECT NOT SURE WITH THIS IMPLEMENTATION
				return (side_dist_x - delta_x);
			}
			else
				return (side_dist_y - delta_y);
		}
	}
}

t_point	calc_first_side_wall(
	t_map_data map_data,
	t_point	first_intersection,
	t_point vector)
{
	t_point	pos;

	pos = first_intersection;
	printf("pos.x == %d, pos.y == %d\n", pos.x, pos.y);
	while (is_inside_map(pos, map_data))
	{
		if (is_wall(map_data.map, pos))
		{
			printf("is wall\n");
			return (pos);
		}
		pos = add_vec(pos, vector);
		printf("after add_vec\n");
		printf("pos.x == %d, pos.y == %d\n", pos.x, pos.y);
	}
	printf("OUTSIDE MAP\n");
	pos.x = 0;
	pos.y = 0;
	return (pos);
}

// t_point	calc_first_wall(
// 	char *map[],
// 	t_point	pos,
// 	t_point	first_x_intersection,
// 	t_point	first_y_intersection,
// 	t_point vector_x,
// 	t_point vector_y)
// {
// 	t_point	wall_y;
// 	t_point	wall_x;
// 	t_point	dist_y;
// 	t_point	dist_x;
//
// 	wall_y = calc_first_side_wall(map, first_y_intersection, vector_y);
// 	wall_x = calc_first_side_wall(map, first_x_intersection, vector_x);
// 	dist_y = calc_dist(pos, wall_y);
// 	dist_x = calc_dist(pos, wall_x);
// 	if (is_inferior(dist_y, dist_x))
// 		return (wall_y);
// 	else
// 		return (wall_x);
// }

t_point	calc_first_wall(t_map_data map_data, t_point pos, double angle)
{
	t_point	wall_y;
	t_point	wall_x;
	t_point	dist_y;
	t_point	dist_x;
	t_direction	dir;

	dir = calc_direction(angle);
	wall_x = calc_first_side_wall(
				map_data,
				calc_first_x_intersection(pos, dir, angle),
				calc_x_vector(dir, angle));
	wall_y = calc_first_side_wall(
				map_data,
				calc_first_y_intersection(pos, dir, angle),
				calc_y_vector(dir, angle));
	dist_x = calc_dist(pos, wall_x);
	dist_y = calc_dist(pos, wall_y);
	if (is_inferior(dist_y, dist_x))
		return (wall_y);
	else
		return (wall_x);
}

// #include <stdio.h>
#include <stdlib.h>

static void	fill_map_line(char **map, char *line, int width, int line_nbr)
{
	for (int i = 0; i < width; ++i)
		map[line_nbr][i] = *(line + i);
}

static char	**alloc_map(void)
{
	int	w = 5;
	int h = 5;
	char	**map = malloc(sizeof(char *) * h);
	for (int i = 0; i < h; ++i)
		map[i] = malloc(sizeof(char) * w);

	// arrays size is w
	char line_0[] = {'1','1','1','1','1'};
	char line_1[] = {'1','0','0','0','1'};
	char line_2[] = {'1','0','0','0','1'};
	char line_3[] = {'1','0','0','0','1'};
	char line_4[] = {'1','1','1','1','1'};
	fill_map_line(map, line_0, w, 0);
	fill_map_line(map, line_1, w, 1);
	fill_map_line(map, line_2, w, 2);
	fill_map_line(map, line_3, w, 3);
	fill_map_line(map, line_4, w, 4);
	return (map);
}

void	cast_rays(t_map_data map_data, t_point pos, double player_angle)
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
				calc_x_vector(dir, angle),
				calc_y_vector(dir, angle));
		printf("ray_length == %d\n", ray_length);
		angle += angle_increment;
		++i;
	}
}

int	main(void)
{
	t_point	pos = {.x = 1024 + 256, .y = 1024 + 256};

	char	**test_map = alloc_map();
	t_map_data map_data = {5, 5, test_map};

	// // Angled to the right/est
	// double	angle = TURN_45;
	// // t_point	first_wall = calc_first_wall(map_data, pos, angle);
	// t_direction dir = calc_direction(angle);
	// int	ray_length = calc_ray_length(map_data, pos, dir, calc_x_vector(dir, angle), calc_y_vector(dir, angle));
	// printf("ray_length == %d\n", ray_length);
	// // Angled to the left/west
	// angle = TURN_135 + 0.01;
	// // t_point	first_wall = calc_first_wall(map_data, pos, angle);
	// dir = calc_direction(angle);
	// ray_length = calc_ray_length(map_data, pos, dir, calc_x_vector(dir, angle), calc_y_vector(dir, angle));
	// printf("ray_length == %d\n", ray_length);
	cast_rays(map_data, pos, TURN_45);



	return (0);
}
