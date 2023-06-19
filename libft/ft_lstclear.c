/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:35:50 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/09 10:26:58 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Supprime et libère la mémoire de l’élément passé en
paramètre, et de tous les éléments qui suivent, à
l’aide de ’del’ et de free(3)
Enfin, le pointeur initial doit être mis à NULL.
*/

#include "../includes/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;

	if (lst == 0 || *lst == 0 || del == 0)
		return ;
	temp = *lst;
	while (*lst)
	{
		*lst = temp -> next;
		ft_lstdelone(temp, del);
		temp = *lst;
	}
}
