/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/05/20 17:51:42 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	map_file(const char *filename)
{
	printf("len -> %zu\n", ft_strlen(filename) );
}

void	rec_arg(const int ac, const char **av, const int acc)
{
	if (acc == ac)
		return ;
	printf("\n%s:\n", av[acc]);
	map_file(av[acc]);
	rec_arg(ac, av, acc + 1);
}

int		main(int ac, char **av)
{
	if (ac == 1)
		map_file("a.out");
	else if (ac == 2)
		map_file(av[1]);
	else
		rec_arg(ac, (const char **) av, 1);
	return (0);
}
