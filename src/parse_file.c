/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:44:34 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/10 19:06:13 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool	handle_map_start(char *line, t_mapbuild *mb, t_perr *err)
{
	if (!mb_push_line(mb, line))
	{
		if (err)
		{
			if (mb->alloc_failed)
				*err = PERR_ALLOC;
			else
				*err = PERR_READ;
		}
		mb->alloc_failed = 0;
		return (false);
	}
	return (true);
}

static t_bool	handle_before_map(
	char *line, t_config *cfg, t_mapbuild *mb, t_perr *err)
{
	t_perr	perr;

	if (is_map_line(line))
	{
		mb->started = true;
		return (handle_map_start(line, mb, err));
	}
	if (!is_blank_or_ws_only(line))
	{
		perr = PERR_OK;
		if (!parse_header_line(line, cfg, &perr))
		{
			if (err)
			{
				if (perr != PERR_OK)
					*err = perr;
				else
					*err = PERR_ID_BAD;
			}
			return (false);
		}
	}
	return (true);
}

static t_bool	handle_in_or_after_map(char *line, t_mapbuild *mb)
{
	if (mb->ended)
	{
		if (line_len_no_nl(line) > 0 && !is_blank_or_ws_only(line))
			return (false);
		return (true);
	}
	if (is_map_line(line))
	{
		if (!mb_push_line(mb, line))
			return (false);
	}
	else
		mb->ended = true;
	return (true);
}

static t_bool	handle_line(
	char *line, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
	t_perr	err;
	t_bool	ok;

	err = PERR_OK;
	if (!mb->started)
		ok = handle_before_map(line, cfg, mb, &err);
	else
	{
		ok = handle_in_or_after_map(line, mb);
		if (!ok)
		{
			if (mb->alloc_failed)
				err = PERR_ALLOC;
			else
				err = PERR_READ;
			mb->alloc_failed = 0;
		}
	}
	if (!ok && out_err)
		*out_err = err;
	free(line);
	return (ok);
}

t_bool	parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
	char	*line;

	if (out_err)
		*out_err = PERR_OK;
	mb_init(mb);
	line = get_next_line(fd);
	while (line)
	{
		if (!handle_line(line, cfg, mb, out_err))
			return (false);
		line = get_next_line(fd);
	}
	return (true);
}
