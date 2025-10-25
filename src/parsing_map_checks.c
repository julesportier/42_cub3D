/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:43:22 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/25 17:51:14 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_bool row_border_ok(const t_map *m, int row_index)
{
    int  col;
    char c;

    col = 0;
    while (col < m->columns)
    {
        c = m->grid[row_index][col];
        if (c == '0')
            return (false);
        col++;
    }
    return (true);
}

static t_bool column_border_ok(const t_map *m, int column_index)
{
    int  row;
    char c;

    row = 0;
    while (row < m->rows)
    {
        c = m->grid[row][column_index];
        if (c == '0')
            return (false);
        row++;
    }
    return (true);
}

t_bool map_quick_border_check(const t_map *m)
{
    if (!m || !m->grid)
        return (false);
    if (m->rows <= 0 || m->columns <= 0)
        return (false);
    if (!row_border_ok(m, 0))
        return (false);
    if (!row_border_ok(m, m->rows - 1))
        return (false);
    if (!column_border_ok(m, 0))
        return (false);
    if (!column_border_ok(m, m->columns - 1))
        return (false);
    return (true);
}

static t_bool zero_cell_neighbors_ok(const t_map *m, int row, int col)
{
    if (m->grid[row][col] != '0')
        return true;
    if (row > 0 && m->grid[row - 1][col] == ' ')
        return false;
    if (row + 1 < m->rows && m->grid[row + 1][col] == ' ')
        return false;
    if (col > 0 && m->grid[row][col - 1] == ' ')
        return false;
    if (col + 1 < m->columns && m->grid[row][col + 1] == ' ')
        return false;
    return true;
}

t_bool map_neighbors_ok(const t_map *m)
{
    int row_index;
    int column_index;

    if (!m || !m->grid)
        return false;
    row_index = 0;
    while (row_index < m->rows)
    {
        column_index = 0;
        while (column_index < m->columns)
        {
            if (!zero_cell_neighbors_ok(m, row_index, column_index))
                return false;
            column_index++;
        }
        row_index++;
    }
    return true;
}
