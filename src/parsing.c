// parsing/parsing.c
#include "parsing.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


char *get_next_line(int fd);

/* ============================================================
**  MAP PARSER
** ============================================================*/

static bool is_map_char(int car)
{
	unsigned char c = (unsigned char)car;
	if (c=='0'||c=='1'||c==' '||c=='N'||c=='S'||c=='E'||c=='W')
		return (true);
	else
		return (false);
}

static int line_len_no_nl(const char *str)
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

static bool is_map_line(const char *str)
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

void mb_init(t_mapbuild *map)
{
	ft_memset(map, 0, sizeof(*map));
	map->player.row = -1;
	map->player.column = -1;
}

static bool mb_grow_buf(t_mapbuild *map, size_t need_more)
{
	size_t want;
	size_t new_cap;
	char *new_buf;
	size_t limit;
	size_t half;

	limit = SIZE_MAX - map->len;
	if (need_more > limit)
		return (false);
	want = map->len + need_more;
	if (want <= map->capacity)
		return (true);
	if (map->capacity == 0)
		new_cap = 256;
	else
		new_cap = map->capacity;
	half = SIZE_MAX / 2;
	while (want > new_cap)
	{
		if (new_cap > half)
			new_cap = want;
		else
			new_cap *= 2;
	}
	new_buf = (char *)realloc(map->buf, new_cap); // NOTE: si besoin, remplace par ta realloc "maison"
	if (!new_buf)
		return (false);
	map->buf = new_buf;
	map->capacity = new_cap;
	return (true);
}

bool mb_push_line(t_mapbuild *map, char *line)
{
	int line_len;
	int i;
	int players_nb;
	int player_column;
	size_t need_more;

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
			if (map->player_count != 0) // un joueur déjà vu sur une ligne précédente
				return (false);
			players_nb ++;
			if (players_nb > 1) // 2-ieme joueur sur la même ligne
				return (false);
			player_column = i;
		}
		i++;
	}
	need_more = (size_t)line_len + 1;
	if (!mb_grow_buf(map, need_more))
		return (false);
	ft_memcpy(map->buf + map->len, line, (size_t)line_len);
	map->len += (size_t)line_len;
	map->buf[map->len] = '\n';
	map->len += 1;
	if (line_len > map->maxw)
		map->maxw = line_len;
	if (players_nb == 1)
	{
		map->player.row = map->rows;
		map->player.column = player_column;
		map->player.dir = line[player_column];
		map->player_count += 1;
	}
	map->rows++; // push réussi
	return (true);
}

void mb_free(t_mapbuild *map)
{
	if (!map)
		return;
	free(map->buf);
	mb_init(map);
}

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
	int r;
	size_t pos;
	int cidx;
	char ch;

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
		ft_memset(out->grid[r], ' ', (size_t)out->columns); // padding à droite
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
		if (r == out->player.row && out->player.column >= 0 && out->player.column < out->columns)
			out->grid[r][out->player.column] = '0';
		r++;
	}
	return (true);
}

bool map_quick_border_check(const t_map *m)
{
	int row_index;
	int column_index;
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
		c = m->grid[0][column_index]; /* haut */
		if (c == '0')
			return (false);
		c = m->grid[m->rows - 1][column_index]; /* bas */
		if (c == '0')
			return (false);
		column_index++;
	}
	// gauche et droite
	row_index = 0;
	while (row_index < m->rows)
	{
		c = m->grid[row_index][0]; /* gauche */
		if (c == '0')
			return (false);
		c = m->grid[row_index][m->columns - 1]; /* droite */
		if (c == '0')
			return (false);
		row_index++;
	}
	return (true);
}

