/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:51:40 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/19 15:16:42 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_bool	check_extension(const char *path)
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
		return (PERR_ARGC);
	path = argv[1];
	if (path == NULL || *path == '\0')
		return (PERR_EMPTY);
	if (!check_extension(path))
		return (PERR_EXT);
	if (out_path)
		*out_path = path;
	return (PERR_OK);
}

void	print_perr(t_perr err, const char *str)
{
	const char	*path_for_msg;
	if (str != NULL)
		path_for_msg = str;
	else
		path_for_msg = "";
	if (err == PERR_OK)
		return ;
	ft_putendl_fd("Error", 2);
	if (err == PERR_ARGC)
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
	else if (err == PERR_EMPTY)
		ft_putendl_fd("Chemin de fichier vide.", 2);
	else if (err == PERR_EXT)
	{
		ft_putstr_fd("Extension invalide pour '", 2);
		ft_putstr_fd((char *)path_for_msg, 2);
		ft_putendl_fd("' (attendu: .cub, sensible a la casse).", 2);
	}
	else
		ft_putendl_fd("Parametres invalides.", 2);
}
