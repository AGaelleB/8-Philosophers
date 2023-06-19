/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:54:21 by abonnefo          #+#    #+#             */
/*   Updated: 2023/02/03 11:57:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*Cette fonction alloue un bloc de mémoire en initialisant tous ces octets 
à la valeur 0. Bien que relativement proche de la fonction malloc, deux aspects 
les différencient :

1 - L'initialisation : calloc met tous les octets du bloc à la valeur 0 
alors que malloc ne modifie pas la zone de mémoire.

2 - Les paramètres d'appels : calloc requière deux paramètres 
(le nombre d'éléments consécutifs à allouer et la taille d'un élément) 
alors que malloc attend la taille totale du bloc à allouer.
*/

#include "../includes/libft.h"
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	char	*dest;

	if (nmemb >= SIZE_MAX || size >= SIZE_MAX)
		return (NULL);
	total = nmemb * size;
	dest = malloc(total);
	if (dest == 0)
		return (NULL);
	ft_bzero(dest, total);
	return (dest);
}
