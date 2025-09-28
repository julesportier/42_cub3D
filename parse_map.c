/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:12:17 by vakozhev          #+#    #+#             */
/*   Updated: 2025/09/28 13:57:14 by vakozhev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h> 

typedef struct s_player
{
    int  row;
    int  column;
    char dir;
} t_player;

typedef struct s_map
{
    int   rows;
    int	  columns;
    char *grid;
    t_player player;
} t_map;

typedef struct s_mapbuild
{
    char *buf;
    size_t len;
    size_t capacity;
    int  *offs;
    int *lens;
    int   rows;
    int rows_capacity;
    int   maxw;
    t_player player;
    int     player_count;
    bool    started;
    bool    ended;
} t_mapbuild;

void	tabs_to_spaces(char *str)
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

bool	is_map_char(int	car) //on n'accepte plus \t ici
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

int	main(int argc, char **argv)
{
	int   fd = (argc > 1) ? open(argv[1], O_RDONLY) : STDIN_FILENO;
	char *line;
	int   n = 1;

	if (fd < 0) { perror("open"); return 1; }

	while ((line = get_next_line(fd)) != NULL)
	{
		int len = line_len_no_nl(line);
		if (len > 0)                 // ignore lignes vides "logiques"
		{
			tabs_to_spaces(line);    // in-place, 0 malloc
			if (is_map_line(line))   // ne lit que les len premiers chars
			{
				// ici tu pousserais dans mb_push_line(line)
				// pour le test, on affiche juste un aperçu:
				printf("L%03d len=%d map=1 -> \"", n, len);
				for (int i = 0; i < len; i++)
					putchar(line[i] == ' ' ? '.' : (unsigned char)line[i]);
				puts("\"");
			}
			else
			{
				printf("L%03d len=%d map=0\n", n, len);
			}
		}
		free(line);  // toujours free la ligne fournie par GNL
		n++;
	}
	if (argc > 1) close(fd);
	return 0;
}
