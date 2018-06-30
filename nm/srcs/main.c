/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/30 16:14:49 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		analyse_object(t_info *inf)
{
	unsigned int		magic;
	int					i;
	const unsigned int	magic_tab[2] = {
		MH_MAGIC, MH_MAGIC_64
	};
	int					(*const func_tab[2])(t_info *) = {
		obj, obj64
	};
	char				*smagic;

	inf->list = NULL;
	if ((inf->endian = get_endianess(inf)) == -1)
		return (-1);
	smagic = (char *)inf->ptr;
	if (!ft_strncmp(smagic, ARMAG, SARMAG))
		return (ranlib(inf));
	magic = *(unsigned int *)inf->ptr;
	i = -1;
	while (++i < 2)
		if (magic_tab[i] == rev(magic, 0, 4, inf->endian))
			return (func_tab[i](inf));
	ft_putendl_fd("The file was not recognized as a valid object file", 2);
	return (-2);
}

int		analyse_file(t_info *inf)
{
	unsigned int		magic;
	int					i;
	const unsigned int	magic_tab[4] = {
		FAT_MAGIC, FAT_MAGIC_64, MH_MAGIC, MH_MAGIC_64
	};
	int					(*const func_tab[4])(t_info *) = {
		obj_fat, obj_fat64, obj, obj64
	};
	char				*smagic;

	inf->list = NULL;
	if ((inf->endian = get_endianess(inf)) == -1)
		return (-1);
	smagic = (char *)inf->ptr;
	if (!ft_strncmp(smagic, ARMAG, SARMAG))
		return (ranlib(inf));
	magic = *(unsigned int *)inf->ptr;
	i = -1;
	while (++i < 4)
		if (magic_tab[i] == rev(magic, 0, 4, inf->endian))
			return (func_tab[i](inf));
	ft_putendl_fd("The file was not recognized as a valid object file", 2);
	return (-2);
}

int		map_file(const char *filename, uint8_t opt)
{
	int				fd;
	struct stat		buf;
	int				code;
	t_info			inf;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (2);
	if (fstat(fd, &buf) == -1)
		return (3);
	if ((inf.ptr = mmap(0, buf.st_size, PROT_READ,
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (4);
	inf.opt = opt;
	inf.end = inf.ptr + buf.st_size;
	inf.mode = CHECK;
	inf.file_name = filename;
	code = analyse_file(&inf);
	if (close(fd) == -1)
		return (5);
	if (munmap(inf.ptr, buf.st_size))
		return (6);
	return (code);
}

int		rec_arg(const int ac, const char **av, const int acc, uint8_t opt)
{
	int code;

	code = 0;
	if (acc == ac)
		return (0);
	ft_putchar('\n');
	ft_putstr(av[acc]);
	ft_putstr(":\n");
	code = map_file(av[acc], opt);
	if (code == 2)
		error_file(av[0], av[acc]);
	if (code)
		return (1 + rec_arg(ac, av, acc + 1, opt));
	return (rec_arg(ac, av, acc + 1, opt));
}

int		main(int ac, char **av)
{
	uint8_t			opt;
	const int		i = get_opt(&opt, ac, av);

	if (ac - i == 1)
	{
		if (map_file("a.out", opt) == 2)
			return (error_file(av[0], "a.out"));
	}
	else if (ac - i == 2)
	{
		if (map_file(av[1 + i], opt) == 2)
			return (error_file(av[0], av[1 + i]));
	}
	else
		return (rec_arg(ac, (const char **)av, 1 + i, opt));
}
