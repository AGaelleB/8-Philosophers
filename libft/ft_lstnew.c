/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:33:46 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/10 10:26:08 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Alloue (avec malloc(3)) et renvoie un nouvel
élément. La variable membre ’content’ est
initialisée à l’aide de la valeur du paramètre
’content’. La variable ’next’ est initialisée à
NULL.
*/

#include "../includes/libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*elem;

	elem = malloc(sizeof(t_list));
	if (elem == 0)
		return (NULL);
	elem -> content = content;
	elem -> next = NULL;
	return (elem);
}
