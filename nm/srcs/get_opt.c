/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 13:25:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/30 13:59:22 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_usage(char *str)
{
	ft_putstr("ft_nm: illegal option -- ");
	ft_putendl(str);
	ft_putendl_fd("usage: ft_nm [-arnpr] [file ...]", 2);
	exit(EXIT_FAILURE);
}

int		valid_arg_opt(char *arg)
{
	if (ft_strlen(arg) > 1)
	{
		if (arg[0] != '-')
			return (1);
		arg++;
		while (*arg != '\0')
		{
			if (!ft_strchr(CHARS_OPT, *arg))
				return (1);
			arg++;
		}
		return (0);
	}
	return (1);
}

void	fill_opt(unsigned char *opt, char *arg)
{
	arg++;
	while (*arg != '\0')
	{
		if (*arg == 'a')
			*opt = *opt | OPT_A;
		if (*arg == 'n')
			*opt = *opt | OPT_N;
		if (*arg == 'p')
			*opt = *opt | OPT_P;
		if (*arg == 'r')
			*opt = *opt | OPT_R;
		arg++;
	}
}

int		get_opt(unsigned char *opt, int ac, char **av)
{
	int			i;

	i = 1;
	while (i < ac)
	{
		if (!ft_strcmp(DELIMIT, av[i]))
			return (i);
		if (av[i][0] != '-' || (!ft_strcmp("-", av[i])))
			return (i - 1);
		if (valid_arg_opt(av[i]))
			print_usage(av[i]);
		fill_opt(opt, av[i]);
		i++;
	}
	return (i - 1);
}
