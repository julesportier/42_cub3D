/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:51:40 by vakozhev          #+#    #+#             */
/*   Updated: 2025/11/19 13:01:50 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

t_perr	validate_params(char **argv, const char **out_path)
{
	const char	*path;
	t_perr		err;

	err = perr_ok;
	path = NULL;
	path = argv[1];
	if (!check_extension(path))
		err = perr_ext;
	if (err != perr_ok)
	{
		print_perr(err);
		return (err);
	}
	*out_path = path;
	return (perr_ok);
}

const char	*perr_str(t_perr e)
{
	if (e == perr_map_closed)
		return ("Map is not closed (found '0' on border)");
	else if (e == perr_map_space)
		return ("Invalid map: floor '0' adjacent to void ' '");
	else if (e == perr_header_missing)
		return ("Header is missing");
	if (e == perr_open)
		return ("Failed to open file");
	else if (e == perr_read)
		return ("Invalid map");
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

void	print_perr(t_perr err)
{
	if (err == perr_ok)
		return ;
	ft_putendl_fd("Error", 2);
	if (err == perr_ext)
		ft_putendl_fd("Invalid config file extension", 2);
	else if (err == perr_player_none)
		ft_putendl_fd("No player on the map", 2);
	else if (err == perr_player_dup)
		ft_putendl_fd("Multiple players on the map", 2);
	else if (err == perr_map_empty)
		ft_putendl_fd("Map section is missing or empty", 2);
	else if (err == perr_trailing)
		ft_putendl_fd("Trailing content into or after the map", 2);
	else if (err == perr_id_missing_content)
		ft_putendl_fd("No content after identifier", 2);
	else
		ft_putendl_fd((char *)perr_str(err), 2);
}
