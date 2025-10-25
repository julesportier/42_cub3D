/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:57:47 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/25 18:24:16 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/*static t_bool parse_colors(const char *str, int *result)
{
	char *end;
	long v;

	if (!str || !result)
		return (false);
	v = strtol(str, &end, 10);
	if (end == str)
		return (false);
	while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')
		end++;
	if (*end != '\0')
		return (false);
	if (v < 0 || v > 255)
		return (false);
	*result = (int)v;
	return (true);
}*/

t_bool parse_colors(const char *str, int *result)
{
    const char *p;
    long v;
    int any;

    if (!str || !result)
        return false;
    p = str;
    while (*p==' '||*p=='\t'||*p=='\r'||*p=='\n') p++;   /* espaces avant */
    if (*p=='+') p++;                                    /* signe + optionnel */
    if (*p=='-' || *p=='\0') return false;              /* pas de négatif / vide */

    v = 0; any = 0;
    while (*p>='0' && *p<='9') {
        v = v*10 + (*p - '0');
        if (v > 255) return false;                      /* borne dure */
        any = 1; p++;
    }
    while (*p==' '||*p=='\t'||*p=='\r'||*p=='\n') p++;  /* espaces après */
    if (*p != '\0' || !any) return false;               /* rien d’autre permis */

    *result = (int)v;
    return true;
}

static t_bool read_component(const char **pp, int *out)
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

static t_bool consume_comma(const char **pp)
{
	const char *p = *pp;

	p = skip_ws(p);
	if (*p != ',')
		return (false);
	p++;
	p = skip_ws(p);
	*pp = p;
	return (true);
}

static void assign_rgb_component(t_rgb *dst, int index, int val)
{
    if (index == 0)
        dst->r = val;
    else if (index == 1)
        dst->g = val;
    else
        dst->b = val;
}

static t_bool finalize_triplet_parse(const char *p, const char **end_after)
{
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
        ++p;
    if (end_after)
        *end_after = p;
    if (*p != '\0')
        return false;
    return true;
}

t_bool parse_triplet(const char *str, t_rgb *dst, const char **end_after)
{
    const char *p;
    int i;
    int val;

    if (!str || !dst)
        return false;
    p = str;
    i = 0;
    while (i < 3)
    {
        if (!read_component(&p, &val))
            return false;
        assign_rgb_component(dst, i, val);
        if (i < 2)
        {
            if (!consume_comma(&p))
                return false;
        }
        i++;
    }
    if (!finalize_triplet_parse(p, end_after))
        return false;
    dst->is_set = true;
    return true;
}
