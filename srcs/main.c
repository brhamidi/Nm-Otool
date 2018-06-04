/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/04 19:26:40 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		ft_nm(void * ptr, size_t file_size)
{
	unsigned int magic = * (unsigned int *)ptr;

	if (magic == MH_MAGIC_64)
		return (handle_64(ptr, file_size));
	return (255);
}

int		map_file(const char *filename)
{
	int				fd;
	void			*ptr;
	struct stat		buf;
	int				code;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (1);
	if (fstat(fd, &buf) == -1)
		return (2);
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (3);
	code = ft_nm(ptr, buf.st_size);
	if (close(fd) == -1)
		return (4);
	if (munmap(ptr, buf.st_size))
		return (5);
	return (code);
}

int		rec_arg(const int ac, const char **av, const int acc)
{
	if (acc == ac)
		return (0);
	ft_putchar('\n');
	ft_putstr(av[acc]);
	ft_putstr(":\n");
	if (map_file(av[acc]))
		return (1 + rec_arg(ac, av, acc + 1));
	return (rec_arg(ac, av, acc + 1));
}

int		main(int ac, char **av)
{
	if (ac == 1)
		return (map_file("a.out"));
	if (ac == 2)
		return (map_file(av[1]));
	return (rec_arg(ac, (const char **)av, 1));
}
