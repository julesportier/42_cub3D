#ifndef PARSING_MAP_H
# define PARSING_MAP_H

# include "libft/src/libft.h"
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>


typedef struct s_player
{
    int  row;
    int  column;
    char dir;
} t_player;

typedef struct s_map
{
    int   rows;
    int	  columns;
    char *grid;
    t_player player;
} t_map;

typedef struct s_mapbuild
{
    char *buf;
    size_t len;
    size_t capacity;
    int   rows;
    int   maxw;
    t_player player;
    int     player_count;
    bool    started;
    bool    ended;
} t_mapbuild;

char      *get_next_line(int fd);


#endif
