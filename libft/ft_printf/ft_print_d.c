/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 12:06:19 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// %d Affiche un nombre décimal (base 10).

#include "../../includes/ft_printf.h"

int	ft_size(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa_printf(int n)
{
	char			*num;
	long int		len;
	unsigned int	nb;

	len = ft_size(n);
	num = malloc(sizeof(char) * (len + 1));
	if (num == 0)
		return (NULL);
	num[len] = '\0';
	if (n == 0)
		num[0] = '0';
	if (n < 0)
	{
		nb = n * -1;
		num[0] = '-';
	}
	else
		nb = n;
	while (nb > 0)
	{
		len--;
		num[len] = '0' + (nb % 10);
		nb = nb / 10;
	}
	return (num);
}

int	ft_print_d(int n)
{
	char	*num;
	int		len;

	num = ft_itoa_printf(n);
	len = ft_print_s(num);
	free(num);
	return (len);
}
