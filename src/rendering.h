/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/09/29 12:58:04 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
# define RENDERING_H

# include "cube.h"

// Field of view in radians (1rad ~ 57.3deg)
# define FOV 1.0
// Wall height in pixels,
// power of two to permit bitshift operations without loss.
# define CUBE_SIZE 512
# define SPEED 0.0000001
# define ROT_SPEED 0.000001
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
# define TURN_360 (M_PI * 2)

// mlx keycodes
# define ESC 65307
# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
// mlx events
# define ON_DESTROY 17
# define ON_KEYPRESS 2
# define ON_KEYRELEASE 3
// mlx masks
# define MASK_KEYPRESS 1L << 0
# define MASK_KEYRELEASE 1L << 1
# define MASK_ESC 1L << 3

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

typedef struct s_point t_step;

typedef struct s_vec
{
	double x;
	double y;
}	t_vec;

typedef struct s_pixel
{
	t_point	pos;
	int		color;
}	t_pixel;

typedef enum e_cardinal
{
	north,
	south,
	est,
	west,
}	t_cardinal;

typedef struct s_direction
{
	t_cardinal	x;
	t_cardinal	y;
}	t_direction;

typedef struct	s_ray
{
	int	length;
	char	side;
	t_cardinal	wall;
}	t_ray;

typedef struct	s_keys_state
{
	char	w;
	char	a;
	char	s;
	char	d;
	char	esc;
	char	changed;
}	t_keys_state;

typedef struct	s_prog_state
{
	t_mlx_data		*mlx_data;
	t_map_data		*map_data;
	t_keys_state	keys;
}	t_prog_state;

// dda_directions.c
t_direction     calc_direction(double angle);
double	scale_angle(double angle);
// dda_utils.c
int	calc_x_step(double angle);
int	calc_y_step(double angle);
int	calc_x_dev(t_point pos, t_direction dir, int x_step);
int	calc_y_dev(t_point pos, t_direction dir, int y_step);

// dda.c
t_ray	calc_ray_length(
	t_map_data	map_data,
	t_point		pos,
	t_direction	dir,
	double		angle);
void	cast_rays(
	t_mlx_data *mlx_data,
	t_map_data map_data,
	t_point pos,
	double player_angle);
// FOR TESTING
char	**alloc_map(void);

// rendering.c
int	calc_line_height(int distance);
void	draw_column(
	t_img_data	*img_data,
	t_pixel		pixel,
	int			distance);
void	draw_ceiling_and_floor(
	t_img_data	*img_data,
	int		ceiling_color,
	int		floor_color);

#endif
