/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:12:17 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/13 15:52:18 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"


/*static void	tabs_to_spaces(char *str) //faut il accepter \t en les changeant en ' ' ???
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
}*/

static bool	is_map_char(int	car)
{
	unsigned char c = (unsigned char)car;
	if (c=='0'||c=='1'||c==' '||c=='N'||c=='S'||c=='E'||c=='W')
		return (true);
	else
		return (false);
}

static int	line_len_no_nl(const char *str)
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

static bool	is_map_line(const char *str)
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
	map->buf[map->len] = '\n';//c'etait + 1
	map->len += 1;
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

bool	parse_map_fd(int fd, t_mapbuild *mb)
{
	char	*line;
	int		len;

	line = NULL;
	len = 0;
	mb_init(mb);
	line = get_next_line(fd);
	while (line)
	{
		len = line_len_no_nl(line);
		//on n'est pas encore dans la carte
		if (!mb->started && !is_map_line(line))
		{
			free(line);
			line = get_next_line(fd);
			continue;
		}
		//on entre dans la carte
		if (!mb->started && is_map_line(line))
			mb->started = true;
		//si la carte est déjà terminée, seules des lignes vides sont autorisées
		if (mb->ended)
		{
			if (len > 0)
			{
				free(line);
				return (false); //on refuse le contenu après la carte
			}
			free(line);
			line = get_next_line(fd);
			continue;
		}
		//pendant la carte
		if (mb->started && is_map_line(line))
		{
			if (!mb_push_line(mb, line))
			{
				free(line);
				return (false); //erreur de carte
			}
		}
		else if (mb->started && !is_map_line(line))
		{
			mb->ended = true; //fermer la carte
			//on ne retourne pas encore : on laisse la boucle vérifier que le reste est vide
			free(line);
			line = get_next_line(fd);
			continue;
		}
		free(line);
		line = get_next_line(fd);
	}
	//au moins une ligne et exactement un joueur doivent être présents
	if (mb->rows <= 0 || mb->player_count != 1)
		return (false);
	return (true);
}
//-----------Construction carte------------------------------------------
//copier une ligne depuis mb->buf vers map->grid :
//  - écrire jusqu'à '\n' ou fin de buffer
// - ignorer les '\r'
// - pad déjà fait ailleurs (grid pré-remplie en ' ')
//  - *pos est avancé juste après le '\n' s'il existe
//on calcule l'index linéaire index = row * columns + col (pour savoir où écrire/lire dans le tab 1D)
/*static void	copy_one_row(const t_mapbuild *mb, t_map *map, int r, size_t *pos)
{
	size_t	row_base;
	int		col;
	char	ch;

	row_base = (size_t)r * (size_t)map->columns;
	col = 0;
	while (*pos < mb->len && mb->buf[*pos] != '\n' && col < map->columns)
	{
		ch = mb->buf[*pos];
		if (ch != '\r')
		{
			map->grid[row_base + (size_t)col] = ch;
			col++;
		}
		(*pos)++;
	}
	if (*pos < mb->len && mb->buf[*pos] == '\n')
		(*pos)++;
}

//Remplir toute la grille depuis le gros buffer, et remplacer le player par '0'
static bool	fill_grid_from_buf(const t_mapbuild *mb, t_map *map)
{
	size_t	pos;
	int		r;
	size_t	row_base;

	pos = 0;
	r = 0;
	while (r < map->rows && pos < mb->len)
	{
		copy_one_row(mb, map, r, &pos);
		if (r == map->player.row && map->player.column >= 0
			&& map->player.column < map->columns)
		{
			row_base = (size_t)r * (size_t)map->columns;
			map->grid[row_base + (size_t)map->player.column] = '0';
		}
		r++;
	}
	return (true);
}*/

/*
//Construir la map finale (rows*columns), grilles paddées en ' '
t_map	*finalize_map(const t_mapbuild *mb)
{
	t_map	*map;
	size_t	total;

	map = NULL;
	total = 0;
	if (!mb)
		return (NULL);
	if (mb->rows <= 0 || mb->maxw <= 0 || mb->player_count != 1)
		return (NULL);//sortir du programme ici à voir 
	map = (t_map *)calloc(1, sizeof(*map));//allouer la struct finale
	if (!map)
		return (NULL);//sortir du programme ici
	map->rows = mb->rows;
	map->columns = mb->maxw;
	map->player = mb->player;
	total = (size_t)map->rows * (size_t)map->columns;//allouer la grille 1D (rows * columns)
	map->grid = (char *)malloc(total);
	if (!map->grid)//sortir du programme ici
	{
		free(map);
		return (NULL);
	}
	ft_memset(map->grid, ' ', total);
	if (!fill_grid_from_buf(mb, map))
	{
		free(map->grid);
		free(map);
		return (NULL);
	}
	return (map);
}*/

static void map_free_split(t_map *m)
{
    int r;

    if (!m)
        return;
    if (m->grid)
    {
        r = 0;
        while (r < m->rows)
        {
            free(m->grid[r]);
            r++;
        }
        free(m->grid);
    }
    ft_memset(m, 0, sizeof(*m));
}

