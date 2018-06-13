/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/13 18:52:36 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const unsigned int magic_tab[4] = {
	FAT_MAGIC,
	FAT_MAGIC_64,
	MH_MAGIC,
	MH_MAGIC_64
};

int (*func_tab[4])(void *, size_t, int) = {
	obj_fat,
	obj_fat64,
	obj,
	obj64
};

uint64_t reverse(uint64_t x, uint64_t r, size_t size, int little)
{
	if (!little)
		return x;
	if (!(size - 1))
		return r | (x & 0xFF);
	return reverse(x >> 8, (r | (x & 0xFF)) << 8, size - 1, little);
}

int		get_endianess(void *ptr, size_t file_size)
{
	unsigned int 	m;

	if (file_size < 8)
		return (-1);
	m = *(int *)ptr;
	return (m == FAT_CIGAM || m == FAT_CIGAM_64
			|| m == MH_CIGAM_64 || m == MH_CIGAM) ? 1 : 0;
}

int		analyse_file(void *ptr, size_t file_size)
{
	const int		endian = get_endianess(ptr, file_size);
	unsigned int	magic = *(int *)ptr;
	int				i = -1;

	if (endian == -1)
		return (1);
	while (++i < 4)
		if (magic_tab[i] == reverse(magic, 0, 4, endian))
			return func_tab[i](ptr, file_size, endian);
	return (1);
}

int		map_file(const char *filename)
{
	int				fd;
	void			*ptr;
	struct stat		buf;
	int				code;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (2);
	if (fstat(fd, &buf) == -1)
		return (3);
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (4);
	code = analyse_file(ptr, buf.st_size);
	if (close(fd) == -1)
		return (5);
	if (munmap(ptr, buf.st_size))
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
