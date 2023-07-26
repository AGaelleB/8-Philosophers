/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 18:50:50 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/26 17:35:27 by abonnefo         ###   ########.fr       */
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

typedef struct s_philo
{
	int					philo_id;
	int					nb_time_eat;
	int					left_fork_id;
	int					right_fork_id;
	long long			time_last_eat;
	long long			time_init;
	pthread_mutex_t		eat_mutex;
	pthread_t			thread_philo;
	struct s_data		*data;
}						t_philo;

typedef struct s_init
{
	int					nb_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_must_eat;
	int					time_to_think;
	int					all_eat;
	int					end_flag;
	t_philo				*philo;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		end_flag_mutex;
	pthread_mutex_t		*forks;
}						t_init;

typedef struct s_data
{
	t_philo	*philo;
	t_init	*init;
}		t_data;

/***************************** actions_philos.c ******************************/
void		action_think(t_philo *philo, t_init *init);
void		action_sleep(t_philo *philo, t_init *init);

/********************************* free_all.c *********************************/
void		free_all_mutex_and_forks(t_init *init);
void		free_data(t_data *data);

/******************************** init_data.c *********************************/
t_init		*init_recup_data(t_init *init, int ac, char **av);
t_init		*init_philo(t_init *init);
void		action_take_fork(t_philo *philo, t_init *init);

/******************************** init_mutex.c ********************************/
t_init		*init_mutex(t_init *data);
t_init		*init_write_mutex(t_init *init);
t_init		*init_eat_mutex(t_init *data);
t_init		*init_end_flag_mutex(t_init *data);
t_init		*init_forks(t_init *data);

/******************************* libft_philo.c ********************************/
int			ft_atoi_philo(char *str);
int			ft_isdigit_str(char *str);
int			ft_strcmp(char *s1, char *s2);

/********************************* routine.c **********************************/
int			check_and_stop_if_philo_died(t_philo *philo, t_init *init);
void		run_routine_philo(t_init *init);

/********************************* utils.c ************************************/
int			write_error(char *str);
long long	get_time_philo(void);
void		print_action(t_init *init, int id, char *str);


void	action_eat(t_philo *philo, t_init *init);
void	action_drop_fork(t_philo *philo, t_init *init);


#endif
