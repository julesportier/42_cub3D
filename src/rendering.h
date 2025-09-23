/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/23 07:41:06 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
# define RENDERING_H

// Field of view in radians (1rad ~ 57.3deg)
# define FOV 1.0
// Wall height in pixels,
// power of two to permit bitshift operations without loss.
# define CUBE_SIZE 512
# define PLAYER_HEIGHT 256
# define SHIFT_OP_512 9
// radians turns
# define TURN_45 M_PI_4
# define TURN_90 M_PI_2
# define TURN_135 (M_PI_2 + M_PI_4)
# define TURN_180 M_PI
# define TURN_225 (M_PI + M_PI_4)
# define TURN_270 (M_PI + M_PI_2)
# define TURN_315 (M_PI + M_PI_2 + M_PI_4)
# define TURN_360 0

typedef struct	s_map_data
{
	int		width;
	int		height;
	char	**map;
}	t_map_data;

typedef struct s_point
{
	int x;
	int y;
}	t_point;

typedef enum e_cardinal
{
	north,
	south,
	est,
	west,
}	t_cardinal;

typedef struct e_direction
{
	t_cardinal	x;
	t_cardinal	y;
}	t_direction;

// dda_utils.c
t_direction	calc_direction(double angle);
t_point	calc_y_vector(t_direction direction, double angle);
t_point	calc_x_vector(t_direction direction, double angle);
// NO MORE USAGE ??
t_point	calc_first_y_intersection(
	t_point pos,
	t_direction direction,
	double angle
);
t_point	calc_first_x_intersection(
	t_point pos,
	t_direction direction,
	double angle
);
//******************

// dda.c
double	calc_angle_increment(int screen_width, double fov);
int	calc_ray_length(
	t_map_data	map_data,
	t_point		pos,
	t_direction	dir,
	t_point		vector_x,
	t_point		vector_y);
void	cast_rays(t_map_data map_data, t_point pos, double player_angle);
// NO MORE USAGE ??
int	is_inside_map(t_point pos, t_map_data map_data);
int	is_wall(char *map[], t_point pos);
t_point	calc_first_side_wall(
	t_map_data map_data,
	t_point	first_intersection,
	t_point vector);
t_point	calc_first_wall(t_map_data map_data, t_point pos, double angle);
//******************

#endif
