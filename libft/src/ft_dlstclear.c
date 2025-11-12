/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstclear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:31:21 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 17:09:01 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstclear(t_dlst *lst, void del(void *))
{
	t_dlst	*node;

	while (lst)
	{
		node = lst;
		lst = node->next;
		del(node->content);
		free(node);
		node = NULL;
	}
}
