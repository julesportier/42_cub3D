#include "parsing.h"

static int push_map_line(t_config *c, const char *line)
{
    if (c->map_h + 1 > c->map_cap) {
        size_t newcap = c->map_cap ? c->map_cap * 2 : 16;
        char **tmp = realloc(c->map_lines, newcap * sizeof *tmp);
        if (!tmp) return -1;
        c->map_lines = tmp;
        c->map_cap   = newcap;
    }
    c->map_lines[c->map_h] = strdup(line);
    if (!c->map_lines[c->map_h]) return -1;
    c->map_h++;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file.cub\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    t_config cfg;
    cfg_init(&cfg);

    t_perr perr = PERR_OK;
    char  *line = NULL;
    int    header_done = 0;
    int    err = 0;

    while ((line = get_next_line(fd)) != NULL)
    {
        // retire le \n final si présent (facultatif mais pratique)
        size_t n = strlen(line);
        while (n && (line[n-1] == '\n' || line[n-1] == '\r')) line[--n] = '\0';
		fprintf(stderr, "DBG: line='%s'\n", line);
        if (!header_done)
        {
            bool handled = parse_header_line(line, &cfg, &perr);
            if (handled)
            {
                // ligne header traitée, on continue
                free(line);
                // vérifie si tout le header est déjà complet (optionnel ici)
                t_perr hc = header_complete(&cfg);
                if (hc == PERR_OK) header_done = 1;
                continue;
            }
            else
            {
				if (perr != PERR_OK)
				{
					free(line);
					err = 1;
					break;
				}
			// 🔽 ajout temporaire
				fprintf(stderr, "DBG: handled=%d perr=%d for line='%s'\n", handled, perr, line);
            // pas header : peut être ligne vide avant map -> on ignore les vides
				const char *p = line;
				while (*p == ' ' || *p == '\t') p++;
				if (*p == '\0') { free(line); continue; }

            // sinon : c'est le début de la map
				header_done = 1;
			}

        // A partir d'ici: on empile la map (y compris la 1ère ligne non-header)
			if (push_map_line(&cfg, line) != 0)
			{
				fprintf(stderr, "OOM pushing map line\n");
				free(line);
				err = 1;
				break;
			}
			free(line);
		}
	}

    close(fd);
	dump_header(&cfg, "before-final-check");
    // Vérification finale du header (au cas où il manquait des éléments)
    if (!err)
    {
        t_perr hc = header_complete(&cfg);
        if (hc != PERR_OK)
        {
            fprintf(stderr, "Header incomplet (%d)\n", hc);
            err = 1;
        }
    }

    // Affiche un petit résumé
    if (!err)
    {
        printf("NO=%s\nSO=%s\nWE=%s\nEA=%s\n",
               cfg.no, cfg.so, cfg.we, cfg.ea);
        if (cfg.floor_rgb.is_set)
            printf("F=%d,%d,%d\n", cfg.floor_rgb.r, cfg.floor_rgb.g, cfg.floor_rgb.b);
        if (cfg.ceil_rgb.is_set)
            printf("C=%d,%d,%d\n", cfg.ceil_rgb.r, cfg.ceil_rgb.g, cfg.ceil_rgb.b);
        printf("Map (%zu lines):\n", cfg.map_h);
        for (size_t i = 0; i < cfg.map_h; i++)
            printf("%s\n", cfg.map_lines[i]);
    }

    cfg_free(&cfg);
    return err ? 1 : 0;
}
