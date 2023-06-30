voici ma structure : 

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


voici mon code :


int	write_error(char *str)
{
	int	len;

	len = 0;
	while(str[len])
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
	// 1 sec = 1 000 milisecondes
}

void	print_action(t_philo *philo, int id, char *str)
{
	printf("%lli ", get_time_philo() - philo->time_init);
	printf("%i ", id); // + 1);
	printf("%s\n", str);
}

void cleanup_forks(t_init *data)
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

t_init	*init_mutex(t_init *data)
{
	int	i;
	int	mutex;

	i = data->nb_of_philo - 1;
	while(i >= 0)
	{
		mutex = pthread_mutex_init(&data->philo[i].mutex, NULL);
		if (mutex != 0)
		{
			printf("Failed to initialize mutex for philosopher %d\n", i);
			free(data->philo);
			free(data);
			return (NULL);
		}
		i--;
	}
	return (data);
}

t_init	*init_eat_count_mutex(t_init *data)
{
	// Initialisation du mutex pour le compteur
	if (pthread_mutex_init(&data->eat_count_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for eat count\n");
		free(data->philo);
		free(data);
		return (NULL);
	}
	return (data);
}


t_init	*init_forks(t_init *data)
{
	int i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philo);
	if (data->forks == NULL)
		return (NULL);

	i = data->nb_of_philo - 1;
	while (i >= 0)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			// Ajout d'un nettoyage pour éviter les leaks
			while(i < data->nb_of_philo)
			{
				pthread_mutex_destroy(&data->forks[i]);
				i++;
			}
			free(data->forks);
			return (NULL);
		}
		i--;
	}
	return (data);
}

t_init	*init_philo(t_init *data)
{
	int	i;

	i = data->nb_of_philo - 1;

	data->philo = malloc(sizeof(t_philo) * (data->nb_of_philo));
	if (data->philo == NULL)
		return (NULL);
	while(i >= 0)
	{
		data->philo[i].philo_id = i + 1; // + 1 ajouter pour decompte;
		data->philo[i].nb_time_eat = 0;
		data->philo[i].left_fork_id = i;
		data->philo[i].right_fork_id = (i + 1) % data->nb_of_philo; // lili  Cela permet de connecter le dernier philosophe avec la première fourchette du tableau, formant ainsi une boucle.
		data->philo[i].time_last_eat = 0;
		i--;
	}
	return (data);
}

t_init	*init_recup_data(t_init *data, int ac, char **av)
{

	data = malloc(sizeof(t_init));
	if (data == NULL)
		return (NULL);
	data->nb_of_philo = ft_atoi_philo(av[1]);
	data->time_to_die = ft_atoi_philo(av[2]);
	data->time_to_eat = ft_atoi_philo(av[3]);
	data->time_to_sleep = ft_atoi_philo(av[4]);
	data->all_finished_eating = 0;

	if(ac == 6)
		data->nb_must_eat = ft_atoi_philo(av[5]);
	else
		data->nb_must_eat = 0;

	return (data);
}

void	action_think(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
	}
	print_action(philo, philo->philo_id, "is thinking");
}

void action_eat(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
	}
	pthread_mutex_lock(&init->death_mutex);
	if (init->nb_must_eat != 0)
	{
		philo->nb_time_eat++;
		if (philo->nb_time_eat >= init->nb_must_eat)
		{
			pthread_mutex_lock(&init->eat_count_mutex);
			init->all_finished_eating++;
			pthread_mutex_unlock(&init->eat_count_mutex);
			if (init->all_finished_eating == init->nb_of_philo)
			{
				init->end_flag = 1;
				printf("%sAll Philosophers eat %d/%d. stopping program\n%s", RED, philo->nb_time_eat, init->nb_must_eat, RESET);
				pthread_mutex_unlock(&init->death_mutex);
				stop_all_if_flag(init);
				return ;
			}
			print_action(philo, philo->philo_id, "is eating");
			pthread_mutex_unlock(&init->death_mutex);
			return ;
		}
	}
	print_action(philo, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_unlock(&init->death_mutex);
}

void	action_grab_fork(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
	}
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
	}
	else 
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
	}
	print_action(philo, philo->philo_id, "has taken a fork"); // left fork 1
	print_action(philo, philo->philo_id, "has taken a fork"); // right fork 2
	action_eat(philo, init);
	usleep(init->time_to_eat * 1000);
	// action_drop_fork(philo, init); // doublons ?? 
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
	}
	
	if (philo->left_fork_id < philo->right_fork_id)
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

void	action_sleep(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
	}
	usleep(init->time_to_sleep * 1000);
	print_action(philo, philo->philo_id, "is sleeping");
}

void stop_all_if_flag(t_init *init)
{
	pthread_mutex_lock(&init->death_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->death_mutex);
		printf("%sEnd flag is set : %d. stopping program%s\n", RED, init->end_flag == 1, RESET);
		exit(0);
	}
	pthread_mutex_unlock(&init->death_mutex);
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) >= init->time_to_die)
	{
		print_action(philo, philo->philo_id, "died");
		pthread_mutex_lock(&init->death_mutex);
		init->end_flag = 1;
		pthread_mutex_unlock(&init->death_mutex);
		return (1);
	}
	else
		return (0);
}

void *thread_routine(void *arg)
{
	t_data *data = (t_data *)arg;

	while (check_if_philo_died(data->philo, data->init) == 0)
	{
		stop_all_if_flag(data->init);
		action_grab_fork(data->philo, data->init);
		action_drop_fork(data->philo, data->init);
		action_sleep(data->philo, data->init);
		action_think(data->philo, data->init);
	}
	return (NULL);
}

void	run_routine_philo(t_init *init)
{
	int	i;
	long long time_init;
	t_data *data;

	time_init = get_time_philo();

	// création des threads par philosophers
	i = init->nb_of_philo - 1;
	while(i >= 0)
	{
		stop_all_if_flag(init);
		data = malloc(sizeof(t_data));
		if (data == NULL)
			return ;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_init = time_init;
		data->philo->time_last_eat = get_time_philo(); // Initialize time_last_eat to current time
		pthread_create(&init->philo[i].thread_philo, NULL, thread_routine, data);
		i--;
	}
	i = init->nb_of_philo - 1;
	// pthread_join attend que tous les threads se terminent avant de continuer
	while (i >= 0) 
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i--;
	}
}

int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;

	if(ac != 6 && ac != 5)
		return (write_error("Wrong numbers of arguments"));
	data = init_recup_data(data, ac, av);
	if (data == NULL)
		return (write_error("Failed to initialize data"));
	data = init_philo(data);
	if (data == NULL)
		return (write_error("Failed to initialize philo"));
	data = init_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize mutex"));
	data = init_forks(data);
	if (data == NULL)
		return (write_error("Failed to initialize forks"));
	run_routine_philo(data);

	cleanup_forks(data);
	return (0);
}
