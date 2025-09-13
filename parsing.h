#ifndef PARSING_H
# define PARSING_H

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h> //attention libft !
#include <stdbool.h>

typedef enum e_perr
{
  PERR_OK = 0,
  PERR_ARGC,   // mauvais nombre d'arguments
  PERR_EMPTY,  // chemin vide
  PERR_EXT,    // mauvaise extension (.cub exigé, sensible à la casse)
  PERR_DIR,    // chemin pointe vers un dossier
  PERR_OPEN    // introuvable / illisible
} t_perr;

t_perr validate_params(int argc, char **argv, const char **out_path);
void   print_perr(t_perr err, const char *str);

#endif
