/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 16:12:15 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/12 13:58:55 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool	trim_range(
	const char *rest, const char **start, const char **end)
{
	const char	*p;
	size_t		len;
	char		ch;

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
	if (id == id_no)
		return (&cfg->no);
	if (id == id_so)
		return (&cfg->so);
	if (id == id_we)
		return (&cfg->we);
	if (id == id_ea)
		return (&cfg->ea);
	return (NULL);
}

static t_bool	tex_take_path(
	const char *rest,
	const char **start,
	const char **end,
	t_perr *perr
)
{
	const char	*suf;
	size_t		len;

	if (!trim_range(rest, start, end))
	{
		*perr = perr_path_miss;
		return (false);
	}
	len = (size_t)(*end - *start);
	if (len < 4)
	{
		*perr = perr_path_miss;
		return (false);
	}
	suf = *end - 4;
	if (ft_strncmp(suf, ".xpm", 4) != 0)
	{
		*perr = perr_path_miss;
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
		*perr = perr_id_bad;
		return (NULL);
	}
	if (*slot)
	{
		*perr = perr_el_dup;
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
		*perr = perr_alloc;
		return (false);
	}
	*slot = path;
	*perr = perr_ok;
	return (true);
}
