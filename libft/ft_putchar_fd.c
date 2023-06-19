/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:13:18 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Écrit le caractère ’c’ sur le descripteur de fichier donné.
*/

#include "../includes/libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}
