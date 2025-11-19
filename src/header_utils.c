/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:55:14 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/19 13:05:51 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_bool	cfg_any_field_set(const t_config *c)
{
	return (c->no || c->so || c->we || c->ea
		|| rgb_is_set(&c->floor_rgb) || rgb_is_set(&c->ceil_rgb));
}

char	*dup_range(const char *start, const char *end)
{
	size_t	len;
	char	*str;

	if (!start || !end)
		return (NULL);
	if (end < start)
		return (NULL);
	len = (size_t)(end - start);
	if (len == SIZE_MAX)
		return (NULL);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	if (len)
		ft_memcpy(str, start, len);
	str[len] = '\0';
	return (str);
}
