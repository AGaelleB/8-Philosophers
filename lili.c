Règles communes
Vous devez faire un programme pour la partie obligatoire et, dans le cas où vous choi-
sissez aussi de faire les bonus, un programme pour la partie bonus. Ces deux programmes
doivent respecter les règles suivantes, communes aux deux parties :
•Les variables globales sont interdites !
•Chaque programme doit prendre les arguments suivants :
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]
◦number_of_philosophers : Le nombre de philosophes, mais aussi le nombre
de fourchettes.
◦time_to_die (en millisecondes) : Si un philosophe n’a pas commencé à manger
time_to_die millisecondes après le début de son précédent repas ou depuis le
début de la simulation, il meurt.
◦time_to_eat (en millisecondes) : Le temps qu’un philosophe prend à manger.
Pendant ce temps, un philosophe doit garder ses deux fourchettes.
◦time_to_sleep (en millisecondes) : Le temps qu’un philosophe passe à dormir.
◦number_of_times_each_philosopher_must_eat (argument optionnel) : Si tous
les philosophes ont mangé au moins number_of_times_each_philosopher_must_eat
fois, la simulation prend fin. Si cet argument n’est pas spécifié, alors la simu-
lation prend fin à la mort d’un philosophe.
•Chaque philosophe se voit assigner un numéro entre 1 et number_of_philosophers.
•Le philosophe numéro 1 est assis à côté du philosophe numéro number_of_philosophers.
Les autres suivent cette logique : philosophe numéro N est assis entre philosophe
numéro N - 1 et philosophe numéro N + 1.
5
Philosophers Qui aurait cru que philosopher serait si mortel ?
Concernant les logs de votre programme :
•Tout changement d’état d’un philosophe doit être formatté comme suit :
◦timestamp_in_ms X has taken a fork
◦timestamp_in_ms X is eating
◦timestamp_in_ms X is sleeping
◦timestamp_in_ms X is thinking
◦timestamp_in_ms X died
Remplacez timestamp_in_ms par le timestamp actuel en millisecondes
et X par le numéro du philosophe.
•Tout message affiché ne doit pas être mélangé avec un autre message.
•Il ne doit pas y avoir plus de 10 ms entre la mort d’un philosophe et l’affichage du
message annonçant sa mort.
•Encore une fois, les philosophes doivent éviter de mourir.
Votre programme ne doit pas avoir de data race.
6
Chapitre V
Partie obligatoire
Nom du pro-
gramme
philo
Fichiers de rendu Makefile, *.h, *.c, dans un dossier philo/
Makefile NAME, all, clean, fclean, re
Arguments number_of_philosophers time_to_die time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]
Fonctions ex-
ternes autorisées
memset, printf, malloc, free, write,
usleep, gettimeofday, pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock
Libft autorisée Non
Description Philosophers avec des threads et des mutex
Les règles spécifiques à la partie obligatoire sont :
•Chaque philosophe doit être représenté par un thread.
•Une fourchette est placée entre chaque paire de philosophes. Cela signifie que, s’il
y a plusieurs philosophes, chaque philosophe a une fourchette à sa gauche et une à
sa droite. S’il n’y a qu’un seul philosophe, il n’y aura donc qu’une seule fourchette
sur la table.
•Afin d’empêcher les philosophes de dupliquer les fourchettes, vous devez protéger
leur état en mémoire avec un mutex pour chacune d’entre elle.



/*mon .h*/

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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

}						t_philo;

typedef struct			s_init
{
	int					nb_of_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_must_eat;
	long long			time_init;
	t_philo				*philo;
	
}						t_init;


/********************************* philo.c ************************************/

/********************************* utils.c ************************************/
int			write_error(char *str);
void		ft_free_tab(char **tab);
long long	get_time(void);
void	print_action(t_init *time, int id, char *str);


/******************************* libft_philo.c ********************************/
int	ft_atoi_philo(char *str);

/********************************** init.c ************************************/
t_init	*init_mutex(t_init *data);
t_init	*init_philo(t_init *data);
t_init	*init_recup_data(t_init *data, int ac, char **av);

/***************************** actions_philos.c ********************************/

void	action_think(t_philo *philo, t_init *data);
void	action_eat(t_philo *philo, t_init	*data);
void	action_drop_fork(t_philo *philo, t_init	*data);
void	action_grab_fork(t_philo *philo, t_init *data);
void	action_sleep(t_philo *philo, t_init	*data);
void	check_if_philo_died(t_philo *philo, t_init *data);

/***************************** routine.c ********************************/

void	*thread_routine(void *data);
void	run_routine_philo(t_init *data);

#endif


/*mes fichier .c*/

#include "../includes/philosophers.h"

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

void	print_action(t_init *init, int id, char *str)
{
	// printf("%sinit->time_init = %lld\n%s", YELLOW, init->time_init, RESET);
	
	printf("%lli ", get_time() - init->time_init);
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


#include "../includes/philosophers.h"

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
	data->nb_must_eat = ft_atoi_philo(av[5]);
	return (data);
}


#include "../includes/philosophers.h"

// timestamp_in_ms X has taken a fork

void	action_think(t_philo *philo, t_init *data)
{
	printf("%sdata->time_init = %lld\n%s", YELLOW, data->time_init, RESET);

	print_action(data, philo->philo_id, "is thinking");
}

void	action_eat(t_philo *philo, t_init	*data)
{
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
	print_action(data, philo->philo_id, "is eating");
	philo->time_last_eat = get_time();
}

void	action_drop_fork(t_philo *philo, t_init	*data)
{
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_unlock(&philo->left_fork_id);
		pthread_mutex_unlock(&philo->right_fork_id);
	}
	else
	{
		pthread_mutex_unlock(&philo->right_fork_id);
		pthread_mutex_unlock(&philo->left_fork_id);
	}
}

void	action_grab_fork(t_philo *philo, t_init *data)
{
	// Philo prend d'abord la fourchette avec le plus petit identifiant
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&philo->left_fork_id);
		pthread_mutex_lock(&philo->right_fork_id);
	}
	else 
	{
		pthread_mutex_lock(&philo->right_fork_id);
		pthread_mutex_lock(&philo->left_fork_id);
	}
	print_action(data, philo->philo_id, "has taken a fork");
	action_eat(philo, data);
	action_drop_fork(philo, data);

}
void	action_sleep(t_philo *philo, t_init	*data)
{
	print_action(data, philo->philo_id, "is sleeping");
}

void	check_if_philo_died(t_philo *philo, t_init *data)
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
	t_philo	*philo;
	philo = (t_philo*)data;

	printf("Debut de la routine\n");
	
	action_think(philo, data);
	action_grab_fork(philo, data);
	action_eat(philo, data);
	action_drop_fork(philo, data);
	action_sleep(philo, data);
	return (NULL);
}

void	run_routine_philo(t_init *data)
{
	int	i;

	// j init mon time ici
	data->time_init = get_time();
	printf("%sdata->time_init = %lld\n%s", BLUE, data->time_init, RESET);


	i = data->nb_of_philo - 1;
	// create threads and pass the specific philosopher to each thread
	while(i >= 0)
	{
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

#include "../includes/philosophers.h"

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
