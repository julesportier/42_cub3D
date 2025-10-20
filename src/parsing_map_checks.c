#include "parsing.h"

t_bool map_quick_border_check(const t_map *m)
{
	int row_index;
	int column_index;
	char c;

	row_index = 0;
	column_index = 0;
	if (!m || !m->grid)
		return (false);
	if (m->rows <= 0 || m->columns <= 0)
		return (false);
	// haut et bas
	while (column_index < m->columns)
	{
		c = m->grid[0][column_index]; /* haut */
		if (c == '0')
			return (false);
		c = m->grid[m->rows - 1][column_index]; /* bas */
		if (c == '0')
			return (false);
		column_index++;
	}
	// gauche et droite
	row_index = 0;
	while (row_index < m->rows)
	{
		c = m->grid[row_index][0]; /* gauche */
		if (c == '0')
			return (false);
		c = m->grid[row_index][m->columns - 1]; /* droite */
		if (c == '0')
			return (false);
		row_index++;
	}
	return (true);
}

t_bool map_neighbors_ok(const t_map *m)
{
	int row_index;
	int column_index;
	char c;

	if (!m || !m->grid)
		return (false);
	row_index = 0;
	while (row_index < m->rows)
	{
		column_index = 0;
		while (column_index < m->columns)
		{
			c = m->grid[row_index][column_index];
			if (c == '0')
			{
				if (row_index > 0 && m->grid[row_index - 1][column_index] == ' ')
					return (false);
				if (row_index + 1 < m->rows && m->grid[row_index + 1][column_index] == ' ')
					return (false);
				if (column_index > 0 && m->grid[row_index][column_index - 1] == ' ')
					return (false);
				if (column_index + 1 < m->columns && m->grid[row_index][column_index + 1] == ' ')
					return (false);
			}
			column_index++;
		}
		row_index++;
	}
	return (true);
}
