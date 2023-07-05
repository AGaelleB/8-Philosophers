
int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;

	if(ac != 6 && ac != 5)
	{
		free(data);
		return (write_error("Wrong numbers of arguments"));
	}
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2]) || !ft_isdigit_str(av[3]) || !ft_isdigit_str(av[4]) || !ft_isdigit_str(av[5]))
	{
		free(data);
		return (write_error("Failed to use negative value"));
	}
	data = init_recup_data(data, ac, av);
	if (data == NULL)
	{
		free(data);
		return (write_error("Failed to initialize data"));
	}
	if (ac == 6 && (data->nb_must_eat == 0))
	{
		free(data);
		return write(2, "", 1);
	}
	if (ac == 6 && (data->nb_must_eat < 0 || !ft_isdigit_str(av[5])))
	{
		free(data);
		return (write_error("Failed to use negative value"));
	}



	
	data = init_philo(data);
	if (data == NULL)
	{
		free(data);
		return (write_error("Failed to initialize philo"));
	}	
	data = init_mutex(data);
	if (data == NULL)
	{
		free(data);
		return (write_error("Failed to initialize mutex"));
	}
	data = init_write_mutex(data);
	if (data == NULL)
	{
		free(data);
		return (write_error("Failed to initialize write_mutex"));
	}
	data = init_eat_count_mutex(data);
	if (data == NULL)
	{
		free(data);
		return (write_error("Failed to initialize eat_count_mutex"));
	}
	data = init_forks(data);
	if (data == NULL)
	{
		free(data);
		return (write_error("Failed to initialize forks"));
	}
	free(data);
	run_routine_philo(data);
	wait_threads(data);
	cleanup_forks(data);
	cleanup_all_mutex(data);
	return (0);
}
