#include "parsing.h"

bool is_blank_or_ws_only(const char *s)
{
	const unsigned char *p = (const unsigned char*)s;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	return (*p == '\0');
}

bool parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
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
					if (out_err)
						*out_err = PERR_READ;
					return (false);
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
					if (out_err)
						*out_err = perr ? perr : PERR_ID_BAD; //attention a refaire sans extr ter
					return (false);
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
			mb->ended = true; // fin de map ; on laissera la boucle vérifier le reste
		}
		free(line);
	}
	// vérif basique/global
	if (!in_map || mb->rows <= 0 || mb->player_count != 1)
	{
		if (out_err)
			*out_err = PERR_READ;
		return (false);
	}
	return (true);
}

void dump_header(const t_config *cfg, const char *title) //attention, cette fonction est pour tester le main uniquement
{
	const char *t;

	if (!cfg)
		return;
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

// MAIN attention pour tester uniquement

int main(int ac, char **av)
{
	int fd;
	t_mapbuild mb;
	t_map m;
	t_config cfg;
	const char *path = NULL;
	t_perr v = validate_params(ac, av, &path);
	if (v != PERR_OK)
	{
		print_perr(v, path);
		return (EXIT_FAILURE);
	}
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
	for (int r = 0; r < m.rows; ++r) //boucle for attention -> main de test uniquement
	{
		for (int c = 0; c < m.columns; ++c)
		{
			char ch = m.grid[r][c];
			putchar(ch == ' ' ? '.' : ch); //expression interdite -> main de test uniquement
		}
		putchar('\n');
	}
	// 6) free
	map_free_split(&m);
	cfg_free(&cfg);
	return (EXIT_SUCCESS);
}
