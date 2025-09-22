#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>     // realloc, free, NULL
#include "parsing.h"

void cfg_init(t_config *cfg)
{
    ft_memset(cfg, 0, sizeof(*cfg));
    cfg->floor_rgb.r = - 1;
    cfg->floor_rgb.g = - 1;
    cfg->floor_rgb.b = - 1;
    cfg->ceil_rgb.r  = - 1;
    cfg->ceil_rgb.g  = - 1;
    cfg->ceil_rgb.b  = - 1;
    cfg->floor_rgb.is_set = false;
    cfg->ceil_rgb.is_set = false;
}

static int push_map_line(t_config *c, const char *line)
{
    if (c->map_h + 1 > c->map_cap) {
        size_t newcap = c->map_cap ? c->map_cap * 2 : 16;
        char **tmp = realloc(c->map_lines, newcap * sizeof *tmp);
        if (!tmp) return -1;
        c->map_lines = tmp;
        c->map_cap   = newcap;
    }
    c->map_lines[c->map_h] = ft_strdup(line);
    if (!c->map_lines[c->map_h]) return -1;
    c->map_h++;
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s file.cub\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
		perror("open");
		return 1;
	}

    t_config cfg; cfg_init(&cfg);

    t_perr perr = PERR_OK;
    char  *line = NULL;
    int    header_done = 0;
    int    err = 0;

    while ((line = get_next_line(fd)) != NULL)
    {
		fprintf(stderr, "DBG main: got line ptr=%p\n", (void*)line);
		size_t n = ft_strlen(line);
		while (n && (line[n-1] == '\n' || line[n-1] == '\r')) line[--n] = '\0';
		fprintf(stderr, "DBG main: trimmed line='%s'\n", line);
		// skip lignes blanches avant parse
		const char *q = line;
		while (*q == ' ' || *q == '\t' || *q == '\r') q++;
		if (*q == '\0') {        // ligne vide
		free(line);
		line = NULL;
		continue;}
		if (!header_done)
		{
			bool handled = parse_header_line(line, &cfg, &perr);
			fprintf(stderr, "DBG after parse_header_line: handled=%d perr=%d\n", handled, perr);

			if (handled)
			{
				fprintf(stderr, "DBG main: about to free(line)\n");
				free(line);
				line = NULL; // 🔒 évite toute réutilisation
				fprintf(stderr, "DBG main: call header_complete\n");
				t_perr hc = header_complete(&cfg);
				fprintf(stderr, "DBG main: header_complete -> %d\n", hc);
				if (hc == PERR_OK) header_done = 1;
				fprintf(stderr, "DBG main: continue loop\n");
				continue;
			}
			else
			{
				if (perr != PERR_OK)
				{
					fprintf(stderr, "Error\nheader: %d\n", perr);
					free(line);
					line = NULL;
					err = 1;
					break;
				}
				const char *p = line;
				while (*p==' '||*p=='\t') p++;
				if (*p == '\0')
				{
					free(line);
					line = NULL;
					continue;
				}
				header_done = 1; // début de la map
            // pas de continue; on garde cette 1ère ligne de map
			}
		}

		// phase map
		if (push_map_line(&cfg, line) != 0)
		{
			fprintf(stderr, "OOM pushing map line\n");
			free(line);
			line = NULL;
			err = 1;
			break;
		}
		free(line);
		line = NULL;
	}
	fprintf(stderr, "DBG main: loop ended (line=%p)\n", (void*)line);
	close(fd);

    // Vérif finale du header si on n'a pas déjà eu d'erreur
    if (!err) {
        t_perr hc = header_complete(&cfg);
        if (hc != PERR_OK) {
            fprintf(stderr, "Header incomplet (%d)\n", hc);
            err = 1;
        }
    }

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
