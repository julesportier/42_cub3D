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

static char	*skip_ws(const char *str)
{
	if (!str)
		return (NULL);
    while (*str == ' ' || *str == '\t' || *str == '\r')
		str ++;
    return (str);
}

static char	*dup_range(const char *start, const char *end)
{
	size_t	len;
	char	*str;

	len  = (size_t)(end - start);
    str = (char*)malloc(len + 1);
	if (!str)
		return (NULL);
	if (len)
		ft_memcpy(str, start, len);
	str[len] = '\0';
    return (str);
}

static bool	parse_u8(const char *str, const char **end_out, int *out)
{
	char	*end;
	long	v;
	if (*str < '0' || *str > '9')
		return (false);
	v = strtol(str, &end, 10);//libft!
	if (end == str || v < 0 || v > 255)
		return (false);
	*out = (int)v;
	if (end_out)
		*end_out = end;
	return (true);
}

static bool	parse_rgb_triplet(const char *p, const char **end_after, int *r, int *g, int *b)
{
    p = skip_ws(p);
    if (!parse_u8(p, &p, r))
		return (false);
    p = skip_ws(p);
    if (*p != ',')
		return (false);
    p++;
	p = skip_ws(p);
    if (!parse_u8(p, &p, g))
		return (false);
    p = skip_ws(p);
    if (*p != ',')
		return (false);
    p++;
	p = skip_ws(p);
    if (!parse_u8(p, &p, b))
		return (false);
    p = skip_ws(p);
    if (end_after)
		*end_after = p;
    return (true);
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

