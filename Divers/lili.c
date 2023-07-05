valgrind --leak-check=full --show-leak-kinds=all ./philo 2 310 200 100
==976247== Memcheck, a memory error detector
==976247== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==976247== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==976247== Command: ./philo 2 310 200 100
==976247== 
==976247== Conditional jump or move depends on uninitialised value(s)
==976247==    at 0x10A187: stop_all_if_flag (routine.c:18)
==976247==    by 0x10A30B: run_routine_philo (routine.c:68)
==976247==    by 0x1094EE: main (main_philo.c:48)
==976247== 
40 1 has taken a fork
49 1 has taken a fork
50 1 is eating
255 2 has taken a fork
256 2 has taken a fork
257 1 is sleeping
257 2 is eating
359 1 is thinking
366 1 died



t_init	*init_death_mutex(t_init *data)
{
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for death check\n");
		free(data->philo);
		free(data);
		return (NULL);
	}
	return (data);
}


void stop_all_if_flag(t_init *init)
{
	pthread_mutex_lock(&init->death_mutex); // a tester de deplacer le lock et unlock dans le if voir si crash
	if (init->end_flag == 1)
	{
		printf("%sPhilo died%s\n", RED, RESET);
		pthread_mutex_unlock(&init->death_mutex);
		exit(0);
	}
	pthread_mutex_unlock(&init->death_mutex);
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		pthread_mutex_lock(&init->death_mutex);
		print_action(philo, init, philo->philo_id, "died");
		init->end_flag = 1;
		exit (1); // magouille a free
		pthread_mutex_unlock(&init->death_mutex);
	}
	else
		return (0);
}

int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;
	if (ac != 6 && ac != 5)
		return (write_error("Wrong numbers of arguments"));
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2]) || !ft_isdigit_str(av[3])
		|| !ft_isdigit_str(av[4]) || (ac == 6 && !ft_isdigit_str(av[5])))
		return (write_error("Invalid value"));
	data = init_recup_data(data, ac, av);
	if (data == NULL)
		return (write_error("Failed to initialize data"));
	data = init_philo(data);
	if (data == NULL)
		return (write_error("Failed to initialize philo"));
	data = init_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize mutex"));
	data = init_write_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize write_mutex"));
	data = init_eat_count_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize eat_count_mutex"));

	data = init_death_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize init_death_mutex"));

	data = init_forks(data);
	if (data == NULL)
		return (write_error("Failed to initialize forks"));
	run_routine_philo(data);
	wait_threads(data);
	cleanup_forks(data);
	cleanup_all_mutex(data);
	return (0);
}
