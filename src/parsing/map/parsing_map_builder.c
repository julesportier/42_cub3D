
#include "../parsing.h"

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
