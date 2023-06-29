Je ne comprend pas pourquoi mon philo meurt des que j execute peux-tu modifier mes fonctions pour correspondre a ce qui est demandé dans le sujet: 

./philo 3 4000 1 1  
0 2 has taken a fork
0 2 has taken a fork
0 2 is eating
1 2 died
End flag is set : 1 stopping program

Voici mon code : 

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
	int					time_to_think;
	t_philo				*philo;
	pthread_mutex_t		*forks; // me permet d utiliser pthread_mutex_lock sans erreurs
	int					end_flag;
	pthread_mutex_t		death_mutex;

}						t_init;

typedef struct	s_data
{
	t_philo	*philo;
	t_init	*init;
}		t_data;

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
			print_action(philo, philo->philo_id, "is eating");
			init->end_flag = 1;
			printf("%sPhilosopher n.%d eat %d/%d. stopping program\n%s", RED, philo->philo_id, philo->nb_time_eat, init->nb_must_eat, RESET);
			pthread_mutex_unlock(&init->death_mutex);
			stop_all_if_flag(init);
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
	action_drop_fork(philo, init);
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

	printf("%sin action_sleep init->end_flag = %d\n%s", GREEN, init->end_flag, RESET);  // DEBUG
	
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
	}

	printf("DEBUT time sleep de philo n.%d = %d\n", philo->philo_id,  init->time_to_sleep);
	usleep(init->time_to_sleep * 1000);
	printf("FIN time sleep de philo n.%d = %d\n\n", philo->philo_id, init->time_to_sleep);

	print_action(philo, philo->philo_id, "is sleeping");
}


void stop_all_if_flag(t_init *init)
{
	pthread_mutex_lock(&init->death_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->death_mutex);
		printf("%sEnd flag is set : %d stopping program%s\n", RED, init->end_flag == 1, RESET);
		exit(0);
	}
	pthread_mutex_unlock(&init->death_mutex);
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	if (philo->time_last_eat >= init->time_to_die)
	{
		print_action(philo, philo->philo_id, "died");
		// pthread_mutex_lock(&init->death_mutex);
		init->end_flag = 1;
		// pthread_mutex_unlock(&init->death_mutex);
		return (init->end_flag = 1);
	}
	else
		return (init->end_flag = 0);
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
		// data->philo[i].init_data = init; // utile ? 
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
