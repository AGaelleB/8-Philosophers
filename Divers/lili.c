void	action_think(t_philo *philo, t_init *init)
{
	pthread_mutex_lock(&init->end_flag_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->end_flag_mutex);
		return;
	}
	pthread_mutex_unlock(&init->end_flag_mutex);
	print_action(init, philo->philo_id, "is thinking");
	usleep(init->time_to_think * 1000);
}

void	action_sleep(t_philo *philo, t_init *init)
{
	pthread_mutex_lock(&init->end_flag_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->end_flag_mutex);
		return;
	}
	pthread_mutex_unlock(&init->end_flag_mutex);
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
	pthread_mutex_lock(&init->end_flag_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->end_flag_mutex);
		return;
	}
	pthread_mutex_unlock(&init->end_flag_mutex);
	int	i;

	i = 0;
	print_action(init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_lock(&(philo->eat_mutex));
	init->philo[i].nb_time_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	if (init->nb_must_eat != 0)
	{
		if ((init->nb_of_philo * init->nb_must_eat)
			== init->philo[i].nb_time_eat)
		{
			pthread_mutex_lock(&(init->end_flag_mutex));
			init->end_flag = 1;
			pthread_mutex_unlock(&(init->end_flag_mutex));
			return ;
		}
		init->all_eat++;
	}
	i++;
	usleep(init->time_to_eat * 1000);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	pthread_mutex_lock(&init->end_flag_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->end_flag_mutex);
		return;
	}
	pthread_mutex_unlock(&init->end_flag_mutex);
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		print_action(init, philo->philo_id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		print_action(init, philo->philo_id, "has taken a fork");
	}
}
