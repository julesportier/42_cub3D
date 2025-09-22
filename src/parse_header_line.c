#include "parsing.h"

static bool parse_colors(const char *str, int *result)
{
    char *end;
    long v = strtol(str, &end, 10);
    fprintf(stderr, "DBG: parse_colors('%s') -> end='%c' (0x%02X) value=%ld\n",
            str ? str : "(null)",
            end && *end ? *end : '0',
            end && *end ? (unsigned char)*end : 0,
            v);
    if (end == str || *end != '\0' || v < 0 || v > 255) return false;
    *result = (int)v;
    return true;
}

static t_id check_id_token(const char *id_tok)
{
    size_t len;

    if (!id_tok || !*id_tok) return ID_NONE;
    len = ft_strlen(id_tok);
    if (len > 2) return ID_UNKNOWN;
    if (len == 2) {
        if (!ft_strncmp(id_tok, "NO", 2)) return ID_NO;
        if (!ft_strncmp(id_tok, "SO", 2)) return ID_SO;
        if (!ft_strncmp(id_tok, "WE", 2)) return ID_WE;
        if (!ft_strncmp(id_tok, "EA", 2)) return ID_EA;
        return ID_UNKNOWN;
    } else { // len == 1
        if (id_tok[0] == 'F') return ID_F;
        if (id_tok[0] == 'C') return ID_C;
        return ID_UNKNOWN;
    }
}

bool parse_header_line(const char *line, t_config *cfg, t_perr *perr)
{
    char **tok = NULL;
    t_id   id;

    if (perr) *perr = PERR_OK;
    if (!line) return false;

    fprintf(stderr, "DBG parse_header_line: enter line='%s'\n", line);

    tok = ft_split_multi(line, " \t\r");
    if (!tok) { if (perr) *perr = PERR_ALLOC; return false; }

    fprintf(stderr, "DBG: tok[0]=%s tok[1]=%s tok[2]=%s\n",
        tok[0]?tok[0]:"(null)", tok[1]?tok[1]:"(null)", tok[2]?tok[2]:"(null)");
    if (!tok[0]) { fprintf(stderr, "DBG: tok[0]=(null) tok[1]=(null) tok[2]=(null)\n"); free_tokens(tok); return false; }
	fprintf(stderr, "DBG: tok[0]=%s tok[1]=%s tok[2]=%s\n",
            tok[0] ? tok[0] : "(null)",
            tok[1] ? tok[1] : "(null)",
            tok[2] ? tok[2] : "(null)");
    id = check_id_token(tok[0]);
    if (id == ID_UNKNOWN) { free_tokens(tok); if (perr) *perr = PERR_ID_BAD; return false; }
    if (id == ID_NONE)    { free_tokens(tok); return false; }

    // --- textures ---
    if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA)
    {
        if (!tok[1] || tok[2]) { free_tokens(tok); if (perr) *perr = PERR_PATH_MISS; return false; }
        char *path = ft_strdup(tok[1]);
        if (!path) { free_tokens(tok); if (perr) *perr = PERR_ALLOC; return false; }

        if ((id==ID_NO && cfg->no) || (id==ID_SO && cfg->so) ||
            (id==ID_WE && cfg->we) || (id==ID_EA && cfg->ea)) {
            if (perr) *perr = PERR_EL_DUP; free(path); free_tokens(tok); return false;
        }
        if      (id==ID_NO) cfg->no = path;
        else if (id==ID_SO) cfg->so = path;
        else if (id==ID_WE) cfg->we = path;
        else                cfg->ea = path;

        free_tokens(tok);
        if (perr) *perr = PERR_OK;
        return true;
    }

    // --- couleurs F/C ---
    if (!tok[1]) { free_tokens(tok); if (perr) *perr = PERR_RGB_BAD; return false; }

    // repartir de la chaîne brute après l'ID, puis splitter avec la virgule aussi
    const char *p = line;
    while (*p==' ' || *p=='\t' || *p=='\r') p++;
    if (*p=='F' || *p=='C') p++;
    while (*p==' ' || *p=='\t' || *p=='\r') p++;

    char **tok_rgb = ft_split_multi(p, " ,\t\r");  // <- prend en compte la virgule
    free_tokens(tok); // on peut libérer le 1er split

    fprintf(stderr, "DBG: rgb = '%s' | '%s' | '%s' | extra=%s\n",
        tok_rgb && tok_rgb[0] ? tok_rgb[0] : "(null)",
        tok_rgb && tok_rgb[1] ? tok_rgb[1] : "(null)",
        tok_rgb && tok_rgb[2] ? tok_rgb[2] : "(null)",
        tok_rgb && tok_rgb[3] ? tok_rgb[3] : "(null)");

    if (!tok_rgb || !tok_rgb[0] || !tok_rgb[1] || !tok_rgb[2] || tok_rgb[3]) {
        free_tokens(tok_rgb);
        if (perr) *perr = PERR_RGB_BAD;
        return false;
    }

    int r,g,b;
    if (!parse_colors(tok_rgb[0], &r) ||
        !parse_colors(tok_rgb[1], &g) ||
        !parse_colors(tok_rgb[2], &b)) {
        free_tokens(tok_rgb);
        if (perr) *perr = PERR_RGB_BAD;
        return false;
    }

    if (id == ID_F) {
        if (cfg->floor_rgb.is_set) { if (perr) *perr = PERR_EL_DUP; free_tokens(tok_rgb); return false; }
        cfg->floor_rgb.r = r; cfg->floor_rgb.g = g; cfg->floor_rgb.b = b; cfg->floor_rgb.is_set = true;
        free_tokens(tok_rgb);
        if (perr) *perr = PERR_OK;
        return true;
    } else { // ID_C
        if (cfg->ceil_rgb.is_set) { if (perr) *perr = PERR_EL_DUP; free_tokens(tok_rgb); return false; }
        cfg->ceil_rgb.r = r; cfg->ceil_rgb.g = g; cfg->ceil_rgb.b = b; cfg->ceil_rgb.is_set = true;
        free_tokens(tok_rgb);
        if (perr) *perr = PERR_OK;
        return true;
    }
}

static bool rgb_is_set(const t_rgb *c)
{
    return (c->is_set);
}

void dump_header(const t_config *c, const char *where)
{
    fprintf(stderr, "[%s] NO=%s SO=%s WE=%s EA=%s | F.set=%d C.set=%d\n",
        where,
        c->no ? c->no : "(null)",
        c->so ? c->so : "(null)",
        c->we ? c->we : "(null)",
        c->ea ? c->ea : "(null)",
        c->floor_rgb.is_set, c->ceil_rgb.is_set);
}

t_perr header_complete(const t_config *cfg)
{
    if (!cfg->no || !cfg->so || !cfg->we || !cfg->ea) {
        return PERR_EL_MISS; // au moins une texture manquante
    }
    if (!rgb_is_set(&cfg->floor_rgb) || !rgb_is_set(&cfg->ceil_rgb))
    {
		return PERR_EL_MISS; // couleur manquante
    }
    return PERR_OK;
}
