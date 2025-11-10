/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 15:43:22 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/10 18:29:00 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

const char	*skip_ws(const char *str)
{
	while (*str == ' ' || *str == '\t' || *str == '\r')
		str ++;
	return (str);
}

t_bool	is_map_line(const char *str)
{
	int	len;
	int	i;

	i = 0;
	len = line_len_no_nl(str);
	if (len == 0)
		return (false);
	while (i < len)
	{
		if (!is_map_char((unsigned char)str[i]))
			return (false);
		i ++;
	}
	return (true);
}

t_bool	is_blank_or_ws_only(const char *s)
{
	const unsigned char	*p;

	p = (const unsigned char *)s;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p ++;
	if (*p == '\0')
		return (true);
	return (false);
}

t_bool	is_map_char(int car)
{
	unsigned char	c;

	c = (unsigned char)car;
	if (c == '0' || c == '1' || c == ' '
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (true);
	else
		return (false);
}

int	line_len_no_nl(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	while (i > 0 && (str[i - 1] == '\n' || str[i - 1] == '\r'))
		i --;
	return (i);
}
