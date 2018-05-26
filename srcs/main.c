/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/05/26 18:30:16 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		mh_dylib(void *ptr, size_t file_size)
{
	ft_putendl("mh_dylib");
	return (0);
}
int		mh_object(void *ptr, size_t file_size)
{
	ft_putendl("mh_object");
	return (0);
}
int		mh_exectue(void *ptr, size_t file_size)
{
	ft_putendl("mh_exectue");
	return (0);
}

int		(*g_file_type_function[11]) (void *ptr, size_t file_size) = 
{
	NULL,
	mh_object,
	mh_exectue,
	NULL,
	NULL,
	NULL,
	mh_dylib
};

int		handle_64(void *ptr, size_t file_size)
{
	struct mach_header_64	*header;

	if (sizeof(struct mach_header_64) > file_size)
		return (1);
	header = (struct mach_header_64 *)ptr;
	if (sizeof(*header) + header->sizeofcmds > file_size)
		return (1);
	return g_file_type_function[header->filetype](ptr, file_size);
}

int		map_file(const char *filename)
{
	int				fd;
	void			*ptr;
	struct stat		buf;
	int				code;

	if ((fd = open(filename, O_RDONLY)) == -1)
	{
		ft_putendl("open() failed");
		return (1);
	}
	if (fstat(fd, &buf) == -1)
	{
		ft_putendl("fstat() failed");
		return (1);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ,
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_putendl("mmap() failed");
		return (1);
	}
	if ((*(unsigned int *)ptr) == MH_MAGIC_64)
		code = handle_64(ptr, buf.st_size);
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
