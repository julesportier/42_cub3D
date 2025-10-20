/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_id.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 16:10:18 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/19 16:10:26 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool	is_id_sep(char c)
{
	if (c == '\0')
		return (true);
	if (c == ' ')
		return (true);
	if (c == '\t')
		return (true);
	if (c == '\r')
		return (true);
	return (false);
}

static t_id	try_take_id(const char **pp)
{
	const char	*p;
	
	if (!pp || !*pp)
		return (ID_UNKNOWN);
	p = *pp;
	if (ft_strncmp(p, "NO", 2) == 0 && is_id_sep(p[2]))
	{
		*pp = p + 2;
		return (ID_NO);
	}
	if (ft_strncmp(p, "SO", 2) == 0 && is_id_sep(p[2]))
	{
		*pp = p + 2;
		return (ID_SO);
	}
	if (ft_strncmp(p, "WE", 2) == 0 && is_id_sep(p[2]))
	{
		*pp = p + 2;
		return (ID_WE);
	}
	if (ft_strncmp(p, "EA", 2) == 0 && is_id_sep(p[2]))
	{
		*pp = p + 2;
		return (ID_EA);
	}
	if (ft_strncmp(p, "F",  1) == 0 && is_id_sep(p[1]))
	{
		*pp = p + 1;
		return (ID_F); 
	}
	if (ft_strncmp(p, "C",  1) == 0 && is_id_sep(p[1]))
	{
		*pp = p + 1;
		return (ID_C); 
	}
	return (ID_UNKNOWN);
}

t_id	parse_id_at_start(const char **pline)
{
	const char	*p; 
	t_id	id;

	p = skip_ws(*pline);
	if (*p == '\0')
	{
		*pline = p;
		return (ID_NONE);
	}
    id = try_take_id(&p);
	*pline = p;
	return (id);
}
