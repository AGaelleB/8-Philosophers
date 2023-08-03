
long long int	ft_atoi_philo(char *str)
{
	int	i;
	int	sign;
	long long int	num;
	
	i = 0;
	sign = 1;
	num = 0;

	int nb = 0;
	while (str[nb] >= '0' && str[nb] <= '9')
		nb++;
	if (nb > 10)
		return (0);
	
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (sign * num);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isdigit_str(char *str)
{
	if (*str == '-')
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	check_before_initialize(int ac, char **av)
{
	if (ac != 6 && ac != 5)
	{
		write_error("Wrong numbers of arguments");
		return (1);
	}
	if (ft_atoi_philo(av[2]) > INT_MAX || ft_atoi_philo(av[3]) > INT_MAX
		|| ft_atoi_philo(av[4]) > INT_MAX
		|| (ac == 6 && ft_atoi_philo(av[5]) > INT_MAX))
	{
		write_error("Numbers must not exceed INT MAX value ");
		return (1);
	}
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2])
		|| !ft_isdigit_str(av[3]) || !ft_isdigit_str(av[4])
		|| (ac == 6 && !ft_isdigit_str(av[5])))
	{
		write_error(" value");
		return (1);
	}
	if (ac == 6 && ft_atoi_philo(av[5]) == 0)
	{
		write_error("Number of meals must be > 0");
		return (1);
	}
	return (0);
}
