/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 18:50:50 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/03 15:04:51 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>

# define INT_MAX 2147483647

# define RESET "\033[0m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

typedef struct s_philo
{
	int					philo_id;
	int					nb_time_eat;
	int					left_fork_id;
	int					right_fork_id;
	long long			time_last_eat;
	long long			time_init;
	pthread_t			thread_philo;
	struct s_data		*data;
}						t_philo;

typedef struct s_init
{
	int					death_printed;

	long long int		nb_of_philo;
	long long int		time_to_die;
	long long int		time_to_eat;
	long long int		time_to_sleep;
	long long int		nb_must_eat;
	long long int		time_to_think;
	int					all_eat;
	int					flag_all_eat;
	int					flag_died;
	t_philo				*philo;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		flag_died_mutex;
	pthread_mutex_t		flag_all_eat_mutex;
	pthread_mutex_t		death_printed_mutex;
	pthread_mutex_t		*forks;
}						t_init;

typedef struct s_data
{
	t_philo	*philo;
	t_init	*init;
}		t_data;

/***************************** actions_philos.c ******************************/
void			action_think(t_philo *philo, t_init *init);
void			action_sleep(t_philo *philo, t_init *init);
void			action_take_fork(t_philo *philo, t_init *init);

/***************************** check_and_stop.c ******************************/
int				check_before_initialize(int ac, char **av);
int				check_flag_died(t_init *init);
int				check_flag_all_eat(t_init *init);
int				check_flag_death_printed(t_init *init);
int				check_time_for_philo_to_die(t_philo *philo, t_init *init);

/********************************* free_all.c *********************************/
void			free_all_mutex_and_forks(t_init *init);

/******************************** init_data.c *********************************/
int				check_single_philo_and_exit(t_init *init, char **av);
t_init			*init_recup_data(t_init *init, int ac, char **av);
t_init			*init_philo(t_init *init);

/******************************** init_mutex.c ********************************/
t_init			*init_write_mutex(t_init *init);
t_init			*init_flag_died_mutex(t_init *data);
t_init			*init_flag_all_eat_mutex(t_init *init);
t_init			*init_death_printed_mutex(t_init *init);
t_init			*init_forks(t_init *data);

/******************************* libft_philo.c ********************************/
long long int	ft_atoi_philo(char *str);
int				ft_isdigit_str(char *str);
int				ft_strcmp(char *s1, char *s2);

/********************************* routine.c **********************************/
void			run_routine_philo(t_init *init);

/********************************* utils.c ************************************/
int				write_error(char *str);
long long		get_time_philo(void);
void			print_action(t_init *init, int id, char *str);

#endif
