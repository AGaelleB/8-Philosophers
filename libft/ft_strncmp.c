/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:53:08 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Cette fonction permet de comparer deux chaînes de caractères 
et de savoir si la première est inférieure, égale ou supérieure à la seconde.
*/

#include "../includes/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((i < n) && (s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0'))
		i++;
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}
