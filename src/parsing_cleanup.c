
#include "parsing.h"

void mb_free(t_mapbuild *map)
{
	if (!map)
		return;
	free(map->buf);
	mb_init(map);
}

void map_free_split(t_map *m)
{
	int r;

	if (!m)
		return;
	if (m->grid)
	{
		r = 0;
		while (r < m->rows)
		{
			free(m->grid[r]);
			r++;
		}
		free(m->grid);
	}
	ft_memset(m, 0, sizeof(*m));
}

void cfg_free(t_config *c)
{
	free(c->no);
	free(c->so);
	free(c->we);
	free(c->ea);
	ft_memset(c, 0, sizeof(*c));
}
