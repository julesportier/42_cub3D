/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:18:06 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/25 17:14:02 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/src/libft.h"
// # include <stdbool.h> // revoir si toutes ces includes sont necessaires ?
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
	PERR_DIR,
	PERR_OPEN,
	PERR_READ,
	PERR_ALLOC,
	PERR_EL_DUP,
	PERR_EL_MISS,
	PERR_ID_BAD,
	PERR_RGB_BAD,
	PERR_PATH_MISS
} t_perr;

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
} t_id;

typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
	t_bool	is_set;
} t_rgb;

typedef struct s_config
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	t_rgb	floor_rgb;
	t_rgb	ceil_rgb;
} t_config;

typedef struct s_player_init
{
    int  row;
    int  column;
    char dir;
}   t_player_init;

typedef struct s_map
{
    int       rows;
    int       columns;
    char    **grid;
    t_player_init  player;
}   t_map;

typedef struct s_mapbuild
{
    char     *buf;        // gros buffer texte concaténé avec '\n'
    size_t    len;        // octets utilisés dans buf
    size_t    capacity;   // capacité allouée de buf
    int       rows;       // nb de lignes carte rencontrées
    int       maxw;       // largeur max rencontrée
    t_player_init  player;     // position/dir trouvées pendant le build
    int       player_count;// combien de joueurs
    t_bool      started;    // on est entré dans la section map
    t_bool      ended;      // on a quitté la section map
}   t_mapbuild;

typedef struct s_parsed
{
    t_config config;  // textures + couleurs
    t_map    map;     // grille + joueur
} t_parsed;

/* ================== Parsing ================== */

char    *get_next_line(int fd);

/* header */
const char *skip_ws(const char *str);
char *dup_range(const char *start, const char *end);
//static t_bool parse_colors(const char *str, int *result);
//static t_bool read_component(const char **pp, int *out);
//static t_bool consume_comma(const char **pp);
t_bool parse_triplet(const char *str, t_rgb *dst, const char **end_after);
//t_bool trim_range(const char *rest, const char **start, const char **end);
//t_bool path_has_xpm_suffix(const char *start, const char *end);
char **cfg_slot_for_id(t_config *cfg, t_id id);
t_id parse_id_at_start(const char **pline);
t_bool handle_texture(t_id id, const char *rest, t_config *cfg, t_perr *perr);
t_bool handle_rgb(t_id id, const char *rest, t_config *cfg, t_perr *perr);
t_bool parse_header_line(const char *line, t_config *cfg, t_perr *perr);
const char *perr_str(t_perr e);
void cfg_init(t_config *c);
t_bool rgb_is_set(const t_rgb *c);
t_perr header_complete(const t_config *cfg);

/* helpers */
t_bool is_map_char(int car);
int     line_len_no_nl(const char *s);
t_bool    is_map_line(const char *s);

/* builder “mb_*” */
void    mb_init(t_mapbuild *mb);
void    mb_free(t_mapbuild *mb);
t_bool    mb_push_line(t_mapbuild *mb, char *line);
t_bool mb_grow_buf(t_mapbuild *map, size_t need_more);
t_bool is_blank_or_ws_only(const char *s);
t_bool parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err);

/* ===== SPLIT: tableau de lignes ===== */
t_bool    map_build_split(const t_mapbuild *mb, t_map *out);
void    map_free_split(t_map *m);
void	map_free_split(t_map *m);
void cfg_free(t_config *c);

/* checks fermeture */
t_bool    map_quick_border_check(const t_map *m);
t_bool    map_neighbors_ok(const t_map *m);

/* debug */
void    dump_map(const t_map *m);
t_perr validate_params(int argc, char **argv, const char **out_path);
void print_perr(t_perr err, const char *str);
t_bool check_extension(const char *path);
void    parsing_free(t_parsed *p);
t_perr  parsing_load(const char *path, t_parsed *out);

#endif
