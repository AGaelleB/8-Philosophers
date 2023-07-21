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

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>

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
	pthread_mutex_t		eat_mutex;
	pthread_mutex_t		mutex;
	pthread_t			thread_philo;
	long long			time_init;
	struct s_data		*data;
}						t_philo;

typedef struct			s_init
{
	int					nb_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_must_eat;
	int					time_to_think;
	t_philo				*philo;
	int					all_finished_eating;
	pthread_mutex_t		*forks;
	pthread_mutex_t		eat_count_mutex;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		write_mutex;
}						t_init;

typedef struct	s_data
{
	t_philo	*philo;
	t_init	*init;
}		t_data;

/********************************* main_philo.c ************************************/
void	cleanup_all(t_init *data);

/********************************* utils.c ************************************/
int			write_error(char *str);
void		ft_free_tab(char **tab);
long long	get_time_philo(void);
void	print_action(t_init *init, int id, char *str);
void		wait_threads(t_init *data);
void		free_all_mutex(t_init *data);
void		free_all_forks(t_init *data);



/******************************* libft_philo.c ********************************/
int	ft_atoi_philo(char *str);
int ft_isdigit_str(char *str);
int	ft_strcmp(char *s1, char *s2);
int	ft_atoi_philo(char *str);

/******************************** init_data.c *********************************/
t_init		*init_mutex(t_init *data);
t_init		*init_philo(t_init *data);

/******************************** init_philo.c *********************************/
t_init		*init_recup_data(t_init *data, int ac, char **av);
t_init		*init_forks(t_init *data);
// int	init_forks(t_init *data);

t_init		*init_eat_count_mutex(t_init *data);
t_init		*init_write_mutex(t_init *init);
t_init		*init_death_mutex(t_init *data);

/***************************** actions_philos.c ********************************/
void		action_think(t_philo *philo, t_init *data);
void		action_eat(t_philo *philo, t_init *data);
void		action_drop_fork(t_philo *philo, t_init *data);
void		action_sleep(t_philo *philo, t_init *data);
void		action_take_fork(t_philo *philo, t_init *init);

/***************************** routine.c ********************************/

void		*thread_run(void *data);
void		run_routine_philo(t_init *data);
int			check_if_philo_died(t_philo *philo, t_init *data);
void		stop_all_if_flag(t_init *init);
int	check_and_stop_if_philo_died(t_philo *philo, t_init *init);


long long	get_time_since_start(long long start_time);


void	free_all_data(t_init *init);

void	free_all(t_init *data);

void	check_if_died(t_philo *philo, t_init *init);
void check_if_all_died(t_init *init);

// void	check_if_all_died(t_philo *philo, t_init *init);


#endif
