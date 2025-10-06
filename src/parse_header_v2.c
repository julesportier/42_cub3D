/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header_v2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:25:27 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/06 16:58:08 by vakozhev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    while (*str==' ' || *str=='\t' || *str=='\r')
		str++;
    // premier non-blanc doit être un chiffre (refuse + et -)
    if (*str < '0' || *str > '9')
		return (false);
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
        tok[nd++] = p[nd-1];
    tok[nd] = '\0';
    if (nd == 0)
		return (false);
    // Valide via ta fonction (exige chaine pure et 0..255)
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
    if (*p != '\0') return false;

    dst->is_set = true;
    return true;
}

// Trim début+fin sur une sous-chaîne [start,end)
static bool trim_range(const char *rest, const char **start, const char **end)
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
}

// vérifie suffixe ".xpm" (sensible à la casse)
static bool path_has_xpm_suffix(const char *start, const char *end)
{
    size_t len = (size_t)(end - start);
    return (len >= 4) && (strncmp(end - 4, ".xpm", 4) == 0);
}

