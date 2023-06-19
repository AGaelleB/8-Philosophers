/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:42 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/19 10:31:31 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <stdint.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>

# include "../includes/ft_printf.h"
# include "../includes/get_next_line.h"
# include "../includes/libft.h"

typedef struct s_cmd
{
	char	*path;
	char	**args;
}	t_cmd;

typedef struct s_data
{
	char			**all_argcs;
	t_cmd			cmd1;
	t_cmd			cmd2;
	struct s_data	*next;
}	t_data;

/********************************* philosophers.c ************************************/

/********************************* utils.c ************************************/

/********************************** path.c ************************************/

#endif