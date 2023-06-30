/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:42 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/29 09:41:41 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

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
#include <sys/time.h>
#include <stdbool.h>

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
	long long			time_init; // ici car bug dans init
}						t_philo;

typedef struct			s_init
{
	int					nb_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_must_eat;
	t_philo				*philo;
	pthread_mutex_t		*forks; // me permet d utiliser pthread_mutex_lock sans erreurs
	int					end_flag;
	int					all_finished_eating; // verif si tous les repas sont pris
	pthread_mutex_t		eat_count_mutex; // Mutex pour protéger le compteur
	pthread_mutex_t		death_mutex;

}						t_init;

typedef struct	s_data
{
	t_philo	*philo;
	t_init	*init;
}		t_data;

/********************************* philo.c ************************************/

/********************************* utils.c ************************************/
int			write_error(char *str);
void		ft_free_tab(char **tab);
long long	get_time_philo(void);
void		print_action(t_philo *philo, int id, char *str);
void		cleanup_forks(t_init *data);


/******************************* libft_philo.c ********************************/
int	ft_atoi_philo(char *str);

/********************************** init.c ************************************/
t_init		*init_mutex(t_init *data);
t_init		*init_philo(t_init *data);
t_init		*init_recup_data(t_init *data, int ac, char **av);
t_init		*init_forks(t_init *data);
t_init		*init_eat_count_mutex(t_init *data);

/***************************** actions_philos.c ********************************/

void		action_think(t_philo *philo, t_init *data);
void		action_eat(t_philo *philo, t_init *data);
void		action_drop_fork(t_philo *philo, t_init *data);
void		action_grab_fork(t_philo *philo, t_init *data);
void		action_sleep(t_philo *philo, t_init *data);

/***************************** routine.c ********************************/

void		*thread_routine(void *data);
void		run_routine_philo(t_init *data);
int			check_if_philo_died(t_philo *philo, t_init *data);
void		stop_all_if_flag(t_init *init);


#endif
