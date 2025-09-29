#ifndef PARSING_H
# define PARSING_H

# include "libft/src/libft.h"
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>


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
    int  *offs;
    int *lens;
    int   rows;
    int rows_capacity;
    int   maxw;
    t_player player;
    int     player_count;
    bool    started;
    bool    ended;
} t_mapbuild;


//t_perr validate_params(int argc, char **argv, const char **out_path);
//void   print_perr(t_perr err, const char *str);
char      *get_next_line(int fd);


#endif
