/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rgb.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 17:00:55 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/19 17:55:13 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool	rgb_take_triplet(
	const char	*rest,
	t_rgb *out,
	const char **after,
	t_perr *perr)
{
	const char	*p;
	t_rgb		tmp;

	tmp.r = 0;
	tmp.g = 0;
	tmp.b = 0;
	tmp.is_set = false;
	p = NULL;
	if (!parse_triplet(rest, &tmp, &p))
	{
		if (perr)
			*perr = PERR_RGB_BAD;
		return (false);
	}
	if (*skip_ws(p) != '\0')
	{
		if (perr)
			*perr = PERR_RGB_BAD;
		return (false);
	}
	*out = tmp;
	*after = p;
	return (true);
}

static t_bool	rgb_assign(t_id	id, const t_rgb *val, t_config *cfg, t_perr *perr)
{
	if (id == ID_F)
	{
		if (cfg->floor_rgb.is_set)
		{
			if (perr)
				*perr = PERR_EL_DUP;
			return (false);
		}
		cfg->floor_rgb = *val;
		return (true);
	}
	if (cfg->ceil_rgb.is_set)
	{
		if (perr)
			*perr = PERR_EL_DUP;
		return (false);
	}
	cfg->ceil_rgb = *val;
	return (true);
}

t_bool	handle_rgb(t_id	id, const char	*rest, t_config	*cfg, t_perr	*perr)
{
	const char	*after;
	t_rgb		tmp;

	if (!rgb_take_triplet(rest, &tmp, &after, perr))
		return (false);
	if (!rgb_assign(id, &tmp, cfg, perr))
		return (false);
	if (perr)
		*perr = PERR_OK;
	return (true);
}

t_bool rgb_is_set(const t_rgb *c)
{
	return (c->is_set);
}

t_perr header_complete(const t_config *cfg)
{
	if (!cfg->no || !cfg->so || !cfg->we || !cfg->ea)
		return (PERR_EL_MISS);
	if (!rgb_is_set(&cfg->floor_rgb) || !rgb_is_set(&cfg->ceil_rgb))
		return (PERR_EL_MISS);
	return (PERR_OK);
}
