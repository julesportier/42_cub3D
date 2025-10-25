/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 16:12:15 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/25 15:43:08 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool trim_range(const char *rest, const char **start, const char **end)
{
	const char *p;
	size_t len;
	char ch;

	p = skip_ws(rest);
	if (*p == '\0')
		return (false);
	len = ft_strlen(p);
	while (len > 0)
	{
		ch = p[len - 1];
		if (!(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'))
			break ;
		len--;
	}
	if (len == 0)
		return (false);
	*start = p;
	*end = p + len;
	return (true);
}
 
char	**cfg_slot_for_id(t_config *cfg, t_id id)
{
	if (id==ID_NO)
		return (&cfg->no);
	if (id==ID_SO)
		return (&cfg->so);
	if (id==ID_WE)
		return (&cfg->we);
	if (id==ID_EA)
		return (&cfg->ea);
	return (NULL);
}

static t_bool tex_take_path(
	const char *rest,
	const char **start,
	const char **end,
	t_perr *perr
)
{
	size_t len;
	const char *suf;

	if (!trim_range(rest, start, end))
	{
		if (perr)
			*perr = PERR_PATH_MISS;
		return (false);
	}
	len = (size_t)(*end - *start);
	if (len < 4)
	{
		if (perr)
			*perr = PERR_PATH_MISS;
		return (false);
	}
	suf = *end - 4;
	if (ft_strncmp(suf, ".xpm", 4) != 0)
	{
		if (perr)
			*perr = PERR_PATH_MISS;
		return (false);
	}
	return (true);
}

static char	**tex_get_slot(t_config *cfg, t_id id, t_perr *perr)
{
	char	**slot;

	slot = cfg_slot_for_id(cfg, id);
	if (!slot)
	{
		if (perr)
			*perr = PERR_ID_BAD;
		return (NULL);
	}
	if (*slot)
	{
		if (perr)
			*perr = PERR_EL_DUP;
		return (NULL);
	}
	return (slot);
}

t_bool	handle_texture(t_id id, const char *rest, t_config *cfg, t_perr *perr)
{
	const char	*start;
	const char	*end;
	char		**slot;
	char		*path;

	if (!tex_take_path(rest, &start, &end, perr))
		return (false);
	slot = tex_get_slot(cfg, id, perr);
	if (!slot)
		return (false);
	path = dup_range(start, end);
	if (!path)
	{
		if (perr)
			*perr = PERR_ALLOC;
		return (false);
	}
	*slot = path;
	if (perr)
		*perr = PERR_OK;
	return (true);
}
