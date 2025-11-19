/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_id.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 16:10:18 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/12 13:38:18 by vakozhev         ###   ########lyon.fr   */
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
	const char **cursor, const char *kw, size_t kw_len, t_id id)
{
	const char	*p_line;

	p_line = *cursor;
	if (ft_strncmp(p_line, kw, kw_len) != 0)
		return (id_unknown);
	else if (*(p_line + kw_len) == '\n')
		return (id_no_field);
	else if (!is_id_sep(p_line[kw_len]))
		return (id_unknown);
	*cursor = p_line + kw_len;
	return (id);
}

static t_id	try_take_id(const char **cursor)
{
	t_id	id;

	id = try_match_keyword(cursor, "NO", 2, id_no);
	if (id != id_unknown)
		return (id);
	id = try_match_keyword(cursor, "SO", 2, id_so);
	if (id != id_unknown)
		return (id);
	id = try_match_keyword(cursor, "WE", 2, id_we);
	if (id != id_unknown)
		return (id);
	id = try_match_keyword(cursor, "EA", 2, id_ea);
	if (id != id_unknown)
		return (id);
	id = try_match_keyword(cursor, "F", 1, id_f);
	if (id != id_unknown)
		return (id);
	id = try_match_keyword(cursor, "C", 1, id_c);
	if (id != id_unknown)
		return (id);
	return (id_unknown);
}

t_id	parse_id_at_start(const char **line_cursor)
{
	const char	*current;
	t_id		id;

	current = skip_ws(*line_cursor);
	if (*current == '\0')
	{
		*line_cursor = current;
		return (id_none);
	}
	id = try_take_id(&current);
	*line_cursor = current;
	return (id);
}
