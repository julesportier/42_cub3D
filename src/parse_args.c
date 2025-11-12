/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:51:40 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/12 12:40:24 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

static t_bool	check_extension(const char *path)
{
	size_t	n;

	if (path == NULL)
		return (false);
	n = ft_strlen(path);
	if (n <= 4)
		return (false);
	if (path[n - 5] == '/')
		return (false);
	if (ft_strncmp(path + (n - 4), ".cub", 4) == 0)
		return (true);
	return (false);
}

t_perr	validate_params(int argc, char **argv, const char **out_path)
{
	const char	*path;

	if (argc != 2)
		return (perr_argc);
	path = argv[1];
	if (path == NULL || *path == '\0')
		return (perr_empty);
	if (!check_extension(path))
		return (perr_ext);
	if (out_path)
		*out_path = path;
	return (perr_ok);
}

const char	*perr_str(t_perr e)
{
	if (e == perr_argc)
		return ("Invalid number of arguments");
	else if (e == perr_empty)
		return ("Empty file path");
	else if (e == perr_ext)
		return ("Invalid file extension (expected .cub)");
	else if (e == perr_open)
		return ("Failed to open file");
	else if (e == perr_read)
		return ("Configuration file illformed");
	else if (e == perr_alloc)
		return ("Out of memory");
	else if (e == perr_el_dup)
		return ("Duplicate identifier");
	else if (e == perr_el_miss)
		return ("Missing identifier");
	else if (e == perr_id_bad)
		return ("Invalid identifier");
	else if (e == perr_file_empty)
		return ("File is empty");
	else if (e == perr_rgb_bad)
		return ("Invalid RGB value");
	else if (e == perr_path_miss)
		return ("Missing/invalid texture path");
	return ("Unknown error");
}

void	print_perr(t_perr err, const char *str)
{
	const char	*path_for_msg;

	if (str != NULL)
		path_for_msg = str;
	else
		path_for_msg = "";
	if (err == perr_ok)
		return ;
	ft_putendl_fd("Error", 2);
	if (err == perr_ext)
	{
		ft_putstr_fd("Extension invalide pour '", 2);
		ft_putstr_fd((char *)path_for_msg, 2);
		ft_putendl_fd("' (attendu: .cub, sensible a la casse).", 2);
	}
	else
		ft_putendl_fd((char *)perr_str(err), 2);
}
