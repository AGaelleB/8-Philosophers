
void	action_think(t_philo *philo, t_init *init)
{
	if (check_end_flag(init))
		return;
	print_action(philo, philo->philo_id, "is thinking");
}

void action_eat(t_philo *philo, t_init *init)
{
	pthread_mutex_lock(&init->death_mutex);
	if (init->nb_must_eat != 0)
	{
		philo->nb_time_eat++;
		if (philo->nb_time_eat >= init->nb_must_eat)
		{
			print_action(philo, philo->philo_id, "is eating");
			init->end_flag = 1;
			printf("Philosopher n.%d a mangé %d/%d.\n", philo->philo_id, philo->nb_time_eat, init->nb_must_eat);
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
	if (check_end_flag(init))
		return;

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
	action_drop_fork(philo, init);
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if (check_end_flag(init))
		return;
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
	if (check_end_flag(init))
		return;
	usleep(init->time_to_sleep * 1000);
	print_action(philo, philo->philo_id, "is sleeping");
}

bool check_end_flag(t_init *init)
{
	bool flag = false;

	pthread_mutex_lock(&init->death_mutex);
	if (init->end_flag == 1)
	{
		flag = true;
	}
	pthread_mutex_unlock(&init->death_mutex);
	
	return flag;
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	if (philo->time_last_eat >= init->time_to_die)
	{
		print_action(philo, philo->philo_id, "died");
		pthread_mutex_lock(&init->death_mutex);
		init->end_flag = 1;
		pthread_mutex_unlock(&init->death_mutex);
		return (init->end_flag = 1);
	}
	else
		return (init->end_flag = 0);
}

void *thread_routine(void *arg)
{
	t_data *data = (t_data *)arg;

	while (check_if_philo_died(data->philo, data->init) == 0 && data->init->end_flag != 1)
	{

		if (check_end_flag(data->init))
			return (NULL);

		if (data->init->end_flag == 1)
			break;
		action_grab_fork(data->philo, data->init);
		if (data->init->end_flag == 1)
			break;
		action_drop_fork(data->philo, data->init);
		if (data->init->end_flag == 1)
			break;
		action_sleep(data->philo, data->init);
		if (data->init->end_flag == 1)
			break;
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
		if (check_end_flag(init))
			return;

		data = malloc(sizeof(t_data));
		if (data == NULL)
			return ;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_init = time_init;
		pthread_create(&init->philo[i].thread_philo, NULL, thread_routine, data);
		i--;
	}
	
	i = init->nb_of_philo - 1;
	while (i >= 0) 
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i--;
	}
}

./philo 3 50 1 4 1
0 3 has taken a fork
0 3 has taken a fork
0 3 is eating
Philosopher n.3 a mangé 1/1.
^C


mon code plente pendant l execution, peux tu corriger mon code ? 