/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:34:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/13 12:29:20 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Compte le nombre d’éléments de la liste

#include "../includes/libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst -> next;
		i++;
	}
	return (i);
}
