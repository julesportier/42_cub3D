/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_builder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 16:51:48 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/10 18:25:34 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static size_t	mb_calc_new_cap(size_t capacity, size_t want)
{
	size_t	new_cap;
	size_t	half;

	if (capacity == 0)
		new_cap = 256;
	else
		new_cap = capacity;
	half = SIZE_MAX / 2;
	while (want > new_cap)
	{
		if (new_cap > half)
			new_cap = want;
		else
			new_cap *= 2;
	}
	return (new_cap);
}

static t_bool	mb_grow_buf(t_mapbuild *map, size_t need_more)
{
	size_t	limit;
	size_t	want;
	size_t	new_cap;
	char	*new_buf;

	limit = SIZE_MAX - map->len;
	if (need_more > limit)
		return (false);
	want = map->len + need_more;
	if (want <= map->capacity)
		return (true);
	new_cap = mb_calc_new_cap(map->capacity, want);
	new_buf = (char *)ft_realloc(map->buf, map->len, new_cap);
	if (!new_buf)
	{
		map->alloc_failed = 1;
		return (false);
	}
	map->buf = new_buf;
	map->capacity = new_cap;
	return (true);
}

static t_bool	mb_scan_line(
	const char *line, int line_len, t_mapbuild *map, int *player_col)
{
	unsigned char	c;
	t_bool			found_player;
	int				i;

	i = 0;
	found_player = false;
	*player_col = -1;
	while (i < line_len)
	{
		c = (unsigned char)line[i];
		if (!is_map_char(c))
			return (false);
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			if (map->player_count != 0 || found_player)
				return (false);
			found_player = true;
			*player_col = i;
		}
		i++;
	}
	return (true);
}

static void	mb_apply_player(
	t_mapbuild *map, const char *line, int player_column)
{
	map->player.row = map->rows;
	map->player.column = player_column;
	map->player.dir = line[player_column];
	map->player_count += 1;
}

t_bool	mb_push_line(t_mapbuild *map, char *line)
{
	size_t	need_more;
	char	*dst;
	int		line_len;
	int		player_column;

	line_len = line_len_no_nl(line);
	if (line_len == 0)
		return (false);
	if (!mb_scan_line(line, line_len, map, &player_column))
		return (false);
	need_more = (size_t)line_len + 1;
	if (!mb_grow_buf(map, need_more))
		return (false);
	dst = map->buf + map->len;
	ft_memcpy(dst, line, (size_t)line_len);
	map->len += (size_t)line_len;
	map->buf[map->len] = '\n';
	map->len += 1;
	if (line_len > map->maxw)
		map->maxw = line_len;
	if (player_column >= 0)
		mb_apply_player(map, line, player_column);
	map->rows++;
	return (true);
}
