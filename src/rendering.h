/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/10/09 10:30:08 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
# define RENDERING_H

# include "cube.h"

// Wall height in pixels,
// power of two to permit bitshift operations without loss.
# define CUBE_SIZE 512
# define COLLISION 128
# define SPEED 300
# define ROT_SPEED 0.1
# define MUL_512 9

# define RED 0x00FF0000
# define GREEN 0x0000FF00
# define BLUE 0x000000FF
# define WHITE 0x00FFFFFF

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
	double	length;
	char	side;
	t_cardinal	wall;
}	t_ray;

// dda_directions.c
t_direction     calc_direction(t_vec vec);
// dda_utils.c
t_vec	calc_steps(t_vec ray_vec);
// t_vec	calc_initial_dev(t_point pos, t_direction dir);

// dda.c
t_ray	calc_ray_length(
	t_map_data	map_data,
	t_vec		pos,
	t_direction	dir,
	t_vec		ray_vec);
void	cast_rays(
	t_mlx_data *mlx_data,
	t_map_data map_data,
	t_vec pos,
	t_vec player_dir,
	t_vec plane_vec);
// FOR TESTING
char	**alloc_map(void);

// rendering.c
int	calc_line_height(double distance);
void	draw_column(
	t_img_data	*img_data,
	t_pixel		pixel,
	double		distance);
void	draw_ceiling_and_floor(
	t_img_data	*img_data,
	int		ceiling_color,
	int		floor_color);

// player_movements.c
t_point	move_forward(t_point pos, t_vec dir_vec, t_map_data *map_data);
t_point	move_backward(t_point pos, t_vec dir_vec, t_map_data *map_data);

// vector_operations.c
void	print_point(char *name, t_point point);
void	print_vec(char *name, t_vec vec);
t_vec	negate_vec(t_vec vec);
t_vec	normalize_vec(t_vec vec);
// t_vec	calc_dir_vec(double angle);
t_vec	rotate_vec(t_vec vec, double angle);
t_vec	add_vec(t_vec vec_a, t_vec vec_b);
t_vec	d_mul_vec(t_vec vec, double mul);

#endif
