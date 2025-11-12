/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:18:06 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/11 19:17:41 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/src/libft.h"
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>

/* ================== Data ================== */

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef enum e_perr
{
	PERR_OK = 0,
	PERR_ARGC,
	PERR_EMPTY,
	PERR_EXT,
//	PERR_DIR,
	PERR_OPEN,
	PERR_READ,
	PERR_ALLOC,
	PERR_EL_DUP,
	PERR_EL_MISS,
	PERR_ID_BAD,
	PERR_RGB_BAD,
	PERR_FILE_EMPTY,
	PERR_PATH_MISS
}	t_perr;

typedef enum e_id
{
	ID_NONE = 0,
	ID_NO,
	ID_SO,
	ID_WE,
	ID_EA,
	ID_F,
	ID_C,
	ID_UNKNOWN
}	t_id;

typedef struct s_rgb
{
	t_bool	is_set;
	int		r;
	int		g;
	int		b;
}	t_rgb;

typedef struct s_config
{
	t_rgb	floor_rgb;
	t_rgb	ceil_rgb;
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
}	t_config;

typedef struct s_player_init
{
	char	dir;
	int		row;
	int		column;
}	t_player_init;

typedef struct s_map
{
	t_player_init	player;
	char			**grid;
	int				rows;
	int				columns;
}	t_map;

typedef struct s_mapbuild
{
	t_player_init	player;
	size_t			len;
	size_t			capacity;
	t_bool			started;
	t_bool			ended;
	char			*buf;
	int				rows;
	int				maxw;
	int				alloc_failed;
	int				player_count;
}	t_mapbuild;

typedef struct s_parsed
{
	t_config	config;
	t_map		map;
}	t_parsed;

/* ================== Parsing ================== */

const char	*perr_str(t_perr e);
const char	*skip_ws(const char *str);
t_bool		parse_file_fd(
				int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err);
t_bool		mb_push_line(t_mapbuild *map, char *line);
t_bool		map_quick_border_check(const t_map *m);
t_bool		map_neighbors_ok(const t_map *m);
t_bool		map_build_split(const t_mapbuild *mb, t_map *out);
t_bool		is_map_line(const char *str);
t_bool		is_blank_or_ws_only(const char *s);
t_bool		is_map_char(int car);
t_bool		parse_triplet(const char *str, t_rgb *dst, const char **end_after);
t_bool		handle_rgb(t_id id, const char *rest, t_config *cfg, t_perr *perr);
t_bool		rgb_is_set(const t_rgb *c);
t_bool		parse_header_line(const char *line, t_config *cfg, t_perr *perr);
t_bool		handle_texture(
				t_id id, const char *rest, t_config *cfg, t_perr *perr);
t_perr		header_complete(const t_config *cfg);
t_perr		parsing_load(const char *path, t_parsed *out);
t_perr		validate_params(int argc, char **argv, const char **out_path);
char		*get_next_line(int fd);
char		*dup_range(const char *start, const char *end);
char		**cfg_slot_for_id(t_config *cfg, t_id id);
void		print_perr(t_perr err, const char *str);
void		mb_free(t_mapbuild *map);
void		map_free_split(t_map *m);
void		cfg_free(t_config *c);
void		parsing_free(t_parsed *p);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void		cfg_init(t_config *c);
void		mb_init(t_mapbuild *map);
t_id		parse_id_at_start(const char **pline);
int			line_len_no_nl(const char *str);

#endif
