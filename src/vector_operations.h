/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:53:11 by juportie          #+#    #+#             */
/*   Updated: 2025/09/18 14:47:13 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_OPERATIONS_H
# define VECTOR_OPERATIONS_H

// Include t_point type
# include "rendering.h"

t_point	add_vec(t_point vec1, t_point vec2);
t_point	sub_vec(t_point vec1, t_point vec2);
t_point calc_dist(t_point vec1, t_point vec2);
int		is_inferior(t_point a, t_point b);

#endif
