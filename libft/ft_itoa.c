/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:16:31 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Alloue (avec malloc(3)) et retourne une chaîne
de caractères représentant l’entier ’n’ reçu en
argument. Les nombres négatifs doivent être gérés.
*/

#include "../includes/libft.h"

static int	size(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*str;
	long int		i;
	unsigned int	nb;

	i = size(n);
	str = malloc(sizeof(char) * i + 1);
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		nb = n * -1;
		str[0] = '-';
	}
	else
		nb = n;
	while (nb > 0)
	{
		i--;
		str[i] = '0' + (nb % 10);
		nb /= 10;
	}
	return (str);
}
