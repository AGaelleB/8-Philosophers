/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:14:05 by abonnefo          #+#    #+#             */
/*   Updated: 2023/01/12 12:15:04 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_convert(va_list args, const char format)
{
	int	print;

	print = 0;
	if (format == 'c')
		print = ft_print_c(va_arg(args, int));
	else if (format == 's')
		print = ft_print_s(va_arg(args, char *));
	else if (format == 'p')
		print = ft_print_p(va_arg(args, unsigned long long int));
	else if (format == 'd' || format == 'i')
		print = ft_print_d(va_arg(args, int));
	else if (format == 'u')
		print = ft_print_u(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		print = ft_print_x(va_arg(args, unsigned int), format);
	else if (format == '%')
		print = ft_print_percent();
	return (print);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	int		print;
	va_list	args;

	va_start(args, format);
	i = 0;
	print = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			print += ft_convert(args, format[i + 1]);
			i++;
		}
		else
			print += ft_print_c(format[i]);
		i++;
	}
	return (print);
	va_end (args);
}
