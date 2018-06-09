/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:30:04 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/09 15:35:26 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint64_t reverse(uint64_t x, uint64_t r, size_t size, int little)
{
	if (! little)
		return x;
	if (! (size - 1))
		return r | (x & 0x000000FF);
	return reverse(x >> 8, (r | (x & 0xFF)) << 8, size - 1, little);
}

int		fat_loop(void *ptr, size_t size, int little, uint64_t acc)
{
	const struct fat_header 	*fheader = (struct fat_header *)ptr;
	const uint32_t				narch = fheader->nfat_arch;
	const struct fat_arch		*farch = ptr + sizeof(*fheader) + (sizeof(*farch) * acc);

	if (acc == reverse(narch, 0, sizeof(uint32_t), little))
		return (0);
	if (nm_object(ptr + reverse(farch->offset, 0, sizeof(uint32_t), little),
			reverse(farch->size, 0, sizeof(uint32_t), little)))
		return fat_loop(ptr, size, little, acc + 1);
	return (0);
}

int		fat_loop_64(void *ptr, size_t size, int little, uint64_t acc)
{
	const struct fat_header 	*fheader = (struct fat_header *)ptr;
	const uint32_t				narch = fheader->nfat_arch;
	const struct fat_arch_64	*farch = ptr + sizeof(*fheader) + (sizeof(*farch) * acc);

	if (acc == reverse(narch, 0, sizeof(uint32_t), little))
		return (0);
	if (nm_object(ptr + reverse(farch->offset, 0, sizeof(uint32_t), little),
			reverse(farch->size, 0, sizeof(uint32_t), little)))
		return fat_loop(ptr, size, little, acc + 1);
	return (0);
}

int		nm_object(void * ptr, size_t file_size)
{
	unsigned int magic = * (unsigned int *)ptr;
	char		*ran = (char *)ptr;

	if (magic == MH_MAGIC_64)
		return (handle_64(ptr, file_size));
	if (magic == MH_MAGIC)
		return (handle_32(ptr, file_size));
	if (magic == MH_CIGAM_64)
		ft_putendl("littlen endian x86_64 not handle");
	if (magic == MH_CIGAM)
		ft_putendl("little endian i386 not handle");
	if (! ft_strncmp(ran, RANLIB_MAGIC, 8))
		ranlib(ptr, file_size);
	return (1);
}

int		analyse_file(void *ptr, size_t file_size)
{
	unsigned int magic = * (int *)ptr;

	if (magic == FAT_MAGIC)
		return fat_loop(ptr, file_size, 0, 0);
	if (magic == FAT_CIGAM)
		return fat_loop(ptr, file_size, 1, 0);
	if (magic == FAT_MAGIC_64)
		return fat_loop_64(ptr, file_size, 0, 0);
	if (magic == FAT_CIGAM_64)
		return fat_loop_64(ptr, file_size, 1, 0);
	return (nm_object(ptr, file_size));
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
	code = analyse_file(ptr, buf.st_size);
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
