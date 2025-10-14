
#include "parsing.h"

bool is_map_char(int car)
{
	unsigned char c = (unsigned char)car;
	if (c=='0'||c=='1'||c==' '||c=='N'||c=='S'||c=='E'||c=='W')
		return (true);
	else
		return (false);
}

int line_len_no_nl(const char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	while (i > 0 && (str[i - 1] == '\n' || str[i - 1] == '\r'))
		i --;
	return (i);
}

bool is_map_line(const char *str)
{
	int len;
	int i;

	i = 0;
	len = line_len_no_nl(str);
	if (len == 0)
		return (false);
	while (i < len)
	{
		if (!is_map_char((unsigned char)str[i]))
			return (false);
		i ++;
	}
	return (true);
}

void mb_init(t_mapbuild *map)
{
	ft_memset(map, 0, sizeof(*map));
	map->player.row = -1;
	map->player.column = -1;
}

bool mb_grow_buf(t_mapbuild *map, size_t need_more)
{
	size_t want;
	size_t new_cap;
	char *new_buf;
	size_t limit;
	size_t half;

	limit = SIZE_MAX - map->len;
	if (need_more > limit)
		return (false);
	want = map->len + need_more;
	if (want <= map->capacity)
		return (true);
	if (map->capacity == 0)
		new_cap = 256;
	else
		new_cap = map->capacity;
	half = SIZE_MAX / 2;
	while (want > new_cap)
	{
		if (new_cap > half)
			new_cap = want;
		else
			new_cap *= 2;
	}
	new_buf = (char *)realloc(map->buf, new_cap); // NOTE: si besoin, remplace par ta realloc "maison"
	if (!new_buf)
		return (false);
	map->buf = new_buf;
	map->capacity = new_cap;
	return (true);
}

bool mb_push_line(t_mapbuild *map, char *line)
{
	int line_len;
	int i;
	int players_nb;
	int player_column;
	size_t need_more;

	line_len = line_len_no_nl(line);
	i = 0;
	players_nb = 0;
	player_column = -1;
	if (line_len == 0)
		return (false);
	while (i < line_len)
	{
		unsigned char c;
		c = (unsigned char)line[i];
		if (!is_map_char(c))
			return (false);
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			if (map->player_count != 0) // un joueur déjà vu sur une ligne précédente
				return (false);
			players_nb ++;
			if (players_nb > 1) // 2-ieme joueur sur la même ligne
				return (false);
			player_column = i;
		}
		i++;
	}
	need_more = (size_t)line_len + 1;
	if (!mb_grow_buf(map, need_more))
		return (false);
	ft_memcpy(map->buf + map->len, line, (size_t)line_len);
	map->len += (size_t)line_len;
	map->buf[map->len] = '\n';
	map->len += 1;
	if (line_len > map->maxw)
		map->maxw = line_len;
	if (players_nb == 1)
	{
		map->player.row = map->rows;
		map->player.column = player_column;
		map->player.dir = line[player_column];
		map->player_count += 1;
	}
	map->rows++; // push réussi
	return (true);
}

bool map_build_split(const t_mapbuild *mb, t_map *out)
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

bool map_quick_border_check(const t_map *m)
{
	int row_index;
	int column_index;
	char c;

	row_index = 0;
	column_index = 0;
	if (!m || !m->grid)
		return (false);
	if (m->rows <= 0 || m->columns <= 0)
		return (false);
	// haut et bas
	while (column_index < m->columns)
	{
		c = m->grid[0][column_index]; /* haut */
		if (c == '0')
			return (false);
		c = m->grid[m->rows - 1][column_index]; /* bas */
		if (c == '0')
			return (false);
		column_index++;
	}
	// gauche et droite
	row_index = 0;
	while (row_index < m->rows)
	{
		c = m->grid[row_index][0]; /* gauche */
		if (c == '0')
			return (false);
		c = m->grid[row_index][m->columns - 1]; /* droite */
		if (c == '0')
			return (false);
		row_index++;
	}
	return (true);
}

bool map_neighbors_ok(const t_map *m)
{
	int row_index;
	int column_index;
	char c;

	if (!m || !m->grid)
		return (false);
	row_index = 0;
	while (row_index < m->rows)
	{
		column_index = 0;
		while (column_index < m->columns)
		{
			c = m->grid[row_index][column_index];
			if (c == '0')
			{
				if (row_index > 0 && m->grid[row_index - 1][column_index] == ' ')
					return (false);
				if (row_index + 1 < m->rows && m->grid[row_index + 1][column_index] == ' ')
					return (false);
				if (column_index > 0 && m->grid[row_index][column_index - 1] == ' ')
					return (false);
				if (column_index + 1 < m->columns && m->grid[row_index][column_index + 1] == ' ')
					return (false);
			}
			column_index++;
		}
		row_index++;
	}
	return (true);
}
