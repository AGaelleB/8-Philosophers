/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:54:05 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/16 11:20:07 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Cette fonction permet de transformer une chaîne de caractères, 
représentant une valeur entière, en une valeur numérique de type int. 
Le terme d'atoi est un acronyme signifiant : ASCII to integer.
*/

#include "../includes/libft.h"

long	ft_atoi(const char *nptr)
{
	int		i;
	int		sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (nptr[i] == ' ' || nptr[i] == '\n' || nptr[i] == '\t'
		||nptr[i] == '\v' || nptr[i] == '\f' || nptr[i] == '\r')
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	while (nptr[i] && nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr = (nbr * 10);
		nbr = nbr + (nptr[i] - '0');
		i++;
	}
	return (nbr * sign);
}
