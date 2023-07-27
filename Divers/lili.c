void print_action(t_init *init, int id, char *str)
{

	if (check_flag_all_eat(init))
	{
		return;
	}
	if (check_flag_died(init))
	{
		pthread_mutex_lock(&init->death_printed_mutex);
		if (init->death_printed == 0)
		{
			init->death_printed = 1;
			pthread_mutex_unlock(&init->death_printed_mutex);
			pthread_mutex_lock(&init->write_mutex);
			printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
			printf("died\n");
			pthread_mutex_unlock(&init->write_mutex);
		}
		else
			pthread_mutex_unlock(&init->death_printed_mutex);
		return;
	}
	pthread_mutex_lock(&init->write_mutex);
	printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
	printf("%s\n", str);
	pthread_mutex_unlock(&init->write_mutex);
}

void	action_think(t_philo *philo, t_init *init)
{
	if (check_flag_died(init) && check_flag_all_eat(init))
		return;;
	print_action(init, philo->philo_id, "is thinking");
	usleep(init->time_to_think * 1000);
}

void	action_sleep(t_philo *philo, t_init *init)
{
	if (check_flag_died(init) && check_flag_all_eat(init))
		return;
	print_action(init, philo->philo_id, "is sleeping");
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
	if (check_flag_died(init) && check_flag_all_eat(init))
		return;

	print_action(init, philo->philo_id, "is eating");
	
	pthread_mutex_lock(&(philo->eat_mutex)); // tout mettre dans ce mutex pour eviter les bugs
	init->all_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	
	if (init->nb_must_eat != 0 && ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		pthread_mutex_lock(&(init->flag_all_eat_mutex));
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000); //en haut plus opti mais bug pour les repas

	if (check_flag_died(init) && check_flag_all_eat(init))
		return;
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	if (check_flag_died(init) && check_flag_all_eat(init))
		return;
		
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
	}
}
int check_flag_died(t_init *init)
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

int check_flag_all_eat(t_init *init)
{
	pthread_mutex_lock(&init->flag_all_eat_mutex);
	if (init->flag_all_eat == 1)
	{
		pthread_mutex_unlock(&init->flag_all_eat_mutex);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&init->flag_all_eat_mutex);
	return (0);
}

int check_time_for_philo_to_die(t_philo *philo, t_init *init)
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

void *thread_run(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (data->init->flag_died != 1 && data->init->flag_all_eat != 1)
	{
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_take_fork(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_eat(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_drop_fork(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_sleep(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_think(data->philo, data->init);
	}
	return (NULL);
}

void init_run_philosophers(t_init *init, long long int time_init)
{
	t_data *data;
	int i;

	i = 0;
	while ((i < init->nb_of_philo))
	{
		data = malloc(sizeof(t_data));
		if (data == NULL)
			return;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_last_eat = get_time_philo();
		init->philo[i].data = data;
		data->philo->time_init = time_init;
		if (pthread_create(&init->philo[i].thread_philo, NULL, thread_run, data))
			return ;
		i++;
	}
}

void run_routine_philo(t_init *init)
{
	long long int	time_init;
	int				i;

	time_init = get_time_philo();
	i = 0;
	init_run_philosophers(init, time_init);
	while ((i < init->nb_of_philo) && (init->flag_died != 1) && (init->flag_all_eat != 1))
	{
		if (check_flag_died(init))
		{
			check_time_for_philo_to_die(init->philo, init); // permet d'arrêter de créer de nouveaux threads si un philo meurt avant que tous les threads soient créés
			return ;
		}
		pthread_join(init->philo[i].thread_philo, NULL);
		free(init->philo[i].data); // m'a supprime des leaks !
		i++;
	}
}

