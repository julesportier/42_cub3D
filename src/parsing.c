/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:19:43 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/21 12:05:40 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_bool is_map_line(const char *str)
{
	int len;
	int i;

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

t_bool is_blank_or_ws_only(const char *s)
{
	const unsigned char *p = (const unsigned char*)s;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	return (*p == '\0');
}

/*t_bool parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
	char *line = NULL;
	t_bool in_map = false;
	int len;
	if (out_err) *out_err = PERR_OK;
	mb_init(mb);
	while ((line = get_next_line(fd)) != NULL)
	{
		len = line_len_no_nl(line);
		if (!in_map)
		{
			if (is_map_line(line))
			{
				in_map = true;
				mb->started = true;
				if (!mb_push_line(mb, line))
				{
					free(line);
					if (out_err)
						*out_err = PERR_READ;
					return (false);
				}
				free(line);
				continue;
			}
			if (!is_blank_or_ws_only(line))
			{
				t_perr perr = PERR_OK;
				if (!parse_header_line(line, cfg, &perr))
				{
					free(line);
					if (out_err)
						*out_err = perr ? perr : PERR_ID_BAD; //attention a refaire
					return (false);
				}
			}
			free(line);
			continue;
		}
		if (mb->ended)
		{
			if (len > 0 && !is_blank_or_ws_only(line))
			{
				free(line);
				if (out_err) *out_err = PERR_READ;
				return (false);
			}
			free(line);
			continue;
		}
		if (is_map_line(line))
		{
			if (!mb_push_line(mb, line))
			{
				free(line);
				if (out_err)
					*out_err = PERR_READ;
				return (false);
			}
		}
		else
		{
			mb->ended = true;
		}
		free(line);
	}
	if (!in_map || mb->rows <= 0 || mb->player_count != 1)
	{
		if (out_err)
			*out_err = PERR_READ;
		return (false);
	}
	return (true);
}*/

static	t_bool handle_header_line(const char *line, t_config *cfg, t_perr *out_err)
{
	t_perr	perr;
	if (is_blank_or_ws_only(line))
		return (true);
	perr = PERR_OK;
	if (!parse_header_line(line, cfg, &perr))
	{
		if (out_err)
		{
			if (perr != PERR_OK)
				*out_err = perr;
			else
				*out_err = PERR_ID_BAD;
		}
		return (false);
	}
	return (true);
}

static	t_bool	parse_map_rest(int fd, t_mapbuild *mb, t_perr *out_err)
{
    char *line;
    int   len;

    while ((line = get_next_line(fd)) != NULL)
    {
        len = line_len_no_nl(line);

        if (mb->ended) {
            // après la fin de la carte : seules les lignes vides/blanches sont permises
            if (len > 0 && !is_blank_or_ws_only(line)) {
                free(line);
                if (out_err) *out_err = PERR_READ;
                return (false);
            }
            free(line);
            continue;
        }
        if (is_map_line(line)) {
            if (!mb_push_line(mb, line)) {
                free(line);
                if (out_err) *out_err = PERR_READ;
                return (false);
            }
        } else {
            mb->ended = true; // fin de carte; la boucle validera le reste
        }
        free(line);
    }
    // vérification finale identique à l'original
    if (mb->rows <= 0 || mb->player_count != 1) {
        if (out_err) *out_err = PERR_READ;
        return (false);
    }
    return (true);
}

// --- 3) Wrapper public (signature inchangée) : lit l'en-tête puis bascule carte ---
t_bool parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
    char   *line;
    int     len;

    if (out_err) *out_err = PERR_OK;
    mb_init(mb);

    while ((line = get_next_line(fd)) != NULL)
    {
        len = line_len_no_nl(line);

        // bascule dès la première ligne de carte rencontrée
        if (is_map_line(line)) {
            mb->started = true;
            if (!mb_push_line(mb, line)) {
                free(line);
                if (out_err) *out_err = PERR_READ;
                return (false);
            }
            free(line);
            // continuer la lecture de la carte (et du trailing) sur le même fd
            return parse_map_rest(fd, mb, out_err);
        }

        // sinon, on est encore dans l'en-tête
        if (!handle_header_line(line, cfg, out_err)) {
            free(line);
            return (false);
        }
        free(line);
    }

    // EOF atteint sans jamais entrer dans la carte
    if (out_err) *out_err = PERR_READ;
    return (false);
}


void cfg_init(t_config *c)
{
	ft_memset(c, 0, sizeof(*c));
}

void mb_init(t_mapbuild *map)
{
	ft_memset(map, 0, sizeof(*map));
	map->player.row = -1;
	map->player.column = -1;
}

t_bool parse_header_line(const char *line, t_config *cfg, t_perr *perr)
{
	const char *p;

	if (perr) *perr = PERR_OK;
	if (!line)
		return (false);
	p = line;
	t_id id = parse_id_at_start(&p);
	if (id == ID_NONE)
		return (false);
	if (id == ID_UNKNOWN)
	{
		if (perr) *perr = PERR_ID_BAD;
		return (false);
	}
	if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA)
		return handle_texture(id, p, cfg, perr);
	else
		return handle_rgb(id, p, cfg, perr);
}

t_perr parsing_load(const char *path, t_parsed *out)
{
    int         fd;
    t_mapbuild  mb;
    t_map       m;
    t_config    cfg;
    t_perr      perr;

    if (!path || !out)
        return (PERR_EMPTY);
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (PERR_OPEN);
    cfg_init(&cfg);
    perr = PERR_OK;
    if (!parse_file_fd(fd, &cfg, &mb, &perr))
    {
        close(fd);
        mb_free(&mb);
        cfg_free(&cfg);
        return (perr ? perr : PERR_READ);//a refaire
    }
    close(fd);
    perr = header_complete(&cfg);
    if (perr != PERR_OK)
    {
        mb_free(&mb);
        cfg_free(&cfg);
        return (perr);
    }
    if (!map_build_split(&mb, &m))
    {
        mb_free(&mb);
        cfg_free(&cfg);
        return (PERR_ALLOC);
    }
    mb_free(&mb);
    if (!map_quick_border_check(&m) || !map_neighbors_ok(&m))
    {
        map_free_split(&m);
        cfg_free(&cfg);
        return (PERR_READ);
    }
    out->config = cfg;
    out->map    = m;
    return (PERR_OK);
}

/******cote exec*/
/*t_parsed parsed;
t_perr e = parsing_load("maps/level.cub", &parsed);
if (e != PERR_OK)
{
    // afficher erreur via print_perr(e, "maps/map.cub") ??
    // puis quitter 
} 
else
{
    // utiliser parsed.config / parsed.map ...
    parsing_free(&parsed);
}*/







