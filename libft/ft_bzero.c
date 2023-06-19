/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:14:53 by gaelle            #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
Permet de mettre à 0 la zone de mémoire en C, presque meme principe que memset
*/

#include "../includes/libft.h"

void	ft_bzero(void	*s, size_t	n)
{
	ft_memset(s, '\0', n);
}
