/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/09 11:52:44 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
# define RENDERING_H

// Field of view in radians (1rad ~ 57.3deg)
# define FOV 1
// Wall height in pixels,
// power of two to permit bitshift operations without loss.
# define CUBE_SIZE 512
# define PLAYER_HEIGHT 256
# define SHIFT_OP_512 9

typedef struct s_point
{
	int x;
	int y;
}	t_point;

typedef enum e_direction
{
	north,
	south,
	est,
	west
}	t_direction;

double	calc_angle_increment(int screen_width, double fov);
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
t_point	calc_y_vector(t_direction direction, double angle);
t_point	calc_x_vector(t_direction direction, double angle);

#endif
