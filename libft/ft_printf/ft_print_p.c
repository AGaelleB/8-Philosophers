/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 15:08:29 by abonnefo          #+#    #+#             */
/*   Updated: 2023/01/12 12:14:51 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
%p L’argument de pointeur void * doit être affiché en hexadécimal.

%p correspond a la valeur hexadécimal de l'adresse de la variable", 
ex : int a = 0x7ffc8d8685a4%
*/

#include "../../includes/ft_printf.h"

int	ft_size_p(unsigned long long int n)
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

void	ft_put_p_base(unsigned long long int n)
{
	if (n >= 16)
	{
		ft_put_p_base(n / 16);
		ft_put_p_base(n % 16);
	}
	else if (n <= 9)
		ft_print_c(n + '0');
	else
		ft_print_c(n - 10 + 'a');
}

int	ft_print_p(unsigned long long int ptr)
{
	int	print_p;

	print_p = 0;
	if (ptr == 0)
		return (write(1, "(nil)", 5));
	else
	{
		print_p = write(1, "0x", 2);
		ft_put_p_base(ptr);
		print_p += ft_size_p(ptr);
	}
	return (print_p);
}
