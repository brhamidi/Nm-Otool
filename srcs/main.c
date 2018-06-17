/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/17 14:53:36 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		get_endianess(t_info *inf)
{
	unsigned int	m;

	if (check(inf, inf->ptr, 8))
		return (-1);
	m = *(unsigned int *)inf->ptr;
	return (m == FAT_CIGAM || m == FAT_CIGAM_64
			|| m == MH_CIGAM_64 || m == MH_CIGAM) ? 1 : 0;
}

int		analyse_file(t_info *inf)
{
	unsigned int		magic;
	int					i;
	const unsigned int	magic_tab[4] = {
		FAT_MAGIC,
		FAT_MAGIC_64,
		MH_MAGIC,
		MH_MAGIC_64
	};
	int (* const func_tab[4])(t_info *) = {
		obj_fat,
		obj_fat64,
		obj,
		obj64
	};

	inf->endian = get_endianess(inf);
	if (inf->endian == -1)
		return (-1);
	magic = *(unsigned int *)inf->ptr;
	i = -1;
	while (++i < 4)
		if (magic_tab[i] == rev(magic, 0, 4, inf->endian))
			return func_tab[i](inf);
	return (-1);
}

int		map_file(const char *filename)
{
	int				fd;
	struct stat		buf;
	int				code;
	t_info			inf;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (2);
	if (fstat(fd, &buf) == -1)
		return (3);
	if ((inf.ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (4);
	inf.end = inf.ptr + buf.st_size;
	inf.list = NULL;
	code = analyse_file(&inf);
	if (close(fd) == -1)
		return (5);
	if (munmap(inf.ptr, buf.st_size))
		return (6);
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
