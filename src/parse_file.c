/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:44:34 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/18 18:27:57 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool	handle_map_start(char *line, t_mapbuild *mb, t_perr *err)
{
	if (!mb_push_line(mb, line))
	{
		if (mb->alloc_failed)
			*err = perr_alloc;
		else
			*err = perr_read;
		mb->alloc_failed = 0;
		return (false);
	}
	return (true);
}

static t_bool	handle_before_map(
	char *line, t_config *cfg, t_mapbuild *mb, t_perr *err)
{
	if (is_map_line(line))
	{
		mb->started = true;
		return (handle_map_start(line, mb, err));
	}
	if (!is_blank_or_ws_only(line))
	{
		*err = perr_ok;
		if (!parse_header_line(line, cfg, err))
			return (false);
	}
	return (true);
}

static t_bool	handle_in_or_after_map(char *line, t_mapbuild *mb, t_perr *err)
{
	if (mb->ended)
	{
		if (line_len_no_nl(line) > 0 && !is_blank_or_ws_only(line))
			return (*err = perr_trailing, false);
		return (true);
	}
	if (is_map_line(line))
	{
		if (!mb_push_line(mb, line))
			return (false);
	}
	else
	{
		if (line_len_no_nl(line) > 0 && !is_blank_or_ws_only(line))
			return (*err = perr_trailing, false);
		mb->ended = true;
	}
	return (true);
}

static t_bool	handle_line(
	char *line, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
	t_bool	ok;
	t_perr local_err = perr_ok;

	*out_err = perr_ok;
	if (!mb->started)
		ok = handle_before_map(line, cfg, mb, out_err);
	else
	{
		ok = handle_in_or_after_map(line, mb, &local_err);
		if (!ok)
		{
			if (mb->alloc_failed)
				*out_err = perr_alloc;
			else
			{
				if (local_err != perr_ok)
					*out_err = local_err;
				else
					*out_err = perr_player_dup;
			}
			mb->alloc_failed = 0;
		}
	}
	free(line);
	return (ok);
}

t_bool	parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
	char	*line;

	*out_err = perr_ok;
	mb_init(mb);
	line = get_next_line(fd);
	if (!line)
	{
		*out_err = perr_file_empty;
		return (false);
	}
	while (line)
	{
		if (!handle_line(line, cfg, mb, out_err))
			return (false);
		line = get_next_line(fd);
	}
	if (!mb->started || mb->rows <= 0)
	{
        *out_err = perr_map_empty;
        return (false);
    }
    if (mb->player_count == 0)
	{
        *out_err = perr_player_none;
        return (false);
    }
    if (mb->player_count > 1)
	{
        *out_err = perr_player_dup;
        return (false);
    }
	return (true);
}
