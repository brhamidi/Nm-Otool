/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/03 18:20:38 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int            mh_dylib(void *ptr, size_t file_size)
{
	ft_putendl("mh_dylib");
	(void)ptr;
	(void)file_size;
	return (0);
}
int            mh_object(void *ptr, size_t file_size)
{
	ft_putendl("mh_object");
	(void)ptr;
	(void)file_size;
	return (0);
}
int            mh_exectue(void *ptr, size_t file_size)
{
	ft_putendl("mh_exectue");
	(void)ptr;
	(void)file_size;
	return (0);
}

int            (*g_file_type_function[11]) (void *ptr, size_t file_size) =
{
	NULL,
	mh_object,
	mh_exectue,
	NULL,
	NULL,
	NULL,
	mh_dylib
};

int		ft_nm(void * ptr, size_t file_size)
{
	unsigned int magic = * (unsigned int *)ptr;
	(void)file_size;

	ft_putulongnbr(magic, 16);
	ft_putchar('\n');
	if (magic == FAT_CIGAM_64)
		ft_putendl("reverse Fat binary 64");
	if (magic == FAT_CIGAM)
		ft_putendl("reverse Fat binary");
	if (magic == MH_CIGAM_64)
		ft_putendl(" reverse mach header 64");
	if (magic == MH_CIGAM)
		ft_putendl("reverse mach header");
	if (magic == FAT_MAGIC_64)
		ft_putendl("Fat binary 64");
	if (magic == FAT_MAGIC)
		ft_putendl("Fat binary");
	if (magic == MH_MAGIC_64)
		ft_putendl("mach header 64");
	if (magic == MH_MAGIC)
		ft_putendl("mach header");
	return (0);
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
		return (1);
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (1);
	code = ft_nm(ptr, buf.st_size);
	if (close(fd) == -1)
		return (1);
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