static bool map_build_split(const t_mapbuild *mb, t_map *out)
{
    int     r;
    size_t  pos;
    int     cidx;
    char    ch;

    if (!mb || !out)
        return (false);
    if (mb->rows <= 0 || mb->maxw <= 0 || mb->player_count != 1)
        return (false);
    ft_memset(out, 0, sizeof(*out));
    out->rows = mb->rows;
    out->columns = mb->maxw;
    out->player = mb->player;
    out->grid = (char **)malloc((size_t)out->rows * sizeof(char *));
    if (!out->grid)
        return (false);
    ft_memset(out->grid, 0, (size_t)out->rows * sizeof(char *));
    r = 0;
    while (r < out->rows)
    {
        out->grid[r] = (char *)malloc((size_t)out->columns);
        if (!out->grid[r])
        {
            out->rows = r;
            map_free_split(out);
            return (false);
        }
        ft_memset(out->grid[r], ' ', (size_t)out->columns); //espaces  à droite
        r++;
    }
    pos = 0;
    r = 0;
    while (r < out->rows && pos < mb->len)
    {
        cidx = 0;
        while (pos < mb->len && mb->buf[pos] != '\n' && cidx < out->columns)
        {
            ch = mb->buf[pos];
            if (ch != '\r')
            {
                out->grid[r][cidx] = ch; /* recouvre le padding */
                cidx++;
            }
            pos++;
        }
        if (pos < mb->len && mb->buf[pos] == '\n')
            pos++;
        // remplace le joueur par '0'
        if (r == out->player.row
            && out->player.column >= 0
            && out->player.column < out->columns)
        {
            out->grid[r][out->player.column] = '0';
        }
        r++;
    }
    return (true);
}

void	mb_free(t_mapbuild *map)
{
	if (!map)
		return;
	free(map->buf);
	mb_init(map);
}

bool map_quick_border_check(const t_map *m)
{
    int  row_index;
    int  column_index;
    char c;

    row_index = 0;
    column_index = 0;
    if (!m || !m->grid)
        return (false);
    if (m->rows <= 0 || m->columns <= 0)
        return (false);
    // haut et bas
    while (column_index < m->columns)
    {
        c = m->grid[0][column_index];                     /* haut */
        if (c == '0')
            return (false);
        c = m->grid[m->rows - 1][column_index];           /* bas */
        if (c == '0')
            return (false);
        column_index++;
    }
    //gauche et droite
    row_index = 0;
    while (row_index < m->rows)
    {
        c = m->grid[row_index][0];                        /* gauche */
        if (c == '0')
            return (false);
        c = m->grid[row_index][m->columns - 1];           /* droite */
        if (c == '0')
            return (false);
        row_index++;
    }
    return (true);
}

bool map_neighbors_ok(const t_map *m)
{
    int  row_index;
    int  column_index;
    char c;

    if (!m || !m->grid)
        return (false);
    row_index = 0;
    while (row_index < m->rows)
    {
        column_index = 0;
        while (column_index < m->columns)
        {
            c = m->grid[row_index][column_index];
            if (c == '0')
            {
                if (row_index > 0 && m->grid[row_index - 1][column_index] == ' ')
                    return (false);
                if (row_index + 1 < m->rows && m->grid[row_index + 1][column_index] == ' ')
                    return (false);
                if (column_index > 0 && m->grid[row_index][column_index - 1] == ' ')
                    return (false);
                if (column_index + 1 < m->columns && m->grid[row_index][column_index + 1] == ' ')
                    return (false);
            }
            column_index++;
        }
        row_index++;
    }
    return (true);
}

/////////////////////tester//////////////////////////////
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parsing_map.h"

/* affichage carte, espaces -> '.' (version char** grid) */
static void dump_map_split(const t_map *m)
{
    int row_index;
    int column_index;
    char c;

    if (!m || !m->grid) return;
    row_index = 0;
    while (row_index < m->rows)
    {
        column_index = 0;
        while (column_index < m->columns)
        {
            c = m->grid[row_index][column_index];
            putchar(c == ' ' ? '.' : c);
            column_index++;
        }
        putchar('\n');
        row_index++;
    }
}

int main(int ac, char **av)
{
    int        fd;
    t_mapbuild mb;
    t_map      m;   /* on laisse map sur la pile; map_build_split va allouer m.grid[r] */

    if (ac < 2)
    {
        fprintf(stderr, "Usage: %s <map.cub>\n", av[0]);
        return (EXIT_FAILURE);
    }
    fd = open(av[1], O_RDONLY);
    if (fd < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
    /* 1) parse la section MAP dans mb (remplit mb->buf, rows, maxw, player, etc.) */
    if (!parse_map_fd(fd, &mb))
    {
        close(fd);
        mb_free(&mb);
        fprintf(stderr, "Parse error: invalid map section.\n");
        return (2);
    }
    close(fd);
    /* 2) construis directement la vue 2D "split" (char**), padding à droite depuis mb->buf */
    if (!map_build_split(&mb, &m))
    {
        mb_free(&mb);
        fprintf(stderr, "Build error: cannot build split grid.\n");
        return (3);
    }
    /* on n'a plus besoin de mb->buf après la construction */
    mb_free(&mb);
    /* 3) quick checks fermeture */
    if (!map_quick_border_check(&m) || !map_neighbors_ok(&m))
    {
        fprintf(stderr, "Map not closed/coherent (quick checks failed).\n");
        map_free_split(&m);
        return (4);
    }
    /* 4) affichage debug */
    printf("rows=%d cols=%d | player=(r=%d,c=%d,dir=%c)\n",
           m.rows, m.columns, m.player.row, m.player.column, m.player.dir);
    dump_map_split(&m);
    /* 5) free */
    map_free_split(&m);
    return (EXIT_SUCCESS);
}

