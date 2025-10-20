
#include "parsing.h"

t_bool map_build_split(const t_mapbuild *mb, t_map *out)
{
	int r;
	size_t pos;
	int cidx;
	char ch;

	if (!mb || !out)
		return (false);
	if (mb->rows <= 0 || mb->maxw <= 0 || mb->player_count != 1)
		return (false);
	ft_memset(out, 0, sizeof(*out));
	out->rows = mb->rows;
	out->columns = mb->maxw;
	out->player = mb->player;
	out->grid = (char **)malloc((size_t)out->rows * sizeof(char *));
	if (!out->grid)
		return (false);
	ft_memset(out->grid, 0, (size_t)out->rows * sizeof(char *));
	r = 0;
	while (r < out->rows)
	{
		out->grid[r] = (char *)malloc((size_t)out->columns);
		if (!out->grid[r])
		{
			out->rows = r;
			map_free_split(out);
			return (false);
		}
		ft_memset(out->grid[r], ' ', (size_t)out->columns); // padding à droite
		r++;
	}
	pos = 0;
	r = 0;
	while (r < out->rows && pos < mb->len)
	{
		cidx = 0;
		while (pos < mb->len && mb->buf[pos] != '\n' && cidx < out->columns)
		{
			ch = mb->buf[pos];
			if (ch != '\r')
			{
				out->grid[r][cidx] = ch; /* recouvre le padding */
				cidx++;
			}
			pos++;
		}
		if (pos < mb->len && mb->buf[pos] == '\n')
			pos++;
		// remplace le joueur par '0'
		if (r == out->player.row && out->player.column >= 0 && out->player.column < out->columns)
			out->grid[r][out->player.column] = '0';
		r++;
	}
	return (true);
}
