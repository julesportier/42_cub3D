#ifndef PARSING_H
# define PARSING_H

# include "libft/src/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>
# include <ctype.h>

typedef enum e_perr
{
  PERR_OK = 0,
  PERR_ARGC,   // mauvais nombre d'arguments
  PERR_EMPTY,  // chemin vide
  PERR_EXT,    // mauvaise extension
  PERR_DIR,    // chemin pointe vers un dossier
  PERR_OPEN,    // introuvable
  PERR_READ, //get next line
  PERR_ALLOC, //malloc 
  PERR_EL_DUP, //id en double
  PERR_EL_MISS, //id manquant
  PERR_ID_BAD, // id incomplet
  PERR_RGB_BAD, //val inv
  PERR_PATH_MISS //texture manquant...
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
    int r;
    int g;
    int b;
    bool is_set;
}   t_rgb;

typedef struct s_config
{
    char *no;
	char *so; 
	char *we; 
	char *ea;
    t_rgb  floor_rgb;
    t_rgb  ceil_rgb;
    char **map_lines; 
    size_t map_h; 
    size_t map_cap;
} t_config;


//t_perr validate_params(int argc, char **argv, const char **out_path);
//void   print_perr(t_perr err, const char *str);
char      *get_next_line(int fd);
bool       parse_header_line(const char *line, t_config *cfg, t_perr *perr);
t_perr     header_complete(const t_config *cfg);
void       cfg_init(t_config *cfg);
void       cfg_free(t_config *cfg);
void dump_header(const t_config *c, const char *where);
char	**free_splits(char **arr, size_t pos);
void	free_tokens(char **tok);
char	**ft_split_multi(const char *str, const char *delims);

#endif
