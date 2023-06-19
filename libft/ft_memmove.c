/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:16:28 by gaelle            #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
en fonction de la position de dest par rapport a la src, 
on copie les element de src du debut vers la fin ou vis versa. 
Le but etant de ne pas ecraser les donnees 
*/

#include "../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;

	if (dest == 0 && src == 0)
		return (NULL);
	if ((unsigned char *)dest < (unsigned char *)src)
	{
		i = 0;
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		while (n)
		{
			((unsigned char *)dest)[n - 1] = ((unsigned char *)src)[n - 1];
			n--;
		}
	}
	return (dest);
}
