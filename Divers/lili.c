void	action_think(t_philo *philo, t_init *init)
{

	check_time_for_philo_to_die(init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;


	if (init->nb_of_philo > 1)
	{
		check_time_for_philo_to_die(init);
		if (check_flag_died(init) || check_flag_all_eat(init))
			return ;
		print_action(init, philo->philo_id, "is thinking");
		usleep(init->time_to_think);
	}
}

void	action_sleep(t_philo *philo, t_init *init)
{

	check_time_for_philo_to_die(init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	
	if (init->nb_of_philo > 1)
	{
		check_time_for_philo_to_die(init);
		if (check_flag_died(init) || check_flag_all_eat(init))
			return ;
		print_action(init, philo->philo_id, "is sleeping");
		usleep(init->time_to_sleep * 1000);
	}
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else if (philo->philo_id % 2 != 0)
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void	action_eat(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is eating");
	pthread_mutex_lock(&(init->flag_all_eat_mutex));
	init->all_eat++;
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));
	pthread_mutex_lock(&(init->flag_all_eat_mutex));
	if (init->nb_must_eat != 0
		&& ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));

	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;

	if (init->nb_of_philo > 1)
	{
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			check_time_for_philo_to_die(init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
		else if (philo->philo_id % 2 != 0)
		{
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			check_time_for_philo_to_die(init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
	}
}

long long	get_time_philo(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_if_philosopher_death(t_init *init, int id)
{
	pthread_mutex_lock(&init->death_printed_mutex);
	if (check_flag_died(init))
	{
		if (init->death_printed == 0)
		{
			init->death_printed++;
			pthread_mutex_lock(&init->write_mutex);
			printf("%lld %d ", init->philo[id - 1].time_last_eat + init->time_to_die - init->philo->time_init, id);
			// printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
			printf("died\n");
			pthread_mutex_unlock(&init->write_mutex);
		}
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	pthread_mutex_unlock(&init->death_printed_mutex);
}


void	print_action(t_init *init, int id, char *str)
{
	print_if_philosopher_death(init, id);
	pthread_mutex_lock(&init->death_printed_mutex);
	if (check_flag_all_eat(init))
	{
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	pthread_mutex_unlock(&init->death_printed_mutex);
	pthread_mutex_lock(&init->death_printed_mutex);
	if (init->death_printed == 0)
	{
		pthread_mutex_lock(&init->write_mutex);
		printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
		printf("%s\n", str);
		pthread_mutex_unlock(&init->write_mutex);
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	else
	{
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
}

int	check_time_for_philo_to_die(t_init *init) // modifié pour verifs tous les philos 
{
	int	i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		if ((get_time_philo() - init->philo[i].time_last_eat) > init->time_to_die)
		{
			pthread_mutex_lock(&(init->flag_died_mutex));
			init->flag_died = 1;
			pthread_mutex_unlock(&(init->flag_died_mutex));
			print_action(init, init->philo[i].philo_id, "died");
			return (1);
		}
		i++;
	}
	return (0);
}

valgrind --tool=helgrind ./philo 5 310 200 1500
==345031== Helgrind, a thread error detector
==345031== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==345031== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==345031== Command: ./philo 5 310 200 1500
==345031== 
==345031== ---Thread-Announcement------------------------------------------
==345031== 
==345031== Thread #2 was created
==345031==    at 0x49A0BA3: clone (clone.S:76)
==345031==    by 0x49A1A9E: __clone_internal (clone-internal.c:83)
==345031==    by 0x490F758: create_thread (pthread_create.c:295)
==345031==    by 0x491027F: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
==345031==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x10A767: init_and_create_threads (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10A7C5: run_routine_philo (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10960B: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031== 
==345031== ---Thread-Announcement------------------------------------------
==345031== 
==345031== Thread #1 is the program's root thread
==345031== 
==345031== ----------------------------------------------------------------
==345031== 
==345031== Possible data race during read of size 8 at 0x4AA61C0 by thread #2
==345031== Locks held: none
==345031==    at 0x109CF4: check_time_for_philo_to_die (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1099AA: action_take_fork (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10A5BA: thread_run (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x490FB42: start_thread (pthread_create.c:442)
==345031==    by 0x49A0BB3: clone (clone.S:100)
==345031== 
==345031== This conflicts with a previous write of size 8 by thread #1
==345031== Locks held: none
==345031==    at 0x10A6F4: init_and_create_threads (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10A7C5: run_routine_philo (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10960B: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==  Address 0x4aa61c0 is 64 bytes inside a block of size 240 alloc'd
==345031==    at 0x484A919: malloc (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x109F6D: init_philo (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1094F0: initialize_data_and_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1095E6: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==  Block was alloc'd by thread #1
==345031== 
-1691401488088 4 died
==345031== ----------------------------------------------------------------
==345031== 
==345031==  Lock at 0x4AA6090 was first observed
==345031==    at 0x4854BFE: pthread_mutex_init (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x10A0D4: init_write_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x109511: initialize_data_and_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1095E6: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==  Address 0x4aa6090 is 80 bytes inside a block of size 248 alloc'd
==345031==    at 0x484A919: malloc (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x109E5C: init_recup_data (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1094CF: initialize_data_and_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1095E6: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==  Block was alloc'd by thread #1
==345031== 
==345031==  Lock at 0x4AA6108 was first observed
==345031==    at 0x4854BFE: pthread_mutex_init (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x10A1C2: init_death_printed_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10956B: initialize_data_and_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1095E6: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==  Address 0x4aa6108 is 200 bytes inside a block of size 248 alloc'd
==345031==    at 0x484A919: malloc (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x109E5C: init_recup_data (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1094CF: initialize_data_and_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1095E6: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==  Block was alloc'd by thread #1
==345031== 
==345031== Possible data race during write of size 8 at 0x4AA6220 by thread #1
==345031== Locks held: none
==345031==    at 0x10A6F4: init_and_create_threads (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10A7C5: run_routine_philo (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10960B: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031== 
==345031== This conflicts with a previous read of size 8 by thread #2
==345031== Locks held: 2, at addresses 0x4AA6090 0x4AA6108
==345031==    at 0x10A9E4: print_if_philosopher_death (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10AA89: print_action (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x109D67: check_time_for_philo_to_die (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1099AA: action_take_fork (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x10A5BA: thread_run (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x490FB42: start_thread (pthread_create.c:442)
==345031==    by 0x49A0BB3: clone (clone.S:100)
==345031==  Address 0x4aa6220 is 160 bytes inside a block of size 240 alloc'd
==345031==    at 0x484A919: malloc (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==345031==    by 0x109F6D: init_philo (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1094F0: initialize_data_and_mutex (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==    by 0x1095E6: main (in /mnt/nfs/homes/abonnefo/Documents/Stud-42/8-Philosophers/philo)
==345031==  Block was alloc'd by thread #1
==345031== 
==345031== 
==345031== Use --history-level=approx or =none to gain increased speed, at
==345031== the cost of reduced accuracy of conflicting-access information
==345031== For lists of detected and suppressed errors, rerun with: -s
==345031== ERROR SUMMARY: 3 errors from 2 contexts (suppressed: 28 from 7)