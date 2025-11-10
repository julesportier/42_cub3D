/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_struct_init.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 12:44:20 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/10 18:27:36 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	cfg_init(t_config *c)
{
	ft_memset(c, 0, sizeof(*c));
}

void	mb_init(t_mapbuild *map)
{
	ft_memset(map, 0, sizeof(*map));
	map->player.row = -1;
	map->player.column = -1;
	map->alloc_failed = 0;
}
