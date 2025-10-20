#include "parsing.h"

static t_bool parse_colors(const char *str, int *result)
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

t_bool parse_triplet(const char *str, t_rgb *dst, const char **end_after)
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
		{
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
