/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_id.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 16:10:18 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/26 17:10:21 by vakozhev         ###   ########lyon.fr   */
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

static t_id	try_match_keyword(
	const char **pp, const char *kw, size_t n, t_id id)
{
	const char	*p;

	if (!pp || !*pp)
		return (ID_UNKNOWN);
	p = *pp;
	if (ft_strncmp(p, kw, n) != 0 || !is_id_sep(p[n]))
		return (ID_UNKNOWN);
	*pp = p + n;
	return (id);
}

static t_id	try_take_id(const char **pp)
{
	t_id	id;

	if (!pp || !*pp)
		return (ID_UNKNOWN);
	id = try_match_keyword(pp, "NO", 2, ID_NO);
	if (id != ID_UNKNOWN)
		return (id);
	id = try_match_keyword(pp, "SO", 2, ID_SO);
	if (id != ID_UNKNOWN)
		return (id);
	id = try_match_keyword(pp, "WE", 2, ID_WE);
	if (id != ID_UNKNOWN)
		return (id);
	id = try_match_keyword(pp, "EA", 2, ID_EA);
	if (id != ID_UNKNOWN)
		return (id);
	id = try_match_keyword(pp, "F", 1, ID_F);
	if (id != ID_UNKNOWN)
		return (id);
	id = try_match_keyword(pp, "C", 1, ID_C);
	if (id != ID_UNKNOWN)
		return (id);
	return (ID_UNKNOWN);
}

t_id	parse_id_at_start(const char **pline)
{
	const char	*p;
	t_id		id;

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
