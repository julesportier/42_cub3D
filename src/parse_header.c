#include "parsing.h"

const char *skip_ws(const char *str)
{
	while (*str == ' ' || *str == '\t' || *str == '\r')
		str ++;
	return (str);
}

char *dup_range(const char *start, const char *end)
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

bool parse_colors(const char *str, int *result)
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
bool read_component(const char **pp, int *out)
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
bool consume_comma(const char **pp)
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

bool parse_triplet(const char *str, t_rgb *dst, const char **end_after)
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
bool trim_range(const char *rest, const char **start, const char **end)
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
bool path_has_xpm_suffix(const char *start, const char *end)
{
	size_t len;

	len = (size_t)(end - start);
	return (len >= 4) && (ft_strncmp(end - 4, ".xpm", 4) == 0);
}

char **cfg_slot_for_id(t_config *cfg, t_id id)
{
	if (id==ID_NO) return (&cfg->no);
	if (id==ID_SO) return (&cfg->so);
	if (id==ID_WE) return (&cfg->we);
	if (id==ID_EA) return (&cfg->ea);
	return (NULL);
}

t_id parse_id_at_start(const char **pline)
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

bool handle_texture_entry(t_id id, const char *rest, t_config *cfg, t_perr *perr)
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

bool handle_rgb_entry(t_id id, const char *rest, t_config *cfg, t_perr *perr)
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

const char *perr_str(t_perr e)
{
    if (e == PERR_OK)       return "OK";
    else if (e == PERR_ARGC) return "ARGC";
    else if (e == PERR_EMPTY)return "EMPTY";
    else if (e == PERR_EXT)  return "EXT";
    else if (e == PERR_DIR)  return "DIR";
    else if (e == PERR_OPEN) return "OPEN";
    else if (e == PERR_READ) return "READ";
    else if (e == PERR_ALLOC)return "ALLOC";
    else if (e == PERR_EL_DUP)  return "EL_DUP";
    else if (e == PERR_EL_MISS) return "EL_MISS";
    else if (e == PERR_ID_BAD)  return "ID_BAD";
    else if (e == PERR_RGB_BAD) return "RGB_BAD";
    else if (e == PERR_PATH_MISS) return "PATH_BAD/MISS";
    return "UNKNOWN";
}

void cfg_init(t_config *c)
{
	ft_memset(c, 0, sizeof(*c));
}

bool rgb_is_set(const t_rgb *c)
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