bool map_neighbors_ok(const t_map *m)
{
	int row_index;
	int column_index;
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

/* ============================================================
**  HEADER PARSER
** ============================================================*/

static const char *skip_ws(const char *str)
{
	while (*str == ' ' || *str == '\t' || *str == '\r')
		str ++;
	return (str);
}

static char *dup_range(const char *start, const char *end)
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

static bool parse_colors(const char *str, int *result)
{
	char *end;
	long v;

	if (!str || !result)
		return (false);
	v = strtol(str, &end, 10);
	if (end == str)
		return (false); // rien lu
	while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')
		end++;
	if (*end != '\0')
		return (false);
	if (v < 0 || v > 255)
		return (false);
	*result = (int)v;
	return (true);
}

// Lit une composante 0..255 sans signe, espaces optionnels autour. Avance *pp.
static bool read_component(const char **pp, int *out)
{
	const char *p = *pp;
	char tok[4];
	int nd = 0;

	p = skip_ws(p);
	if (*p < '0' || *p > '9')
		return (false);
	while (nd < 3 && p[nd] >= '0' && p[nd] <= '9')
	{
		tok[nd] = p[nd];
		nd ++;
	}
	tok[nd] = '\0';
	if (nd == 0)
		return (false);
	if (!parse_colors(tok, out))
		return (false);
	p += nd;
	p = skip_ws(p);
	*pp = p;
	return (true);
}

// Consomme une virgule avec espaces optionnels autour, avance *pp.
static bool consume_comma(const char **pp)
{
	const char *p = *pp;

	p = skip_ws(p);
	if (*p != ',')
		return (false);
	p++; // virgule
	p = skip_ws(p);
	*pp = p;
	return (true);
}

static bool parse_triplet(const char *str, t_rgb *dst, const char **end_after)
{
	const char *p;
	int i = 0;
	int val;

	if (!str || !dst)
		return (false);
	p = str;
	while (i < 3)
	{
		if (!read_component(&p, &val))
			return (false);
		if (i == 0) dst->r = val;
		else if (i == 1) dst->g = val;
		else dst->b = val;
		if (i < 2)
		{ // entre R-G et G-B: virgule requise
			if (!consume_comma(&p))
				return (false);
		}
		i++;
	}
	while (*p==' ' || *p=='\t' || *p=='\r' || *p=='\n')
		++p;
	if (end_after)
		*end_after = p;
	if (*p != '\0')
		return (false);
	dst->is_set = true;
	return (true);
}

// Trim début+fin sur une sous-chaîne [start,end)
static bool trim_range(const char *rest, const char **start, const char **end)
{
	const char *p;
	size_t len;
	char ch;

	p = skip_ws(rest);
	if (*p == '\0')
		return (false);
	len = ft_strlen(p);
	while (len > 0)
	{
		ch = p[len - 1];
		if (!(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'))
			break;
		len--;
	}
	if (len == 0)
		return (false);
	*start = p;
	*end = p + len;
	return (true);
}

// vérifie suffixe ".xpm"
static bool path_has_xpm_suffix(const char *start, const char *end)
{
	size_t len;

	len = (size_t)(end - start);
	return (len >= 4) && (ft_strncmp(end - 4, ".xpm", 4) == 0);
}

static char **cfg_slot_for_id(t_config *cfg, t_id id)
{
	if (id==ID_NO) return (&cfg->no);
	if (id==ID_SO) return (&cfg->so);
	if (id==ID_WE) return (&cfg->we);
	if (id==ID_EA) return (&cfg->ea);
	return (NULL);
}

static t_id parse_id_at_start(const char **pline)
{
	const char *p;

	p = skip_ws(*pline);
	t_id id = ID_UNKNOWN;
	if (*p == '\0')
	{
		*pline = p;
		return ID_NONE;
	}
	if (ft_strncmp(p, "NO", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{ id = ID_NO; p += 2; }
	else if (ft_strncmp(p, "SO", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{ id = ID_SO; p += 2; }
	else if (ft_strncmp(p, "WE", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{ id = ID_WE; p += 2; }
	else if (ft_strncmp(p, "EA", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{ id = ID_EA; p += 2; }
	else if (ft_strncmp(p, "F", 1) == 0 && (p[1]=='\0' || p[1]==' ' || p[1]=='\t' || p[1]=='\r'))
	{ id = ID_F; p += 1; }
	else if (ft_strncmp(p, "C", 1) == 0 && (p[1]=='\0' || p[1]==' ' || p[1]=='\t' || p[1]=='\r'))
	{ id = ID_C; p += 1; }
	else if (*p == ' ' || *p == '\t' || *p == '\r')
		id = ID_NONE;
	else
		id = ID_UNKNOWN;
	*pline = p;
	return (id);
}

static bool handle_texture_entry(t_id id, const char *rest, t_config *cfg, t_perr *perr)
{
	const char *start;
	const char *end;
	char **slot;
	char *path;

	if (!trim_range(rest, &start, &end))
	{
		if (perr) *perr = PERR_PATH_MISS;
		return (false);
	}
	if (!path_has_xpm_suffix(start, end))
	{
		if (perr) *perr = PERR_PATH_MISS;
		return (false);
	}
	slot = cfg_slot_for_id(cfg, id);
	if (!slot)
	{
		if (perr) *perr = PERR_ID_BAD;
		return (false);
	}
	if (*slot)
	{
		if (perr) *perr = PERR_EL_DUP;
		return (false);
	}
	path = dup_range(start, end);
	if (!path)
	{
		if (perr) *perr = PERR_ALLOC;
		return (false);
	}
	*slot = path;
	if (perr) *perr = PERR_OK;
	return (true);
}

static bool handle_rgb_entry(t_id id, const char *rest, t_config *cfg, t_perr *perr)
{
	t_rgb tmp;
	const char *after;

	tmp.r = 0; tmp.g = 0; tmp.b = 0; tmp.is_set = false;
	after = NULL;
	if (!parse_triplet(rest, &tmp, &after))
	{
		if (perr) *perr = PERR_RGB_BAD;
		return (false);
	}
	if (*skip_ws(after) != '\0')
	{
		if (perr) *perr = PERR_RGB_BAD;
		return (false);
	}
	if (id == ID_F)
	{
		if (cfg->floor_rgb.is_set)
		{
			if (perr) *perr = PERR_EL_DUP;
			return (false);
		}
		cfg->floor_rgb = tmp;
	}
	else
	{
		if (cfg->ceil_rgb.is_set)
		{
			if (perr) *perr = PERR_EL_DUP;
			return (false);
		}
		cfg->ceil_rgb = tmp;
	}
	if (perr) *perr = PERR_OK;
	return (true);
}

bool parse_header_line(const char *line, t_config *cfg, t_perr *perr)
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
		return handle_texture_entry(id, p, cfg, perr);
	else
		return handle_rgb_entry(id, p, cfg, perr);
}

static const char *perr_str(t_perr e)
{
	switch (e)
	{
		case PERR_OK:      return "OK";
		case PERR_ALLOC:   return "ALLOC";
		case PERR_ID_BAD:  return "ID_BAD";
		case PERR_PATH_MISS:return "PATH_BAD/MISS";
		case PERR_RGB_BAD: return "RGB_BAD";
		case PERR_EL_DUP:  return "EL_DUP";
		case PERR_EL_MISS: return "EL_MISS";
		case PERR_ARGC:    return "ARGC";
		case PERR_EMPTY:   return "EMPTY";
		case PERR_EXT:     return "EXT";
		case PERR_DIR:     return "DIR";
		case PERR_OPEN:    return "OPEN";
		case PERR_READ:    return "READ";
		default:           return "UNKNOWN";
	}
}

static void cfg_init(t_config *c)
{
	ft_memset(c, 0, sizeof(*c));
}

static void cfg_free(t_config *c)
{
	free(c->no);
	free(c->so);
	free(c->we);
	free(c->ea);
	ft_memset(c, 0, sizeof(*c));
}

static bool rgb_is_set(const t_rgb *c)
{
	return (c->is_set);
}

t_perr header_complete(const t_config *cfg)
{
	if (!cfg->no || !cfg->so || !cfg->we || !cfg->ea)
		return PERR_EL_MISS; // au moins une texture manquante
	if (!rgb_is_set(&cfg->floor_rgb) || !rgb_is_set(&cfg->ceil_rgb))
		return PERR_EL_MISS; // couleur manquante
	return PERR_OK;
}

void dump_header(const t_config *cfg, const char *title)
{
	const char *t;

	if (!cfg) return;
	t = title ? title : "HEADER";
	printf("[%s]\n", t);
	printf(" NO: %s\n", cfg->no ? cfg->no : "(null)");
	printf(" SO: %s\n", cfg->so ? cfg->so : "(null)");
	printf(" WE: %s\n", cfg->we ? cfg->we : "(null)");
	printf(" EA: %s\n", cfg->ea ? cfg->ea : "(null)");
	printf(" F : %s (%d,%d,%d)\n",
		cfg->floor_rgb.is_set ? "set" : "unset",
		cfg->floor_rgb.r, cfg->floor_rgb.g, cfg->floor_rgb.b);
	printf(" C : %s (%d,%d,%d)\n",
		cfg->ceil_rgb.is_set ? "set" : "unset",
		cfg->ceil_rgb.r, cfg->ceil_rgb.g, cfg->ceil_rgb.b);
}

/* ============================================================
**  PARSE GLOBAL: une seule passe header + map (un seul main)
** ============================================================*/

static bool is_blank_or_ws_only(const char *s)
{
	const unsigned char *p = (const unsigned char*)s;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	return (*p == '\0');
}

// Retourne true si tout le fichier est parsé correctement.
// - Remplit cfg (header)
// - Construit mb (buffer brut de la map)
// NB: on garde la logique de parse_map_fd et parse_header_line.
static bool parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
	char *line = NULL;
	bool in_map = false;
	int len;

	if (out_err) *out_err = PERR_OK;
	mb_init(mb);

	while ((line = get_next_line(fd)) != NULL)
	{
		len = line_len_no_nl(line);

		// Pas encore dans la carte : on traite l'en-tête
		if (!in_map)
		{
			if (is_map_line(line))
			{
				// on entre dans la carte
				in_map = true;
				mb->started = true;
				if (!mb_push_line(mb, line))
				{
					free(line);
					if (out_err) *out_err = PERR_READ;
					return false;
				}
				free(line);
				continue;
			}
			// header: lignes vides → ok ; sinon essayer de parser
			if (!is_blank_or_ws_only(line))
			{
				t_perr perr = PERR_OK;
				if (!parse_header_line(line, cfg, &perr))
				{
					free(line);
					if (out_err) *out_err = perr ? perr : PERR_ID_BAD;
					return false;
				}
			}
			free(line);
			continue;
		}

		// Déjà dans la carte :
		if (mb->ended)
		{
			// après la fin de map, seules lignes vides autorisées
			if (len > 0 && !is_blank_or_ws_only(line))
			{
				free(line);
				if (out_err) *out_err = PERR_READ;
				return false;
			}
			free(line);
			continue;
		}
		if (is_map_line(line))
		{
			if (!mb_push_line(mb, line))
			{
				free(line);
				if (out_err) *out_err = PERR_READ;
				return false;
			}
		}
		else
		{
			mb->ended = true; // fin de map ; on laissera la boucle vérifier le reste
		}
		free(line);
	}

	// vérifs basiques
	if (!in_map || mb->rows <= 0 || mb->player_count != 1)
	{
		if (out_err) *out_err = PERR_READ;
		return false;
	}
	return true;
}

/* ============================================================
**  MAIN unique (test et démo)
** ============================================================*/

int main(int ac, char **av)
{
	int fd;
	t_mapbuild mb;
	t_map m;
	t_config cfg;
	t_perr perr = PERR_OK;

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

	cfg_init(&cfg);

	// 1) parse header + map en une passe
	if (!parse_file_fd(fd, &cfg, &mb, &perr))
	{
		close(fd);
		mb_free(&mb);
		cfg_free(&cfg);
		fprintf(stderr, "Parse error: %s\n", perr_str(perr));
		return (2);
	}
	close(fd);

	// 2) valider l’en-tête
	perr = header_complete(&cfg);
	if (perr != PERR_OK)
	{
		mb_free(&mb);
		cfg_free(&cfg);
		fprintf(stderr, "Header incomplete: %s\n", perr_str(perr));
		return (3);
	}

	// 3) construire la grille "split" (char **)
	if (!map_build_split(&mb, &m))
	{
		mb_free(&mb);
		cfg_free(&cfg);
		fprintf(stderr, "Build error: cannot build split grid.\n");
		return (4);
	}
	mb_free(&mb);

	// 4) quick checks fermeture
	if (!map_quick_border_check(&m) || !map_neighbors_ok(&m))
	{
		fprintf(stderr, "Map not closed/coherent (quick checks failed).\n");
		map_free_split(&m);
		cfg_free(&cfg);
		return (5);
	}

	// 5) affichage debug
	printf("== HEADER ==\n");
	dump_header(&cfg, "CONFIG");
	printf("\n== MAP ==\n");
	printf("rows=%d cols=%d | player=(r=%d,c=%d,dir=%c)\n",
		m.rows, m.columns, m.player.row, m.player.column, m.player.dir);
	for (int r = 0; r < m.rows; ++r)
	{
		for (int c = 0; c < m.columns; ++c)
		{
			char ch = m.grid[r][c];
			putchar(ch == ' ' ? '.' : ch);
		}
		putchar('\n');
	}

	// 6) free
	map_free_split(&m);
	cfg_free(&cfg);
	return (EXIT_SUCCESS);
}
