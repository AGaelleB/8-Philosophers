/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:40:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/22 19:42:49 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return (NULL);
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	write_error(char *str)
{
	int	len;

	len = 0;
	while(str[len])
		len++;
	write(2, "Error: ", 7);
	write(2, str, len);
	write(2, "\n", 1);
	return (0);
}