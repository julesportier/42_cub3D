/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:22:49 by vakozhev          #+#    #+#             */
/*   Updated: 2025/09/13 17:22:55 by vakozhev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool	is_delim(char c, const char *str)
{
	size_t	i;
	
	i = 0;
	if (str == NULL  || str[0] == '\0')
		return (false);
	if (c == '\0')
		return (false);
	while(str[i] != '\0')
	{
		if (c == str[i])
			return (true);
		i ++;
	}
	return (false);
}

static size_t	count_splits(const char *str, const char *delim)
{
	size_t	i;
	bool	in;
	int	count;

	in = false;
	count = 0;
	if (!str || !*str)
		return (0);
	while (str[i])
	{
		if (!in && !is_delim(str[i], delim))
		{
			in = true;
			count ++;
		}
		else if (in && is_delim(str[i], delim))
			in = false;
		i ++;
	}
	return (count);
}

static size_t	get_split_len(const char *str, const char *delim, size_t start)
{
	size_t	i;

	i = 0;
	while (str[start + i] && !is_delim(str[start + i], delim))
		i ++;
	return (i);
}

static char	*make_split(const char *str, size_t start, size_t len)
{
	char	*out;

	out = malloc(len + 1);
	if (!out)
		return (NULL);
	memcpy(out, (str + start), len);
	out[len] = '\0';
	return (out);
}

static char	**free_splits(char **arr, size_t pos)
{
	while (pos--)
		free(arr[pos]);
	free(arr);
	return (NULL);
}

char	**ft_split_multi(const char *str, const char *delims)
{
	size_t	nb_tokens;
	size_t	i;
	size_t	pos;
	size_t	len;
	char	**arr;

	nb_tokens = 0;
	i = 0;
	pos = 0;
	len = 0;
	arr = NULL;
	if (!str || !delims || !*delims)
		return (NULL);
	nb_tokens = count_splits(str, delims);
	arr = malloc((nb_tokens + 1) * sizeof *arr);
	if (!arr)
		return (NULL);
	while (pos < nb_tokens)
	{
		while (str[i] && is_delim(str[i], delims))
			i ++;
		len = get_split_len(str, delims, i);
		arr[pos] = make_split(str, i, len);
		if (!arr[pos])
			return (free_splits(arr, pos));
		i += len;
		while (str[i] && is_delim(str[i], delims))
			i ++;
		pos ++;
	}
	arr[pos] = NULL;
	return (arr);
}

static void	free_tokens(char **tok)
{
	size_t	i;

	i = 0;
	if (!tok)
		return;
	while (tok[i])
	{
		free(tok[i]);
		i ++;
	}
	free(tok);
}

