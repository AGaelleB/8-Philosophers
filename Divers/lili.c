comment faire pour regler mon probleme de datarace ?

int	check_flag_died(t_init *init)
{
	pthread_mutex_lock(&(init->flag_died_mutex));
	if (init->flag_died == 1)
	{
		pthread_mutex_unlock(&init->flag_died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->flag_died_mutex);
	return (0);
}

int	check_time_for_philo_to_die(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		pthread_mutex_lock(&(init->flag_died_mutex));
		init->flag_died = 1;
		pthread_mutex_unlock(&(init->flag_died_mutex));
		print_action(init, philo->philo_id, "died");
		return (1);
	}
	return (0);
}

void	ft_usleep(long long duration, t_init *init)
{
	long long	start;

	start = get_time_philo();
	while (!check_time_for_philo_to_die(init->philo, init))
	{
		if (get_time_philo() - start >= duration)
			break ;
		usleep(init->nb_of_philo * 2);
	}
}


void	action_think(t_philo *philo, t_init *init)
{
	if (init->nb_of_philo > 1)
	{
		print_action(init, philo->philo_id, "is thinking");
		ft_usleep(init->time_to_think, init);
	}
}

void	action_sleep(t_philo *philo, t_init *init)
{
	if (init->nb_of_philo > 1)
	{
		print_action(init, philo->philo_id, "is sleeping");
		ft_usleep(init->time_to_sleep, init);
	}
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else if (philo->philo_id % 2 != 0)
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void	action_eat(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is eating");
	pthread_mutex_lock(&(init->flag_all_eat_mutex));
	init->all_eat++;
	if (init->nb_must_eat != 0
		&& ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));
	philo->time_last_eat = get_time_philo();
	ft_usleep(init->time_to_eat, init);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;

	if (init->nb_of_philo > 1)
	{
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
		else if (philo->philo_id % 2 != 0)
		{
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
	}
}

./philo 5 310 200 1500
1 1 has taken a fork
1 1 has taken a fork
1 1 is eating
1 3 has taken a fork
1 3 has taken a fork
1 3 is eating
==================
WARNING: ThreadSanitizer: data race (pid=458782)
  Read of size 8 at 0x7b3c00000010 by thread T3 (mutexes: write M5, write M6):
    #0 check_time_for_philo_to_die srcs/check_and_stop.c:53 (philo+0x233d)
    #1 ft_usleep3 srcs/actions_philos.c:46 (philo+0x19db)
    #2 action_eat srcs/actions_philos.c:112 (philo+0x1e44)
    #3 action_take_fork srcs/actions_philos.c:140 (philo+0x2147)
    #4 thread_run srcs/routine.c:25 (philo+0x313c)

  Previous write of size 8 at 0x7b3c00000010 by thread T1 (mutexes: write M7, write M8):
    #0 action_eat srcs/actions_philos.c:110 (philo+0x1e15)
    #1 action_take_fork srcs/actions_philos.c:140 (philo+0x2147)
    #2 thread_run srcs/routine.c:25 (philo+0x313c)

  Location is heap block of size 240 at 0x7b3c00000000 allocated by main thread:
    #0 malloc ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:655 (libtsan.so.0+0x31c57)
    #1 init_philo srcs/init_data.c:59 (philo+0x2757)
    #2 initialize_data_and_mutex srcs/main_philo.c:39 (philo+0x1687)
    #3 main srcs/main_philo.c:67 (philo+0x179a)

  Mutex M5 (0x7b3400000078) created at:
    #0 pthread_mutex_init ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:1227 (libtsan.so.0+0x4bee1)
    #1 init_forks srcs/init_mutex.c:69 (philo+0x2c48)
    #2 initialize_data_and_mutex srcs/main_philo.c:54 (philo+0x1720)
    #3 main srcs/main_philo.c:67 (philo+0x179a)

  Mutex M6 (0x7b3400000050) created at:
    #0 pthread_mutex_init ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:1227 (libtsan.so.0+0x4bee1)
    #1 init_forks srcs/init_mutex.c:69 (philo+0x2c48)
    #2 initialize_data_and_mutex srcs/main_philo.c:54 (philo+0x1720)
    #3 main srcs/main_philo.c:67 (philo+0x179a)

  Mutex M7 (0x7b3400000028) created at:
    #0 pthread_mutex_init ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:1227 (libtsan.so.0+0x4bee1)
    #1 init_forks srcs/init_mutex.c:69 (philo+0x2c48)
    #2 initialize_data_and_mutex srcs/main_philo.c:54 (philo+0x1720)
    #3 main srcs/main_philo.c:67 (philo+0x179a)

  Mutex M8 (0x7b3400000000) created at:
    #0 pthread_mutex_init ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:1227 (libtsan.so.0+0x4bee1)
    #1 init_forks srcs/init_mutex.c:69 (philo+0x2c48)
    #2 initialize_data_and_mutex srcs/main_philo.c:54 (philo+0x1720)
    #3 main srcs/main_philo.c:67 (philo+0x179a)

  Thread T3 (tid=458786, running) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
    #1 init_and_create_threads srcs/routine.c:54 (philo+0x3429)
    #2 run_routine_philo srcs/routine.c:71 (philo+0x34ac)
    #3 main srcs/main_philo.c:70 (philo+0x17c1)

  Thread T1 (tid=458784, running) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
    #1 init_and_create_threads srcs/routine.c:54 (philo+0x3429)
    #2 run_routine_philo srcs/routine.c:71 (philo+0x34ac)
    #3 main srcs/main_philo.c:70 (philo+0x17c1)

SUMMARY: ThreadSanitizer: data race srcs/check_and_stop.c:53 in check_time_for_philo_to_die
==================
201 5 has taken a fork
201 5 has taken a fork
201 5 is eating
201 1 is sleeping
201 3 is sleeping
201 2 has taken a fork
201 4 has taken a fork
201 2 has taken a fork
201 2 is eating
312 1 died
ThreadSanitizer: reported 1 warnings