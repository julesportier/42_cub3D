/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_load.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:02:54 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/12 13:59:53 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_bool	parse_header_line(const char *line, t_config *cfg, t_perr *perr)
{
	const char	*p_line_copy;
	t_id		id;

	*perr = perr_ok;
	if (!line)
		return (false);
	p_line_copy = line;
	id = parse_id_at_start(&p_line_copy);
	if (id == id_none)
		return (false);
	if (id == id_unknown)
	{
		*perr = perr_id_bad;
		return (false);
	}
	if (id == id_no || id == id_so || id == id_we || id == id_ea)
		return (handle_texture(id, p_line_copy, cfg, perr));
	else
		return (handle_rgb(id, p_line_copy, cfg, perr));
}

static t_perr	load_cfg_build_map(int fd, t_config *cfg, t_map *m)
{
	t_mapbuild	mb;
	t_perr		perr;

	perr = perr_ok;
	cfg_init(cfg);
	if (!parse_file_fd(fd, cfg, &mb, &perr))
	{
		mb_free(&mb);
		if (perr != perr_ok)
			return (perr);
	}
	perr = header_complete(cfg);
	if (perr != perr_ok)
		return (mb_free(&mb), perr);
	if (!map_build_split(&mb, m))
		return (mb_free(&mb), perr_read);
	mb_free(&mb);
	if (!map_quick_border_check(m) || !map_neighbors_ok(m))
		return (map_free_split(m), perr_read);
	return (perr_ok);
}

t_perr	parsing_load(const char *path, t_parsed *out)
{
	t_config	cfg;
	t_perr		perr;
	t_map		m;
	int			fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		print_perr(perr_open, path);
		return (perr_open);
	}
	perr = load_cfg_build_map(fd, &cfg, &m);
	close(fd);
	if (perr != perr_ok)
	{
		cfg_free(&cfg);
		print_perr(perr, path);
		return (perr);
	}
	out->config = cfg;
	out->map = m;
	return (perr_ok);
}
