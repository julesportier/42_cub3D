/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:57:47 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/12 14:06:27 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool	parse_colors(const char *str, int *result)
{
	const char	*cursor;
	long		value;

	if (!str || !result)
		return (false);
	cursor = skip_ws(str);
	if (*cursor == '+')
		cursor ++;
	if (*cursor < '0' || *cursor > '9')
		return (false);
	value = 0;
	while (*cursor >= '0' && *cursor <= '9')
	{
		value = value * 10 + (*cursor - '0');
		if (value > 255)
			return (false);
		cursor ++;
	}
	cursor = skip_ws(cursor);
	if (*cursor != '\0')
		return (false);
	*result = (int)value;
	return (true);
}

static t_bool	read_component(const char **cursor, int *out)
{
	const char	*p;
	char		digits[4];
	int			digit_count;

	p = *cursor;
	digit_count = 0;
	p = skip_ws(p);
	if (*p < '0' || *p > '9')
		return (false);
	while (digit_count < 3 && p[digit_count] >= '0' && p[digit_count] <= '9')
	{
		digits[digit_count] = p[digit_count];
		digit_count ++;
	}
	digits[digit_count] = '\0';
	if (digit_count == 0)
		return (false);
	if (!parse_colors(digits, out))
		return (false);
	p += digit_count;
	p = skip_ws(p);
	*cursor = p;
	return (true);
}

static t_bool	consume_comma(const char **cursor)
{
	const char	*p;

	p = *cursor;
	p = skip_ws(p);
	if (*p != ',')
		return (false);
	p ++;
	p = skip_ws(p);
	*cursor = p;
	return (true);
}

static void	assign_rgb_component(t_rgb *dst, int index, int val)
{
	if (index == 0)
		dst->r = val;
	else if (index == 1)
		dst->g = val;
	else
		dst->b = val;
}

t_bool	parse_triplet(const char *str, t_rgb *dst, const char **end_after)
{
	int			component_index;
	int			component_value;

	if (!str || !dst)
		return (false);
	component_index = 0;
	while (component_index < 3)
	{
		if (!read_component(&str, &component_value))
			return (false);
		assign_rgb_component(dst, component_index, component_value);
		if (component_index < 2 && !consume_comma(&str))
			return (false);
		component_index ++;
	}
	while (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n')
		str ++;
	if (end_after)
		*end_after = str;
	if (*str != '\0')
		return (false);
	dst->is_set = true;
	return (true);
}
