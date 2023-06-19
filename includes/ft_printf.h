/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 09:10:58 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/13 15:00:17 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>

int		ft_printf(const char *format, ...);
int		ft_print_percent(void);
int		ft_print_c(int c);
int		ft_print_d(int n);
int		ft_print_p(unsigned long long int n);
int		ft_print_s(char *str);
int		ft_print_u(unsigned int n);
int		ft_print_x(unsigned int n, char format);

#endif