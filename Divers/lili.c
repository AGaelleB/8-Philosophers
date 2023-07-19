

pourquoi mon programme ne stop pas lors de l execution de print_action lorsque je demande d ecrir "died", normalement le programme devrait se stopper des le premier "d" rencontré

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
	struct s_data *data; // Ajout d'un pointeur vers la structure t_data Baptiste
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
	int					end_flag;
	int					all_finished_eating; // verif si tous les repas sont pris
	pthread_mutex_t		*forks; // me permet d utiliser pthread_mutex_lock sans erreurs
	pthread_mutex_t		eat_count_mutex; // Mutex pour protéger le compteur
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		write_mutex; // permet d ecrire sans superposer 
}						t_init;

typedef struct	s_data
{
	t_philo	*philo;
	t_init	*init;
}		t_data;

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

// void	action_sleep(t_philo *philo, t_init *init) // BAPTISTE
// {
// 	long long int start_sleep_time;
// 	long long int end_sleep_time;
// 	long long int sleep_duration;

// 	print_action(philo, init, philo->philo_id, "is sleeping");
// 	start_sleep_time = get_time_philo();
// 	usleep(init->time_to_sleep * 1000);
// 	end_sleep_time = get_time_philo();
// 	sleep_duration = end_sleep_time - start_sleep_time;
// 	if (sleep_duration < init->time_to_sleep)
// 		usleep(init->time_to_sleep - sleep_duration);
// }


void	action_eat(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_lock(&init ->eat_count_mutex); // mauvais mutex je pense
	philo->nb_time_eat++;
	pthread_mutex_unlock(&init->eat_count_mutex);
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
				print_action(philo, init, philo->philo_id, "END TOUT LE MONDE A GRAILLE !!!!");
				init->end_flag = 1;
				cleanup_all_mutex(init);
				pthread_mutex_unlock(&init->death_mutex);
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


void	print_action(t_philo *philo, t_init *init, int id, char *str)
{
	if(str[1] == 'd')
		exit(-1);
	pthread_mutex_lock(&init->write_mutex);
	printf("%lli ", get_time_philo() - philo->time_init);
	printf("%i ", id);
	printf("%s\n", str);
	pthread_mutex_unlock(&init->write_mutex);
}


void check_death(t_init *init, t_philo *philo)
{
	long long int current_time = get_time_philo();
	long long int time_since_last_eat = current_time - philo->time_last_eat;
	if ((time_since_last_eat > (long long int)init->time_to_die) || init->end_flag == 1)
	{
		pthread_mutex_lock(&(init->death_mutex));
		print_action(philo, init, philo->philo_id, "IL EST MORT OK C'EST FINI MAINTENANT !!!!");
		print_action(philo, init, philo->philo_id, "died");
		pthread_mutex_unlock(&(init->death_mutex));
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
	int	i;

	i = 1;
	while (i <= init->nb_must_eat)
	{
		action_take_fork(philo, init);
		if(i == init->nb_must_eat)
			break;
		check_all_deaths(init);
		
		action_sleep(philo, init);
		check_all_deaths(init);
		
		action_think(philo, init);
		check_all_deaths(init);
		
		i++;
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