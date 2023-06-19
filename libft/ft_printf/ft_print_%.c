/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_%.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 16:12:14 by abonnefo          #+#    #+#             */
/*   Updated: 2023/01/30 14:45:48 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//%% Affiche un signe pourcentage.

#include "../../includes/ft_printf.h"

int	ft_print_percent(void)
{
	return (write(1, "%", 1));
}
