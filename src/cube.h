/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:18:35 by juportie          #+#    #+#             */
/*   Updated: 2025/09/04 12:24:22 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

// Window size
# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
// mlx
# define ON_DESTROY 17
# define ESC 65307

// MLX data structures
typedef struct s_img_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img_data;

typedef struct s_mlx_data
{
	void		*mlx;
	void		*win;
	t_img_data	img;
}	t_mlx_data;

// free.c
void	free_mlx(t_mlx_data *mlx_data);
// mlx_alloc.c
int		allocate_mlx(t_mlx_data *mlx_data, char *title);
// mlx_hooks_utils.c
int		end_loop_mouse(t_mlx_data *mlx_data);
int		end_loop_esc(int keycode, t_mlx_data *mlx_data);

#endif
