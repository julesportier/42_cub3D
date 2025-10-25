/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 15:43:22 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/25 16:10:51 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

const char	*skip_ws(const char *str)
{
	while (*str == ' ' || *str == '\t' || *str == '\r')
		str ++;
	return (str);
}

/*const char *perr_str(t_perr e)
{
    if (e == PERR_OK)       return "OK";
    else if (e == PERR_ARGC) return "Invalid number of arguments";
    else if (e == PERR_EMPTY)return "EMPTY";
    else if (e == PERR_EXT)  return "EXT";
    else if (e == PERR_DIR)  return "DIR";
    else if (e == PERR_OPEN) return "OPEN";
    else if (e == PERR_READ) return "READ";
    else if (e == PERR_ALLOC)return "ALLOC";
    else if (e == PERR_EL_DUP)  return "EL_DUP";
    else if (e == PERR_EL_MISS) return "EL_MISS";
    else if (e == PERR_ID_BAD)  return "ID_BAD";
    else if (e == PERR_RGB_BAD) return "RGB_BAD";
    else if (e == PERR_PATH_MISS) return "PATH_BAD/MISS";
    return "UNKNOWN";
}*/

const char *perr_str(t_perr e)
{
    if (e == PERR_OK)         return "OK";
    else if (e == PERR_ARGC)  return "Invalid number of arguments";
    else if (e == PERR_EMPTY) return "Empty file path";
    else if (e == PERR_EXT)   return "Invalid file extension (expected .cub)";
    else if (e == PERR_DIR)   return "Path is a directory";
    else if (e == PERR_OPEN)  return "Failed to open file";
    else if (e == PERR_READ)  return "Failed to read file";
    else if (e == PERR_ALLOC) return "Out of memory";
    else if (e == PERR_EL_DUP)   return "Duplicate identifier";
    else if (e == PERR_EL_MISS)  return "Missing identifier";
    else if (e == PERR_ID_BAD)   return "Invalid identifier";
    else if (e == PERR_RGB_BAD)  return "Invalid RGB value";
    else if (e == PERR_PATH_MISS) return "Missing/invalid texture path";
    return "Unknown error";
}


char *dup_range(const char *start, const char *end)
{
	size_t len;
	char *str;

	if (!start || !end)
		return (NULL);
	if (end < start)
		return (NULL);
	len = (size_t)(end - start);
	if (len == SIZE_MAX)
		return (NULL);
	str = (char*)malloc(len + 1);
	if (!str)
		return (NULL);
	if (len)
		ft_memcpy(str, start, len);
	str[len] = '\0';
	return (str);
}

t_bool is_map_char(int car)
{
	unsigned char c = (unsigned char)car;
	if (c=='0'||c=='1'||c==' '||c=='N'||c=='S'||c=='E'||c=='W')
		return (true);
	else
		return (false);
}

int line_len_no_nl(const char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	while (i > 0 && (str[i - 1] == '\n' || str[i - 1] == '\r'))
		i --;
	return (i);
}

