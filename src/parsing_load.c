/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_load.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:02:54 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/11 19:41:02 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_bool	parse_header_line(const char *line, t_config *cfg, t_perr *perr)
{
	const char	*p;
	t_id		id;

	if (perr)
		*perr = PERR_OK;
	if (!line)
		return (false);
	p = line;
	id = parse_id_at_start(&p);
	if (id == ID_NONE)
		return (false);
	if (id == ID_UNKNOWN)
	{
		if (perr)
			*perr = PERR_ID_BAD;
		return (false);
	}
	if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA)
		return (handle_texture(id, p, cfg, perr));
	else
		return (handle_rgb(id, p, cfg, perr));
}

static t_perr	load_cfg_build_map(int fd, t_config *cfg, t_map *m)
{
	t_mapbuild	mb;
	t_perr		perr;

	perr = PERR_OK;
	cfg_init(cfg);
	if (!parse_file_fd(fd, cfg, &mb, &perr))
	{
		mb_free(&mb);
		if (perr != PERR_OK)
			return (perr);
	}
	perr = header_complete(cfg);
	if (perr != PERR_OK)
		return (mb_free(&mb), perr);
	if (!map_build_split(&mb, m))
		return (mb_free(&mb), PERR_ALLOC);
	mb_free(&mb);
	if (!map_quick_border_check(m) || !map_neighbors_ok(m))
		return (map_free_split(m), PERR_READ);
	return (PERR_OK);
}

t_perr	parsing_load(const char *path, t_parsed *out)
{
	t_config	cfg;
	t_perr		perr;
	t_map		m;
	int			fd;

	if (!path || !out)
		return (print_perr(PERR_EMPTY, path), PERR_EMPTY);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		print_perr(PERR_OPEN, path);
		return (PERR_OPEN);
	}
	perr = load_cfg_build_map(fd, &cfg, &m);
	close(fd);
	if (perr != PERR_OK)
	{
		cfg_free(&cfg);
		print_perr(perr, path);
		return (perr);
	}
	out->config = cfg;
	out->map = m;
	return (PERR_OK);
}
