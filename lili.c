mon code

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

long long	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
	// 1 sec = 1 000 milisecondes
}

void	print_action(t_philo *philo, int id, char *str)
{
	// printf("%sinit->time_init = %lld\n%s", YELLOW, init->time_init, RESET);
	
	printf("%lli ", get_time() - philo->time_init);
	printf("%i ", id + 1);
	printf("%s\n", str);

}

int	ft_atoi_philo(char *str)
{
	int result = 0;
	int sign = 1;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
			str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
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

t_init	*init_philo(t_init *data)
{
	int	i;

	i = data->nb_of_philo - 1;

	data->philo = malloc(sizeof(t_philo) * data->nb_of_philo);
	if (data->philo == NULL)
		return (NULL);
	while(i >= 0)
	{
		data->philo[i].philo_id = i;
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
	if(ac == 6)
		data->nb_must_eat = ft_atoi_philo(av[5]);
	else
		data->nb_must_eat = 0;
	return (data);
}

void	action_think(t_philo *philo)
{
	// printf("%sphilo->time_init = %lld\n%s", YELLOW, philo->time_init, RESET);
	print_action(philo, philo->philo_id, "is thinking");
}

void	action_eat(t_philo *philo, t_init *data) // ICI
{
	// t_init	*data;
	// ajouter aussi un check de si le temps entre 2 repas a ete respecté, sinon dead

	if (data->nb_must_eat != 0)
	{
		philo->nb_time_eat++;
		if (philo->nb_time_eat >= data->nb_must_eat)
		{
			printf("Philosopher %d a mangé %d fois. Fin de la simulation.\n", philo->philo_id, philo->nb_time_eat);
			// ajouter ici la logique pour terminer la simulation
			// définir un flag pour indiquer la fin de la simulation
			// ou utiliser un mécanisme de synchro pour informer les autres threads
			// que la simulation doit se terminer.
		}
	}
	print_action(philo, philo->philo_id, "is eating");
	philo->time_last_eat = get_time();
}

void	action_grab_fork(t_philo *philo, t_init *data)
{
	// t_init	*data;
	
	// Philo prend d'abord la fourchette avec le plus petit identifiant
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork_id]);
		pthread_mutex_lock(&data->forks[philo->right_fork_id]);
	}
	else 
	{
		pthread_mutex_lock(&data->forks[philo->right_fork_id]);
		pthread_mutex_lock(&data->forks[philo->left_fork_id]);
	}
	print_action(philo, philo->philo_id, "has taken a fork");
	action_eat(philo, data);
	action_drop_fork(philo, data);

}

void	action_drop_fork(t_philo *philo, t_init *data)
{
	// t_init	*data;

	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_unlock(&data->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&data->forks[philo->right_fork_id]);

	}
	else
	{
		pthread_mutex_unlock(&data->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&data->forks[philo->left_fork_id]);
	}
}

void	action_sleep(t_philo *philo)
{
	print_action(philo, philo->philo_id, "is sleeping");
}

void	check_if_philo_died(t_philo *philo)
{
	/*
	Un philosophe meurt s'il n'a pas commencé à manger dans l'intervalle de 
	time_to_die. 
	Creer une autre boucle dans la fonction principale qui vérifie continuellement
	si chaque philosophe a commencé à manger dans le temps imparti
	*/

}


void *thread_routine(void *data)
{
	t_init	*init;
	t_philo	*philo;

	philo = (t_philo*)data;
	init = philo->init_data;  // Use the init_data from the philo struct

	printf("Debut de la routine\n");
	
	action_think(philo);
	action_grab_fork(philo, init);
	action_eat(philo, init);
	action_drop_fork(philo, init);
	action_sleep(philo);
	return (NULL);
}


void	run_routine_philo(t_init *data)
{
	int	i;
	long long time_init;

	// j init mon time ici
	time_init = get_time();
	// printf("%time_init = %lld\n%s", BLUE, time_init, RESET);

	i = data->nb_of_philo - 1;
	// create threads and pass the specific philosopher to each thread
	while(i >= 0)
	{
		data->philo[i].time_init = time_init;
		data->philo[i].init_data = data; // utile ? 
		pthread_create(&data->philo[i].thread_philo, NULL, thread_routine, &data->philo[i]);
		i--;
	}
	
	i = data->nb_of_philo - 1;
	// pthread_join attend que tous les threads se terminent avant de continuer
	while (i >= 0) 
	{
		pthread_join(data->philo[i].thread_philo, NULL);
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
	run_routine_philo(data);
	return (0);
}

