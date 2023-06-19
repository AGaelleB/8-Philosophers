/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:17:00 by gaelle            #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Recherche la première occurence d'une valeur typée int, 
//mais interprétée en tant que char, dans un bloc de mémoire.

#include "../includes/libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char	*str;

	str = (char *)s;
	while (n != 0)
	{
		if ((*str) == (char)c)
			return (str);
		str++;
		n--;
	}
	return (NULL);
}
