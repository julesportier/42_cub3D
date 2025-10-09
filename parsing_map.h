/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:18:06 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/09 19:56:51 by vakozhev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_MAP_H
# define PARSING_MAP_H

# include <stddef.h>
# include <stdbool.h>
# include <limits.h>
# include "libft/src/libft.h"
# include <stdint.h>

/* ================== Data ================== */

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
    int       player_count;
    bool      started;    // on est entré dans la section map
    bool      ended;      // on a quitté la section map
}   t_mapbuild;

/* ================== Parsing ================== */

char    *get_next_line(int fd);

/* helpers lignes */
//int     line_len_no_nl(const char *s);
//bool    is_map_line(const char *s);

/* builder “mb_*” */
//void    mb_init(t_mapbuild *mb);
void    mb_free(t_mapbuild *mb);
//bool    mb_push_line(t_mapbuild *mb, char *line);
//bool    parse_map_fd(int fd, t_mapbuild *mb);

/* ===== SPLIT: tableau de lignes ===== */
//bool    map_build_split(const t_mapbuild *mb, t_map *out);
//void    map_free_split(t_map *m);

/* checks fermeture */
bool    map_quick_border_check(const t_map *m);
bool    map_neighbors_ok(const t_map *m);

/* debug */
//void    dump_map(const t_map *m);

#endif
