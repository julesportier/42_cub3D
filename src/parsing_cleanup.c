#include "parsing.h"

char **free_splits(char **arr, size_t pos)
{
    if (!arr) return NULL;
    for (size_t j = 0; j < pos; j++) free(arr[j]);  // pas d’underflow possible
    free(arr);
    return NULL;
}

void	free_tokens(char **tok)
{
	size_t	i;

	i = 0;
	if (!tok)
		return;
	while (tok[i])
	{
		free(tok[i]);
		i ++;
	}
	free(tok);
}

void cfg_free(t_config *c)
{
	size_t	i;
	
	i = 0;
    free(c->no);
    free(c->so);
    free(c->we);
    free(c->ea);
    while (i < c->map_h)
    {
		free(c->map_lines[i]);
		i ++;
	}
    free(c->map_lines);
}
