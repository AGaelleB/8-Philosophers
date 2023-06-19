/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:15:00 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Alloue (avec malloc(3)) et retourne une copie de la chaîne ’s1’, sans les 
caractères spécifiés dans ’set’ au début et à la fin de la chaîne de caractères.
*/

#include "../includes/libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	front;
	size_t	rear;
	char	*str;

	if (s1 == 0 || set == 0)
		return (NULL);
	str = 0;
	front = 0;
	rear = ft_strlen(s1);
	while (s1[front] && ft_strchr(set, s1[front]))
		front++;
	while (s1[rear - 1] && ft_strchr(set, s1[rear - 1]) && rear > front)
		rear--;
	str = (char *)malloc(sizeof(char) * (rear - front + 1));
	if (str == 0)
		return (NULL);
	ft_strlcpy(str, &s1[front], rear - front + 1);
	return (str);
}
