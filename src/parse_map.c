/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:12:17 by vakozhev          #+#    #+#             */
/*   Updated: 2025/09/28 19:55:21 by vakozhev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_map.h"


void	tabs_to_spaces(char *str) //faut il accepter \t en les changeant en ' ' ???
{
	int	i;

	i = 0;
	if (!str)
		return;
	while (str[i])
	{
		if (str[i] == '\t')
			str[i] = ' ';
		i ++;
	}
}

bool	is_map_char(int	car) //on n'accepte plus \t ici, on convertit avant
{
	unsigned char c = (unsigned char)car;
	if (c=='0'||c=='1'||c==' '||c=='N'||c=='S'||c=='E'||c=='W')
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

bool	is_map_line(const char *str)
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

void	mb_init(t_mapbuild *map)
{
    ft_memset(map, 0, sizeof(*map));
    map->player.row = -1;
    map->player.column = -1;
}

static bool mb_grow_buf(t_mapbuild *map, size_t need_more)
{
	size_t	want; //want = len de buff actuel + need_more (= len de la ligne qu'on veut ajouter + 1(= \n))
	size_t	new_cap; //a combien reallouer 
	char	*new_buf; //buff apres realloc
	size_t	limit; //eviter overflow de want
	size_t	half; //pour eviter l'overflow lors de doublement
	
	limit = SIZE_MAX - map->len; //calculer en amont si want ne depassera pas size_max 
	if (need_more > limit) 
		return (false);
	want = map->len + need_more;
    if (want <= map->capacity) //si l'espace actuel suffit, on ne fait rien
		return (true);	
    if (map->capacity == 0)
		new_cap = 256;
	else
		new_cap = map->capacity;
	half = SIZE_MAX / 2;
    while (want > new_cap)
    {
		if (new_cap > half) //on ne pourra plus doubler
			new_cap = want; //on a verifie au debut que want ne depasse pas size_max
		else
			new_cap *= 2; //on double jusqu'à couvrir want
	}
    new_buf = (char *)realloc(map->buf, new_cap); //faire la fonction realloc 'maison'!!!
    if (!new_buf)
		return (false);
    map->buf = new_buf; 
    map->capacity = new_cap;
    return (true);
}

bool	mb_push_line(t_mapbuild *map, char *line)
{
	int		line_len;
	int		i;
	int		players_nb;
	int		player_column;
	size_t	need_more;

	line_len = line_len_no_nl(line);
	i = 0;
	players_nb = 0;
	player_column = -1;

	if (line_len == 0)
		return (false);
	while (i < line_len)
	{
		unsigned char c;
		c = (unsigned char)line[i];
		if (!is_map_char(c))
			return (false);
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			if (map->player_count != 0)   // un joueur déjà vu sur une ligne précédente
				return (false);
			players_nb ++;
			if (players_nb > 1)           // 2-ieme joueur sur la même ligne
				return (false);
			player_column = i;
		}
		i++;
	}
	need_more = (size_t)line_len + 1;
	if (!mb_grow_buf(map, need_more))
		return (false);
	ft_memcpy(map->buf + map->len, line, (size_t)line_len); // copier line_len octets a la fin des donnees deja copiées précédemment
	map->len += (size_t)line_len; // on met a jour le buff
	map->buf[map->len + 1] = '\n';
	if (line_len > map->maxw)
		map->maxw = line_len; // on met a jour la ligne la plus longue (largeur max)
	if (players_nb == 1)
	{
		map->player.row = map->rows;
		map->player.column = player_column;
		map->player.dir = line[player_column];
		map->player_count += 1;
	}
	map->rows++; // push réussi, on a stocke une ligne de plus
	return (true);
}
