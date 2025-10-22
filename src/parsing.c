
#include "parsing.h"

t_bool is_map_line(const char *str)
{
	int len;
	int i;

	i = 0;
	len = line_len_no_nl(str);
	if (len == 0)
		return (false);
	while (i < len)
	{
		if (!is_map_char((unsigned char)str[i]))
			return (false);
		i ++;
	}
	return (true);
}

t_bool is_blank_or_ws_only(const char *s)
{
	const unsigned char *p = (const unsigned char*)s;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	return (*p == '\0');
}

static t_bool handle_before_map(char *line, t_config *cfg, t_mapbuild *mb, t_perr *err)
{
    if (is_map_line(line))
	{
        mb->started = true;
        if (!mb_push_line(mb, line))
		{
            if (err) *err = PERR_READ;
            return (false);
        }
        return (true);
    }
    if (!is_blank_or_ws_only(line))
	{
        t_perr perr = PERR_OK;
        if (!parse_header_line(line, cfg, &perr))
		{
            if (err) *err = (perr != PERR_OK) ? perr : PERR_ID_BAD;
            return (false);
        }
    }
    return (true);
}

static t_bool handle_in_or_after_map(char *line, t_mapbuild *mb, t_perr *err)
{
    if (mb->ended)
	{
        if (line_len_no_nl(line) > 0 && !is_blank_or_ws_only(line))
		{
            if (err)
				*err = PERR_READ;
            return (false);
        }
        return (true);
    }
    if (is_map_line(line))
	{
        if (!mb_push_line(mb, line))
		{
            if (err)
				*err = PERR_READ;
            return (false);
        }
    }
	else
	{
        mb->ended = true;
    }
    return true;
}

static t_bool	handle_line(char *line, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
    t_perr err;
    t_bool func;

	err = PERR_OK;
    if (!mb->started)
        func = handle_before_map(line, cfg, mb, &err);
	else
        func = handle_in_or_after_map(line, mb, &err);
    if (!func && out_err)
        *out_err = err;
    free(line);
    return (func);
}

static t_bool	read_loop(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
    char	*line;

	line = get_next_line(fd);
    while (line)
	{
        if (!handle_line(line, cfg, mb, out_err))
            return (false);
        line = get_next_line(fd);
    }
    return (true);
}

t_bool	parse_file_fd(int fd, t_config *cfg, t_mapbuild *mb, t_perr *out_err)
{
	if (out_err)
		*out_err = PERR_OK;
	mb_init(mb);
	return (read_loop(fd, cfg, mb, out_err));
}

void cfg_init(t_config *c)
{
	ft_memset(c, 0, sizeof(*c));
}

void mb_init(t_mapbuild *map)
{
	ft_memset(map, 0, sizeof(*map));
	map->player.row = -1;
	map->player.column = -1;
}

t_bool parse_header_line(const char *line, t_config *cfg, t_perr *perr)
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
		return handle_texture(id, p, cfg, perr);
	else
		return handle_rgb(id, p, cfg, perr);
}

static t_perr load_cfg_build_map(int fd, t_config *cfg, t_map *m)
{
    t_perr     perr = PERR_OK;
    t_mapbuild mb;

    cfg_init(cfg);

    if (!parse_file_fd(fd, cfg, &mb, &perr))
        return (perr != PERR_OK) ? perr : PERR_READ;

    perr = header_complete(cfg);
    if (perr != PERR_OK) {
        mb_free(&mb);
        return perr;
    }

    if (!map_build_split(&mb, m)) {
        mb_free(&mb);
        return PERR_ALLOC;
    }
    mb_free(&mb);

    if (!map_quick_border_check(m) || !map_neighbors_ok(m)) {
        map_free_split(m);
        return PERR_READ;
    }
    return PERR_OK;
}

t_perr parsing_load(const char *path, t_parsed *out)
{
    if (!path || !out)
        return PERR_EMPTY;

    int fd = open(path, O_RDONLY);
    if (fd < 0)
        return PERR_OPEN;

    t_config cfg;
    t_map    m;
    t_perr   perr = load_cfg_build_map(fd, &cfg, &m);

    close(fd);

    if (perr != PERR_OK) {
        cfg_free(&cfg);
        return perr;
    }

    out->config = cfg;
    out->map    = m;
    return PERR_OK;
}

