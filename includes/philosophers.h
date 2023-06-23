/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:42 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/23 12:53:12 by abonnefo         ###   ########.fr       */
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
# include <pthread.h>

# define RESET "\033[0m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

typedef	struct			s_philo
{
	int					philo_id;
	int					nb_time_eat;
	int					left_fork_id;
	int					right_fork_id;
	long long			time_last_eat;
	pthread_mutex_t		mutex; // permet d init des mutex 
	pthread_t			thread_philo; //creation de mes threads
}						t_philo;

typedef struct			s_init
{
	int					nb_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_must_eat;
	t_philo				*philo;
	
}						t_init;

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

/********************************* philo.c ************************************/

/********************************* utils.c ************************************/
int	write_error(char *str);
void	ft_free_tab(char **tab);

/******************************* libft_philo.c ********************************/
int	ft_atoi_philo(char *str);

/********************************** init.c ************************************/
t_init	*init_mutex(t_init *data);
t_init	*init_philo(t_init *data);
t_init	*init_recup_data(t_init *data, int ac, char **av);

#endif