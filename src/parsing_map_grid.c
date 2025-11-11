/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_grid.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:51:40 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/10 18:27:03 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t	n;
	void	*newp;

	if (new_size == 0)
		return (NULL);
	newp = malloc(new_size);
	if (!newp)
		return (NULL);
	if (ptr && old_size)
	{
		if (old_size < new_size)
			n = old_size;
		else
			n = new_size;
		ft_memcpy(newp, ptr, n);
		free(ptr);
	}
	return (newp);
}

static t_bool	alloc_padded_grid(t_map *out)
{
	int	r;

	r = 0;
	out->grid = (char **)malloc((size_t)out->rows * sizeof(char *));
	if (!out->grid)
		return (false);
	ft_memset(out->grid, 0, (size_t)out->rows * sizeof(char *));
	while (r < out->rows)
	{
		out->grid[r] = (char *)malloc((size_t)out->columns);
		if (!out->grid[r])
		{
			out->rows = r;
			map_free_split(out);
			return (false);
		}
		ft_memset(out->grid[r], ' ', (size_t)out->columns);
		r ++;
	}
	return (true);
}

static void	copy_row_from_buf(
	const t_mapbuild *mb, t_map *out, int r, size_t *pos)
{
	char	ch;
	int		cidx;

	cidx = 0;
	while (*pos < mb->len && mb->buf[*pos] != '\n' && cidx < out->columns)
	{
		ch = mb->buf[*pos];
		if (ch != '\r')
		{
			out->grid[r][cidx] = ch;
			cidx++;
		}
		(*pos)++;
	}
	if (*pos < mb->len && mb->buf[*pos] == '\n')
		(*pos)++;
	if (r == out->player.row && out->player.column >= 0
		&& out->player.column < out->columns)
		out->grid[r][out->player.column] = '0';
}

t_bool	map_build_split(const t_mapbuild *mb, t_map *out)
{
	size_t	pos;
	int		r;

	if (!mb || !out)
		return (false);
	if (mb->rows <= 0 || mb->maxw <= 0 || mb->player_count != 1)
		return (false);
	ft_memset(out, 0, sizeof(*out));
	out->rows = mb->rows;
	out->columns = mb->maxw;
	out->player = mb->player;
	if (!alloc_padded_grid(out))
		return (false);
	pos = 0;
	r = 0;
	while (r < out->rows && pos < mb->len)
	{
		copy_row_from_buf(mb, out, r, &pos);
		r ++;
	}
	return (true);
}
