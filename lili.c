//parfois mes philos meurent, jepense que l erreur se trouve dans l utilisaation de mes usleep(); peux tu arranger mon code

int	action_think(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	print_action(philo, init, philo->philo_id, "is thinking");
	return (1);
}

int action_eat(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
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
				print_action(philo, init, philo->philo_id, "is eating");
				printf("%sAll Philosophers eat %d/%d. stopping program\n%s", RED, philo->nb_time_eat, init->nb_must_eat, RESET);
				pthread_mutex_unlock(&init->death_mutex);
				stop_all_if_flag(init);
				return (1);
			}
			print_action(philo, init, philo->philo_id, "is eating");
			pthread_mutex_unlock(&init->death_mutex);
			return (1);
		}
	}
	print_action(philo, init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_unlock(&init->death_mutex);
	return (1);
}

int action_grab_fork(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		print_action(philo, init, philo->philo_id, "has taken a fork"); // left fork 1
		print_action(philo, init, philo->philo_id, "has taken a fork"); // right fork 2
		usleep(init->time_to_eat * 1000);  // avancer pour eviter les deadlocks
	}
	else 
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		print_action(philo, init, philo->philo_id, "has taken a fork"); // left fork 1
		print_action(philo, init, philo->philo_id, "has taken a fork"); // right fork 2
		usleep(init->time_to_eat * 1000);  // avancer pour eviter les deadlocks

	}
	usleep(50);
	return (1);
}

int	action_drop_fork(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
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
	return (1);
}

int	action_sleep(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is sleeping");
	usleep(init->time_to_sleep * 1000); // avancer pour eviter les deadlocks
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	return (1);
}

void stop_all_if_flag(t_init *init)
{
	// usleep(50); // test
	pthread_mutex_lock(&init->death_mutex);
	if (init->end_flag == 1)
	{
		printf("%sEnd flag is set : %d. stopping program%s\n", RED, init->end_flag == 1, RESET);
		pthread_mutex_unlock(&init->death_mutex);
		exit(0);
	}
	pthread_mutex_unlock(&init->death_mutex);
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	// usleep(50); // test
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		print_action(philo, init, philo->philo_id, "died");
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

	while (1)
	{
		action_grab_fork(data->philo, data->init);
		action_eat(data->philo, data->init);
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
		data->philo->time_last_eat = get_time_philo();
		pthread_create(&init->philo[i].thread_philo, NULL, thread_routine, data);
		i--;
	}
	while (!init->end_flag)
		usleep(100); // wait for some time, then check again
	
	i = init->nb_of_philo - 1;
	while (i >= 0) 
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i--;
	}
}