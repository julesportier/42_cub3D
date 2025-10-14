/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_v2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:25:27 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/08 16:28:48 by vakozhev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_header.h"

static const char	*skip_ws(const char *str)
{
	//if (!str)
		//return (NULL);
    while (*str == ' ' || *str == '\t' || *str == '\r')
		str ++;
    return (str);
}

static char	*dup_range(const char *start, const char *end) //calcule nombre d’octets entre start inclus et end exclus
{
	size_t	len;
	char	*str;
	
	if (!start || !end)
		return (NULL);
	if (end < start)
		return (NULL);
	len  = (size_t)(end - start);
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
    // tolérer espaces de tête
    //while (*str==' ' || *str=='\t' || *str=='\r')
	//	str++;
    // premier non-blanc doit être un chiffre (refuse + et -)
   // if (*str < '0' || *str > '9')
	//	return (false);
    v = strtol(str, &end, 10);//LIBFT!
    if (end == str)
		return (false); // rien lu
    // tolérer espaces de fin
    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') //?
		end++;
    if (*end != '\0')
		return (false);
    if (v < 0 || v > 255)
		return (false);
    *result = (int)v;
    return (true);
}

/*static bool parse_triplet(const char *str, t_rgb *dst, const char **end_after)
{
    const char *p;
    char        tok[4];   // "255" + '\0'
    int         val;
    int         i;
    int         nd;

    if (!str || !dst)
        return (false);
    p = str;
    i = 0;
    while (i < 3)
    {
        nd = 0;
        p = skip_ws(p); // consommer espaces éventuels (début ou après virgule)
        // lire 1..3 chiffres (pas de signe)
        if (*p < '0' || *p > '9')
            return (false);
        while (nd < 3 && p[nd] >= '0' && p[nd] <= '9')
        {
            tok[nd] = p[nd];
            nd++;
        }
        tok[nd] = '\0';
        // Valide/convertit via TA parse_colors (exige chaîne pure et 0..255)
        if (!parse_colors(tok, &val))
            return (false);
        // avance après les chiffres et tolère espaces avant la virgule
        p += nd;
        p = skip_ws(p);
        // virgule obligatoire après R et G, espaces possibles des deux côtés
        if (i < 2)
        {
            if (*p != ',')
                return (false);
            p++;               // consomme la virgule
            p = skip_ws(p);    // espaces après la virgule
        }
        // stocke
        if (i == 0)
			dst->r = val;
        else if (i == 1)
			dst->g = val;
        else
			dst->b = val;
        i++; // incrémenter l’index composante
    }
    // tolérer espaces/retours finaux, mais rien d’autre
    while (*p==' ' || *p=='\t' || *p=='\r' || *p=='\n')
        ++p;
    if (end_after)
        *end_after = p;
    if (*p != '\0')
        return (false);

    dst->is_set = true;
    return (true);
}*/

// Lit une composante 0..255 sans signe, espaces optionnels autour.
// Avance *pp jusqu'après la composante (et espaces suivants).
static bool read_component(const char **pp, int *out)
{
    const char *p = *pp;
    char tok[4];
    int nd = 0;

    p = skip_ws(p);                           // espaces avant
    if (*p < '0' || *p > '9')
		return (false);   // doit commencer par un chiffre
    while (nd < 3 && p[nd] >= '0' && p[nd] <= '9')
	{
        tok[nd] = p[nd];
		nd ++;
	}
    tok[nd] = '\0';
    if (nd == 0)
		return (false);
    // Valide via la fonction (exige chaine pure et 0..255)
    if (!parse_colors(tok, out))
		return (false);
    p += nd;
    p = skip_ws(p);                           // espaces après
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
    p++;                                      // virgule
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
        if (i == 0)
			dst->r = val;
        else if (i == 1)
			dst->g = val;
        else
			dst->b = val;
        if (i < 2)
        {                         // entre R-G et G-B: virgule requise
            if (!consume_comma(&p))
				return (false);
        }
        i++;
    }
    // Tolérer espaces/retours finaux, rien d’autre
    while (*p==' ' || *p=='\t' || *p=='\r' || *p=='\n') ++p;
    if (end_after) *end_after = p;
    if (*p != '\0')
		return (false);
    dst->is_set = true;
    return (true);
}

// Trim début+fin sur une sous-chaîne [start,end), ensuite alloc avec dup_range
/*static bool trim_range(const char *rest, const char **start, const char **end)
{
    const char *p = skip_ws(rest);
    if (*p == '\0')
		return false;
    const char *q = p + strlen(p);
    while (q > p && (q[-1]==' ' || q[-1]=='\t' || q[-1]=='\r'))
		q--;
    if (q == p)
		return false;
    *start = p;
	*end = q;
    return true;
}*/

