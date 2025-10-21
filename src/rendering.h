/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:08:18 by juportie          #+#    #+#             */
/*   Updated: 2025/10/16 09:44:44 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
# define RENDERING_H

# include "cube.h"
# include "parsing.h"

// Wall height in pixels,
// power of two to permit bitshift operations without loss.
# define COLLISION 0.25
# define SPEED 0.1
# define ROT_SPEED 0.1

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
	t_vec		vec;
	double		length;
	char		side;
	t_cardinal	wall;
}	t_ray;

typedef struct	s_texture
{
	int			width;
	int			height;
	double		height_ratio;
	char		*filename;
	t_img_data	img_data;
}	t_texture;

typedef struct s_textures
{
	t_texture	north;
	t_texture	south;
	t_texture	est;
	t_texture	west;
}	t_textures;

typedef struct	s_player
{
	t_vec	pos;
	t_vec	dir;
	t_vec	plane;
}	t_player;

typedef struct	s_colors
{
	int	ceiling;
	int	floor;
}	t_colors;

typedef struct	s_state
{
	t_mlx_data	mlx_data;
	t_map_data	map;
	t_textures	textures;
	t_player	player;
	t_colors	colors;
}	t_state;

// init_state.c
int	init_state(t_state *state, t_parsed *parsed);
// dda_directions.c
t_direction     calc_direction(t_vec vec);
// dda_utils.c
t_vec	calc_steps(t_vec ray_vec);

// dda.c
void	calc_ray(
	t_map_data	map_data,
	t_vec		pos,
	t_direction	dir,
	t_ray		*ray);
void	cast_rays(t_state *state);
// FOR TESTING
char	**alloc_map(void);

// rendering.c
int	calc_wall_height(double distance);
void	draw_column(
	t_img_data	*img_data,
	int			x_pos,
	t_ray		*ray,
	t_vec		*player_pos,
	t_texture	*texture,
	int			ceiling_color,
	int			floor_color);
void	draw_ceiling_and_floor(
	t_img_data	*img_data,
	int		ceiling_color,
	int		floor_color);

// textures.c
int		texture_to_image(t_mlx_data *mlx_data, t_texture *texture);
int		load_textures(t_mlx_data *mlx_data, t_textures *textures);
int		get_texture_color(t_texture *texture, int x, int y);
int		get_texture_x(t_ray *ray, t_vec *player_pos, t_texture *texture);

// player_movements.c
t_vec	move_forward(t_vec pos, t_vec dir_vec, t_map_data *map_data);
t_vec	move_backward(t_vec pos, t_vec dir_vec, t_map_data *map_data);
t_vec	strafe_left(t_vec pos, t_vec dir_vec, t_map_data *map_data);
t_vec	strafe_right(t_vec pos, t_vec dir_vec, t_map_data *map_data);

// vector_operations.c
void	print_point(char *name, t_point point);
void	print_vec(char *name, t_vec vec);
t_vec	negate_vec(t_vec vec);
t_vec	normalize_vec(t_vec vec);
// t_vec	calc_dir_vec(double angle);
t_vec	rotate_vec(t_vec vec, double angle);
t_vec	add_vec(t_vec vec_a, t_vec vec_b);
t_vec	d_mul_vec(t_vec vec, double mul);

// free.c
void	free_mlx(t_mlx_data *mlx_data);
void	free_state(t_state *state, t_parsed *parsed);

#endif
