/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:18:06 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/19 14:15:26 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdbool.h> // revoir si toutes ces includes sont necessaires ?
# include <stdlib.h>
# include "libft/src/libft.h"
# include <stdint.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>

/* ================== Data ================== */

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
	bool	is_set;
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

typedef struct s_player
{
    int  row;
    int  column;
    char dir;
}   t_player;

typedef struct s_map
{
    int       rows;
    int       columns;
    char    **grid;
    t_player  player;
}   t_map;

typedef struct s_mapbuild
{
    char     *buf;        // gros buffer texte concaténé avec '\n'
    size_t    len;        // octets utilisés dans buf
    size_t    capacity;   // capacité allouée de buf
    int       rows;       // nb de lignes carte rencontrées
    int       maxw;       // largeur max rencontrée
    t_player  player;     // position/dir trouvées pendant le build
    int       player_count;// combien de joueurs
    bool      started;    // on est entré dans la section map
    bool      ended;      // on a quitté la section map
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
//static bool parse_colors(const char *str, int *result);
//static bool read_component(const char **pp, int *out);
//static bool consume_comma(const char **pp);
bool parse_triplet(const char *str, t_rgb *dst, const char **end_after);
//bool trim_range(const char *rest, const char **start, const char **end);
//bool path_has_xpm_suffix(const char *start, const char *end);
char **cfg_slot_for_id(t_config *cfg, t_id id);
t_id parse_id_at_start(const char **pline);
bool handle_texture(t_id id, const char *rest, t_config *cfg, t_perr *perr);
bool handle_rgb(t_id id, const char *rest, t_config *cfg, t_perr *perr);
bool parse_header_line(const char *line, t_config *cfg, t_perr *perr);
const char *perr_str(t_perr e);
void cfg_init(t_config *c);
bool rgb_is_set(const t_rgb *c);
t_perr header_complete(const t_config *cfg);

/* helpers */
bool is_map_char(int car);
int     line_len_no_nl(const char *s);
bool    is_map_line(const char *s);

/* builder “mb_*” */
void    mb_init(t_mapbuild *mb);
void    mb_free(t_mapbuild *mb);
bool    mb_push_line(t_mapbuild *mb, char *line);
bool mb_grow_buf(t_mapbuild *map, size_t need_more);
bool is_blank_or_ws_only(const char *s);
bool parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err);

/* ===== SPLIT: tableau de lignes ===== */
bool    map_build_split(const t_mapbuild *mb, t_map *out);
void    map_free_split(t_map *m);
void	map_free_split(t_map *m);
void cfg_free(t_config *c);

/* checks fermeture */
bool    map_quick_border_check(const t_map *m);
bool    map_neighbors_ok(const t_map *m);

/* debug */
void    dump_map(const t_map *m);
t_perr validate_params(int argc, char **argv, const char **out_path);
void print_perr(t_perr err, const char *str);
bool check_extension(const char *path);
int main(int ac, char **av);
void    parsing_free(t_parsed *p);
t_perr  parsing_load(const char *path, t_parsed *out);

#endif