static bool	trim_range(const char *rest, const char **start, const char **end)
{
	const char	*p;
	size_t		len;
//	size_t		i;
	char		ch;

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
	*start = p;        //sous-chaîne commence à p
	*end = p + len;
	return (true);
}

// vérifie suffixe ".xpm"
static bool	path_has_xpm_suffix(const char *start, const char *end)
{
    size_t	len;
	
	len = (size_t)(end - start);
    return (len >= 4) && (ft_strncmp(end - 4, ".xpm", 4) == 0);
}

static char	**cfg_slot_for_id(t_config *cfg, t_id id)
{
    if (id==ID_NO)
		return (&cfg->no);
    if (id==ID_SO)
		return (&cfg->so);
    if (id==ID_WE)
		return (&cfg->we);
    if (id==ID_EA)
		return (&cfg->ea);
    return (NULL);
}

static t_id	parse_id_at_start(const char **pline)
{
    const char	*p;
	
	p = skip_ws(*pline);
    t_id id = ID_UNKNOWN;

    if (*p == '\0')
	{
		*pline = p;
		return ID_NONE;
	}
    if (ft_strncmp(p, "NO", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{
		id = ID_NO;
		p += 2;
	}
    else if (ft_strncmp(p, "SO", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{
		id = ID_SO;
		p += 2;
	}
    else if (ft_strncmp(p, "WE", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{
		id = ID_WE;
		p += 2;
	}
    else if (ft_strncmp(p, "EA", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
	{
		id = ID_EA;
		p += 2;
	}
    else if (ft_strncmp(p, "F",  1) == 0 && (p[1]=='\0' || p[1]==' ' || p[1]=='\t' || p[1]=='\r'))
	{
		id = ID_F;
		p += 1;
	}
    else if (ft_strncmp(p, "C",  1) == 0 && (p[1]=='\0' || p[1]==' ' || p[1]=='\t' || p[1]=='\r'))
	{
		id = ID_C;
		p += 1;
	}
    else if (*p == ' ' || *p == '\t' || *p == '\r')
		id = ID_NONE;
    else
		id = ID_UNKNOWN;
    *pline = p;
    return (id);
}

static bool	handle_texture_entry(t_id id, const char *rest, t_config *cfg, t_perr *perr)
{
    const char	*start;
	const char	*end;
	char	**slot;
	char	*path;

    if (!trim_range(rest, &start, &end))
	{
		if (perr)
			*perr = PERR_PATH_MISS;
		return (false);
	}
    if (!path_has_xpm_suffix(start, end))
	{
		if (perr)
			*perr = PERR_PATH_MISS;
		return (false);
	}
    slot = cfg_slot_for_id(cfg, id);
    if (!slot)
	{
		if (perr)
			*perr = PERR_ID_BAD;
		return (false);
	}
    if (*slot)
	{
		if (perr)
			*perr = PERR_EL_DUP;
		return (false);
	}
    path = dup_range(start, end);
    if (!path)
	{
		if (perr)
			*perr = PERR_ALLOC;
		return (false);
	}
    *slot = path;
    if (perr)
		*perr = PERR_OK;
    return (true);
}

/*static bool	handle_rgb_entry(t_id id, const char *rest, t_config *cfg, t_perr *perr)
{
    int r;
	int	g;
	int	b;
	const char *after;
	
	after = NULL;
    if (!parse_rgb_triplet(rest, &after, &r, &g, &b))
	{
		if (perr)
			*perr = PERR_RGB_BAD;
		return false;
	}
    if (*skip_ws(after) != '\0')
	{
		if (perr)
			*perr = PERR_RGB_BAD;
		return false;
	}
    if (id == ID_F)
	{
        if (cfg->floor_rgb.is_set)
		{
			if (perr)
				*perr = PERR_EL_DUP;
			return false;
		}
        cfg->floor_rgb = (t_rgb){ .r=r, .g=g, .b=b, .is_set=true };
    }
	else
	{
        if (cfg->ceil_rgb.is_set)
		{
			if (perr)
				*perr = PERR_EL_DUP;
			return false;
		}
        cfg->ceil_rgb = (t_rgb){ .r=r, .g=g, .b=b, .is_set=true};
    }
    if (perr)
		*perr = PERR_OK;
    return true;
}*/

static bool	handle_rgb_entry(t_id id, const char *rest, t_config *cfg, t_perr *perr)
{
	t_rgb		tmp;
	const char	*after;

	tmp.r = 0;
	tmp.g = 0;
	tmp.b = 0;
	tmp.is_set = false;
	after = NULL;
	/* parse "R,G,B" → remplit tmp et positionne after à la fin */
	if (!parse_triplet(rest, &tmp, &after))
	{
		if (perr)
			*perr = PERR_RGB_BAD;
		return (false);
	}
	/* rien d’autre après (parse_triplet tolère déjà espaces/retours finaux) */
	if (*skip_ws(after) != '\0')
	{
		if (perr)
			*perr = PERR_RGB_BAD;
		return (false);
	}
	if (id == ID_F)
	{
		if (cfg->floor_rgb.is_set)
		{
			if (perr)
				*perr = PERR_EL_DUP;
			return (false);
		}
		cfg->floor_rgb = tmp;
	}
	else
	{
		if (cfg->ceil_rgb.is_set)
		{
			if (perr)
				*perr = PERR_EL_DUP;
			return (false);
		}
		cfg->ceil_rgb = tmp;
	}
	if (perr)
		*perr = PERR_OK;
	return (true);
}

bool	parse_header_line(const char *line, t_config *cfg, t_perr *perr)
{
	const char	*p;
    if (perr)
		*perr = PERR_OK;
    if (!line)
		return (false);
    p = line;
    t_id id = parse_id_at_start(&p);
    if (id == ID_NONE)
		return (false);
    if (id == ID_UNKNOWN)
	{
		if (perr)
			*perr = PERR_ID_BAD;
		return (false);
	}
    if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA)
        return handle_texture_entry(id, p, cfg, perr);
    else
        return handle_rgb_entry(id, p, cfg, perr);
}

static const char *perr_str(t_perr e) {
    switch (e) {
        case PERR_OK:        return "OK";
        case PERR_ALLOC:     return "ALLOC";
        case PERR_ID_BAD:    return "ID_BAD";
        case PERR_PATH_MISS: return "PATH_BAD/MISS";
        case PERR_RGB_BAD:   return "RGB_BAD";
        case PERR_EL_DUP:    return "EL_DUP";
        case PERR_EL_MISS:   return "EL_MISS";
        default:             return "UNKNOWN";
    }
}

static void	cfg_init(t_config *c)
{
    memset(c, 0, sizeof(*c));
}

static void cfg_free(t_config *c)
{
    free(c->no);
    free(c->so);
    free(c->we);
    free(c->ea);
    memset(c, 0, sizeof(*c));
}

static bool rgb_is_set(const t_rgb *c)
{
    return (c->is_set);
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

static void	run_line(const char *line, t_config *cfg)
{
    t_perr perr = PERR_OK;
    bool ok = parse_header_line(line, cfg, &perr);
    printf("line: %-30s  -> %s (%s)\n", line, ok ? "ACCEPTED" : "REJECTED", perr_str(perr));
}

static void	run_from_file(const char *path, t_config *cfg)
{
    FILE *f = fopen(path, "r");
    if (!f)
	{
        perror("fopen");
        return;
    }
    char buf[4096];
    size_t line_no = 0;
    while (fgets(buf, sizeof buf, f))
	{
        line_no++;
        // strip \n
        size_t n = strlen(buf);
        if (n && (buf[n-1] == '\n'))
			buf[n-1] = '\0';
        if (buf[0] == '\0')
			continue; // skip empty
        run_line(buf, cfg);
    }
    fclose(f);
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

int main(int argc, char **argv)
{
    t_config cfg;
    cfg_init(&cfg);

    if (argc > 1) {
        printf("== Testing header from file: %s ==\n", argv[1]);
        run_from_file(argv[1], &cfg);
    } else {
        printf("== Testing with built-in lines ==\n");
        const char *lines[] = {
            // Textures (espaces dans le path / suffixe .xpm requis)
            "NO   ./textures/north.xpm",
            "SO\t./textures/with space south.xpm   ",
            "WE\t./textures/UPPERCASE.XPM",            // devrait être REJECTED (suffixe sensible à la casse)
            "EA   ./textures/east.xpm  ",
            // Couleurs (espaces autour des virgules autorisés)
            "F  12 ,34,   56",
            "C\t255, 0 , 255",
            // Cas d’erreurs
            "C 256,0,0",             // hors borne
            "F  1,2",                // manque une composante
            "F  1  ,  2  ,  3  x",   // trailing garbage
            "XX whatever",           // ID inconnu
            "SO ./textures/dupe.xpm" // doublon SO
        };
        size_t nb = sizeof(lines)/sizeof(lines[0]);
        for (size_t i = 0; i < nb; ++i) run_line(lines[i], &cfg);
    }

    printf("\n-- Final state --\n");
    dump_header(&cfg, "TEST");

    t_perr status = header_complete(&cfg);
    printf("header_complete: %s\n", perr_str(status));

    cfg_free(&cfg);
    return (status == PERR_OK) ? 0 : 1;
}
