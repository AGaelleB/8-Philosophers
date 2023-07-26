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
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		*forks;
}						t_init;

typedef struct s_data
{
	t_philo	*philo;
	t_init	*init;
}		t_data;

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

void	print_action(t_init *init, int id, char *str)
{
	pthread_mutex_lock(&init->write_mutex);
	printf("%lld ", (get_time_philo() - init->philo->time_init));
	printf("%d ", id);
	printf("%s\n", str);
	if (ft_strcmp(str, "died") == 0)
	{
		free_all_mutex_and_forks(init);
		exit (-1);
	}
	if ((init->end_flag == 1) && (ft_strcmp(str, "is eating") == 0))
	{
		free_all_mutex_and_forks(init);
		exit (-1);
	}
	pthread_mutex_unlock(&init->write_mutex);
}

void	calculate_and_set_time_to_think(t_init *init)
{
	int	time_to_eat_and_sleep;
	int	time_to_think;

	time_to_eat_and_sleep = init->time_to_eat + init->time_to_sleep;
	time_to_think = (init->time_to_die - (time_to_eat_and_sleep)) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	init->time_to_think = time_to_think;
}

void	check_single_philo_and_exit(t_init *init, char **av)
{
	if (init->nb_of_philo == 1)
	{
		usleep(init->time_to_die * 1000);
		printf("%d %s died\n", ft_atoi_philo(av[2]) + 1, av[1]);
		free_all_mutex_and_forks(init);
		exit (-1);
	}
}

void	check_and_set_nb_must_eat(t_init *init, int ac, char **av)
{
	if (ac == 6)
	{
		init->nb_must_eat = ft_atoi_philo(av[5]);
		if (init->nb_must_eat == 0)
		{
			write(2, "", 1);
			free_all_mutex_and_forks(init);
			exit (-1);
		}
	}
	else
		init->nb_must_eat = 0;
}

t_init	*init_recup_data(t_init *init, int ac, char **av)
{
	init = malloc(sizeof(t_init));
	if (init == NULL)
	{
		free(init);
		return (NULL);
	}
	init->nb_of_philo = ft_atoi_philo(av[1]);
	init->time_to_die = ft_atoi_philo(av[2]);
	init->time_to_eat = ft_atoi_philo(av[3]);
	init->time_to_sleep = ft_atoi_philo(av[4]);
	calculate_and_set_time_to_think(init);
	check_and_set_nb_must_eat(init, ac, av);
	check_single_philo_and_exit(init, av);
	return (init);
}

t_init	*init_philo(t_init *init)
{
	int	i;

	i = init->nb_of_philo - 1;
	init->philo = malloc(sizeof(t_philo) * (init->nb_of_philo));
	if (init->philo == NULL)
		return (NULL);
	while (i >= 0)
	{
		init->philo[i].philo_id = i + 1;
		init->philo[i].nb_time_eat = 0;
		init->all_eat = 0;
		init->end_flag = 0;
		init->philo[i].left_fork_id = i;
		if (init->nb_of_philo == 1)
			init->philo[i].right_fork_id = (i + 1);
		else
			init->philo[i].right_fork_id = (i + 1) % init->nb_of_philo;
		init->philo[i].time_last_eat = 0;
		pthread_mutex_init(&(init->philo[i].eat_mutex), NULL);
		i--;
	}
	return (init);
}

t_init	*init_write_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->write_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for write\n");
		free_all_mutex_and_forks(init);
		return (NULL);
	}
	return (init);
}

t_init	*init_death_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->death_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for death check\n");
		free_all_mutex_and_forks(init);
		return (NULL);
	}
	return (init);
}

t_init	*init_forks(t_init *init)
{
	int	i;

	init->forks = malloc(sizeof(pthread_mutex_t) * init->nb_of_philo);
	if (init->forks == NULL)
	{
		free_all_mutex_and_forks(init);
		return (NULL);
	}
	i = init->nb_of_philo - 1;
	while (i >= 0)
	{
		if (pthread_mutex_init(&init->forks[i], NULL) != 0)
		{
			while (i < init->nb_of_philo)
			{
				pthread_mutex_destroy(&init->forks[i]);
				i++;
			}
			free(init->forks);
			return (NULL);
		}
		i--;
	}
	return (init);
}

