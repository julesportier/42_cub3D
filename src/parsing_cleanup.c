/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:17:06 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/19 16:47:04 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	mb_free(t_mapbuild *map)
{
	if (!map)
		return ;
	free(map->buf);
	mb_init(map);
}

void	map_free_split(t_map *m)
{
	int	r;
	
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

void	cfg_free(t_config *c)
{
	free(c->no);
	free(c->so);
	free(c->we);
	free(c->ea);
	ft_memset(c, 0, sizeof(*c));
}
