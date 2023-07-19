int	write_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(2, "Error: ", 7);
	write(2, str, len);
	write(2, "\n", 1);
	return (0);
}

long long	get_time_philo(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_action(t_philo *philo, t_init *init, int id, char *str)
{
	pthread_mutex_lock(&init->write_mutex);
	printf("%lli ", get_time_philo() - philo->time_init);
	printf("%d ", id);
	printf("%s\n", str);
	if (ft_strcmp(str, "died") == 0)
		exit(-1);
	pthread_mutex_unlock(&init->write_mutex);
}

void	cleanup_all_mutex(t_init *data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->eat_count_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->write_mutex);
	while (i < data->nb_of_philo)
	{
		pthread_mutex_destroy(&data->philo[i].mutex);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	free(data->philo);
	free(data);
}

void	cleanup_forks(t_init *data)
{
	int	i;

	if (data->forks != NULL)
	{
		i = data->nb_of_philo - 1;
		while (i >= 0)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i--;
		}
		free(data->forks);
		data->forks = NULL;
	}
}

void	action_think(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is thinking");
	usleep(init->time_to_think * 1000);
}

void	action_sleep(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is sleeping");
	usleep(init->time_to_sleep * 1000);
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void	action_eat(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	philo->nb_time_eat++;
	usleep(init->time_to_eat * 1000);
	if (init->nb_must_eat != 0)
	{
		if (philo->nb_time_eat >= init->nb_must_eat)
		{
			pthread_mutex_lock(&init ->eat_count_mutex);
			init->all_finished_eating++;
			pthread_mutex_unlock(&init->eat_count_mutex);
			if (init->all_finished_eating == init->nb_of_philo)
			{
				pthread_mutex_lock(&init->death_mutex);
				cleanup_all_mutex(init);
				pthread_mutex_unlock(&init->death_mutex);
				// check_all_deaths(init);
				exit(0);
			}
		}
	}
	action_drop_fork(philo, init);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		}
		else
		{
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		}
		print_action(philo, init, philo->philo_id, "has taken a fork");
		print_action(philo, init, philo->philo_id, "has taken a fork");
		action_eat(philo, init);
}


void check_death(t_init *init, t_philo *philo)
{
	long long int current_time = get_time_philo();
	long long int time_since_last_eat = current_time - philo->time_last_eat;
	int end = 0;

	if (time_since_last_eat > init->time_to_die)
	{
		pthread_mutex_lock(&init->death_mutex);
		end = 1;
		pthread_mutex_unlock(&init->death_mutex);
	}
	if (end == 1)
	{
		pthread_mutex_lock(&init->death_mutex);
		print_action(philo, init, philo->philo_id, "died");
		cleanup_all_mutex(init);
		pthread_mutex_unlock(&init->death_mutex);
	}
}

void check_all_deaths(t_init *init)
{
	int i = 0;
	while (i < init->nb_of_philo)
	{
		check_death(init, &init->philo[i]);
		i++;
	}
}

void	routine_one(t_init*init, t_philo *philo)
{
	while (philo->nb_time_eat != init->nb_must_eat)
	{
		action_take_fork(philo, init);
		if (philo->nb_time_eat == init->nb_must_eat)
			break;
		check_all_deaths(init);

		action_sleep(philo, init);
		check_all_deaths(init);
		
		action_think(philo, init);
		check_all_deaths(init);
	}
}

void routine_two(t_init *init, t_philo *philo)
{
	while (1)
	{
		action_take_fork(philo, init);
		check_all_deaths(init);
		
		action_sleep(philo, init);
		check_all_deaths(init);
				
		action_think(philo, init);
		check_all_deaths(init);
				
	}
}

void	*thread_run(void *arg)
{
	t_data *data = (t_data *)arg;

	if(data->init->nb_must_eat > 0)
		routine_one(data->init, data->philo);
	else 
		routine_two(data->init, data->philo);
	return(NULL);
}

void run_routine_philo(t_init *init) 
{
	int i = 0;
	long long int time_init = get_time_philo();
	t_data *data = NULL;

	while (i < init->nb_of_philo)
	{
		data = malloc(sizeof(t_data));
		if (!data)
		{
			int j = 0;
			while (j < i)
			{
				free(init->philo[j].data);
				j++;
			}
			return;
		}
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_init = time_init;
		data->philo->time_last_eat = get_time_philo();
		init->philo[i].data = data;
		pthread_create(&init->philo[i].thread_philo, NULL, thread_run, data);
		i++;
	}
	i = 0;
	while (i < init->nb_of_philo)
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i++;
	}
	i = 0;
	while (i < init->nb_of_philo)
	{
		free(init->philo[i].data);
		i++;
	}
}

void	cleanup_all(t_init *data)
{
	cleanup_all_mutex(data);
	cleanup_forks(data);
}

t_init	*initialize_data_and_mutex(t_init *data, int ac, char **av)
{
	data = init_recup_data(data, ac, av);
	data = init_philo(data);
	data = init_mutex(data);
	data = init_write_mutex(data);
	data = init_eat_count_mutex(data);
	data = init_death_mutex(data);
	data = init_forks(data);
	return (data);
}

int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;
	if (ac != 6 && ac != 5)
		return (write_error("Wrong numbers of arguments"));
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2])
		|| !ft_isdigit_str(av[3]) || !ft_isdigit_str(av[4])
		|| (ac == 6 && !ft_isdigit_str(av[5])))
		return (write_error("Invalid value"));
	data = initialize_data_and_mutex(data, ac, av);
	if (!data)
	{
		cleanup_all(data);
		return (write_error("Failed to initialize"));
	}
	run_routine_philo(data);
	cleanup_all(data);
	return (0);
}
