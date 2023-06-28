// je souhaite creer une fonction check_if_philo_died qui bouclera dans thread_routine pour verifier et arreter le programme des le 1er deces de philo. Cependant les donnes sont mal stcikees et mes data semblent ne pas etre enregistrees. Est-ce que tu vois mon erreur et si oui peux tu me la corriger ? 

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <stdint.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <pthread.h>
#include <sys/time.h>

# define RESET "\033[0m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

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
}						t_init;

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

	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000)); // - time start ? 
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

		// printf("%s***** INIT PHILO nb : '%d' *****\n", BLUE, data->philo[i].philo_id);
		// printf("philo_id = %d\n", data->philo[i].philo_id);
		// printf("nb_time_eat = %d\n", data->philo[i].nb_time_eat);
		// printf("left_fork_id = %d\n", data->philo[i].left_fork_id );
		// printf("right_fork_id = %d\n", data->philo[i].right_fork_id);
		// printf("time_last_eat = %lld\n", data->philo[i].time_last_eat);
		// printf("i = %d\n", i);
		// printf("********** END INIT ***********%s\n\n", RESET);
		
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
	data->time_to_think = data->time_to_die - (data->time_to_eat + data->time_to_sleep);
	if(ac == 6)
		data->nb_must_eat = ft_atoi_philo(av[5]);
	else
		data->nb_must_eat = 0;
		
	printf("%s***** INIT DATA *****\n", MAGENTA);
	printf("nb_of_philo = %d\n", data->nb_of_philo);
	printf("time_to_die = %d\n", data->time_to_die);
	printf("time_to_eat = %d\n", data->time_to_eat);
	printf("time_to_sleep = %d\n", data->time_to_sleep);
	printf("nb_must_eat = %d\n", data->nb_must_eat);
	printf("time_to_think = %d\n", data->time_to_think);
	printf("****** END INIT ******\n\n%s", RESET);

	return (data);
}

void	action_think(t_philo *philo, t_init *data)
{
	usleep(data->time_to_sleep * 1000);

	// printf("%sphilo->time_init = %lld\n%s", YELLOW, philo->, RESET);
	print_action(philo, philo->philo_id, "is thinking");
}

void	action_eat(t_philo *philo, t_init *data) // ICI
{
	// ajouter aussi un check de si le temps entre 2 repas a ete respecté, sinon dead

	if (data->nb_must_eat != 0)
	{
		philo->nb_time_eat++;
		if (philo->nb_time_eat >= data->nb_must_eat)
		{
			printf("Philosopher n.%d a mangé %d fois. Fin de la simulation.\n", philo->philo_id, philo->nb_time_eat);
			// ajouter ici la logique pour terminer la simulation
			// définir un flag pour indiquer la fin de la simulation
			// ou utiliser un mécanisme de synchro pour informer les autres threads
			// que la simulation doit se terminer.
		}
	}
	print_action(philo, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
}

void	action_grab_fork(t_philo *philo, t_init *data)
{
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

void	action_sleep(t_philo *philo, t_init *data)
{
	// printf("DEBUT time sleep de philo n.%d = %d\n", philo->philo_id,  data->time_to_sleep);
	usleep(data->time_to_sleep * 1000);
	// printf("FIN time sleep de philo n.%d = %d\n\n", philo->philo_id, data->time_to_sleep);
	
	print_action(philo, philo->philo_id, "is sleeping");
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	/*
	Un philosophe meurt s'il n'a pas commencé à manger dans l'intervalle de 
	time_to_die. 
	*/

	printf("%s***** DATA INIT *****\n", GREEN);
	printf("nb_of_philo = %d\n", init->nb_of_philo);
	printf("time_to_die = %d\n", init->time_to_die);
	printf("time_to_eat = %d\n", init->time_to_eat);
	printf("time_to_sleep = %d\n", init->time_to_sleep);
	printf("nb_must_eat = %d\n", init->nb_must_eat);
	printf("time_to_think = %d\n", init->time_to_think);
	printf("****** END DATA INIT ******\n\n%s", RESET);


	// printf("%s time_last_eat = %lld\n%s", BLUE, philo->time_last_eat, RESET);
	// printf("%s time_to_die = %d\n%s", YELLOW, init->time_to_die, RESET);

	if (philo->time_last_eat >= init->time_to_die)
	{
		print_action(philo, philo->philo_id, "died");
		return (-1);
	}
	else
		return (0);

	// mettre fin au programme des le 1er philo died et ne pas afficher les autre died
}

void *thread_routine(void *data)
{
	t_init	*init;
	t_philo	*philo;

	philo = (t_philo*)data;
	init = (t_init*)data;
	
	while (check_if_philo_died(philo, data) == 0)
	{
		if (check_if_philo_died(philo, data) == -1)
			break;
		else
		{
			// action_grab_fork(philo, init); // Segfault
			// action_eat(philo, init);
			// action_drop_fork(philo, init); // Segfault
			action_sleep(philo, data);
			action_think(philo, data);
		}
		
	}
	return (NULL);
}

void	run_routine_philo(t_init *data)
{
	int	i;
	long long time_init;
	
	// j init mon time ici
	time_init = get_time_philo();
	
	// printf("%s time_init = %lld\n%s", BLUE, time_init, RESET);

	// creation des threads par philosophers
	i = data->nb_of_philo - 1;
	while(i >= 0)
	{
		data->philo[i].time_init = time_init;
		// data->philo[i].init_data = data; // utile ? 
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
	data = init_forks(data);
	if (data == NULL)
		return (write_error("Failed to initialize forks"));
	run_routine_philo(data);

	cleanup_forks(data);
	return (0);
}
