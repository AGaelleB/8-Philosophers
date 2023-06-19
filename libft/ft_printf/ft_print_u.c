/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 14:27:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 12:06:50 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// %u Affiche un nombre décimal non signé (base 10).

#include "../../includes/ft_printf.h"

int	ft_usize(unsigned int n)
{
	int	len;

	len = 0;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_uitoa_printf(unsigned int n)
{
	char			*num;
	unsigned int	len;

	len = ft_usize(n);
	num = malloc(sizeof(char) * (len + 1));
	if (num == 0)
		return (NULL);
	num[len] = '\0';
	while (n > 0)
	{
		len--;
		num[len] = '0' + (n % 10);
		n = n / 10;
	}
	return (num);
}

int	ft_print_u(unsigned int n)
{
	char			*num;
	unsigned int	len;

	len = 0;
	if (n == 0)
		len = write(1, "0", 1);
	else
	{
		num = ft_uitoa_printf(n);
		len = ft_print_s(num);
		free(num);
	}
	return (len);
}
