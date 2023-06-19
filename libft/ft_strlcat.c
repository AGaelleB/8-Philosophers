/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:53:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
The strlcpy() and strlcat() functions copy and concatenate strings respectively
*/

#include "../includes/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	dst_size;
	size_t	src_size;

	if (size == 0 && dst == 0)
		return (0);
	dst_size = ft_strlen(dst);
	src_size = ft_strlen(src);
	if (size <= dst_size)
		return (size + src_size);
	i = dst_size;
	j = 0;
	while (src[j] && ((i + j) < (size - 1)))
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst_size + src_size);
}
