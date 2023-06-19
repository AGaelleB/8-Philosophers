/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 15:27:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/01/12 12:15:00 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
%x Affiche un nombre en hexadécimal (base 16) avec des lettres min et MAJ
base hexa = 0123456789abcdef
*/

#include "../../includes/ft_printf.h"

int	ft_hexa_usize(unsigned int n)
{
	int	len;

	len = 0;
	while (n != 0)
	{
		n = n / 16;
		len++;
	}
	return (len);
}

void	ft_puthexa_base(unsigned int n, char format)
{
	if (n >= 16)
	{
		ft_puthexa_base(n / 16, format);
		ft_puthexa_base(n % 16, format);
	}
	else if (n <= 9)
		ft_print_c(n + '0');
	else
	{
		if (format == 'x')
			ft_print_c(n - 10 + 'a');
		if (format == 'X')
			ft_print_c(n - 10 + 'A');
	}
}

int	ft_print_x(unsigned int n, char format)
{
	if (n == 0)
		return (write(1, "0", 1));
	else
		ft_puthexa_base(n, format);
	return (ft_hexa_usize(n));
}