void	free_data(t_data *data)
{
	free(data);
}

void free_all_mutex(t_init *init)
{
	int i = 0;
	while (i < init->nb_of_philo)
	{
		pthread_mutex_destroy(&init->philo[i].eat_mutex);
		i++;
	}
	pthread_mutex_destroy(&init->write_mutex);
	pthread_mutex_destroy(&init->death_mutex);
}

void	free_all_forks(t_init *init)
{
	int	i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		pthread_mutex_destroy(&init->forks[i]);
		i++;
	}
}

void free_all_mutex_and_forks(t_init *init)
{
	free_all_forks(init);
	free_all_mutex(init);
	free(init->philo);
	free(init->forks);
	free(init);
}

void	action_think(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is thinking");
	check_and_stop_if_philo_died(philo, init);
	usleep(init->time_to_think * 1000);
	check_and_stop_if_philo_died(philo, init);
}

void	action_sleep(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is sleeping");
	check_and_stop_if_philo_died(philo, init);
	usleep(init->time_to_sleep * 1000);
	check_and_stop_if_philo_died(philo, init);
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	check_and_stop_if_philo_died(philo, init);
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
	int	i;

	i = 0;
	print_action(init, philo->philo_id, "is eating");
	check_and_stop_if_philo_died(philo, init);
	philo->time_last_eat = get_time_philo();
	pthread_mutex_lock(&(philo->eat_mutex));
	init->philo[i].nb_time_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	if (init->nb_must_eat != 0)
	{
		if ((init->nb_of_philo * init->nb_must_eat)
			== init->philo[i].nb_time_eat)
		{
			init->end_flag = 1;
			check_and_stop_if_philo_died(philo, init);
			free_all_mutex_and_forks(init);
			exit (-1);
		}
		init->all_eat++;
	}
	i++;
	check_and_stop_if_philo_died(philo, init);
	usleep(init->time_to_eat * 1000);
	check_and_stop_if_philo_died(philo, init);
	action_drop_fork(philo, init);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_and_stop_if_philo_died(philo, init);
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
	print_action(init, philo->philo_id, "has taken a fork");
	print_action(init, philo->philo_id, "has taken a fork");
	// check_and_stop_if_philo_died(philo, init);
	action_eat(philo, init);
}

void *thread_run(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (1)
	{
		action_take_fork(data->philo, data->init);
		action_sleep(data->philo, data->init);
		action_think(data->philo, data->init);
	}
	return (NULL);
}

int check_and_stop_if_philo_died(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		usleep(200); // permet de pas mourir fourchette a la main
		print_action(init, philo->philo_id, "died");
	}
	return (0);
}

void init_run_philosophers(t_init *init, long long int time_init)
{
	t_data *data;
	int i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		data = malloc(sizeof(t_data));
		if (data == NULL)
		{
			free_all_mutex_and_forks(init);
			free(data);
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
}

void run_routine_philo(t_init *init)
{
	long long int time_init;
	int i;

	time_init = get_time_philo();
	i = 0;
	init_run_philosophers(init, time_init);
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
	free_all_mutex_and_forks(init);
}

t_init *initialize_data_and_mutex(t_init *data, int ac, char **av)
{
	data = init_recup_data(data, ac, av);
	if (!data)
		return (NULL);
	data = init_philo(data);
	if (!data)
		return (NULL);
	data = init_write_mutex(data);
	if (!data)
		return (NULL);
	data = init_death_mutex(data);
	if (!data)
		return (NULL);
	data = init_forks(data);
	if (!data)
		return (NULL);
	return data;
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
		free_all_mutex_and_forks(data);
		free(data);
		return (write_error("Failed to initialize"));
	}
	run_routine_philo(data);
	int i = 0;
	while (i < data->nb_of_philo)
	{
		free_data(data->philo[i].data);
		i++;
	}
	free(data->philo);
	free_all_mutex_and_forks(data);
	free(data);
	return (0);
}

