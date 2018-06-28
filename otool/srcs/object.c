/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:49:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/28 20:18:46 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void		put_value(const unsigned long int n, int padd)
{
	const unsigned int len = ft_nbytes(n, 16);

	ft_putnchar('0', padd - len);
	ft_putulongnbr(n, 16);
	ft_putchar('\t');
}

int		print_text_section(t_info *inf, void *curr, uint64_t size, uint64_t acc, uint64_t value)
{
	unsigned char 	c;

	if (acc > size)
	{
		ft_putchar('\n');
		return (0);
	}
	if (check(inf, curr, 0))
		return (-1);
	c = *(unsigned char*)curr;
	if (acc == 1)
		put_value(value + acc - 1, inf->arch == I386 ? 8 : 16);
	if (ft_nbytes(c, 16) == 1)
		ft_putchar('0');
	ft_putulongnbr(c, 16);
	ft_putchar(' ');
	if ((acc % 16 == 0) && acc + 1 <= size)
	{
		ft_putchar('\n');
		if (acc + 1 <= size)
			put_value(value + acc, inf->arch == I386 ? 8 : 16);
	}
	return (print_text_section(inf, curr + 1, size, acc + 1, value));
}

int		loop_sect64(t_info *inf, void *curr, uint32_t nsects)
{
	struct section_64	*sect;

	if (!nsects)
		return (1);
	if (check(inf, curr, sizeof(*sect)))
		return (-1);
	sect = (struct section_64*)curr;
	if (!ft_strcmp(sect->sectname, SECT_TEXT)
			&& !ft_strcmp(sect->segname, SEG_TEXT))
	{
		ft_putendl("Contents of (__TEXT,__text) section");
		return (print_text_section(inf, inf->ptr + rev(sect->offset, 0,
					sizeof(uint64_t), inf->endian), rev(sect->size, 0, sizeof(uint64_t), inf->endian), 1
					,rev(sect->addr, 0, sizeof(uint64_t), inf->endian)));
	}
	return (loop_sect64(inf, sect + 1, nsects - 1));
}

int		section64_mach(t_info *inf, void *curr)
{
	struct segment_command_64	*seg;
	uint32_t					nsects;

	if (check(inf, curr, sizeof(*seg)))
		return (-1);
	seg = (struct segment_command_64*)curr;
	nsects = rev(seg->nsects, 0, sizeof(uint32_t), inf->endian);
	if (nsects > 0)
		return (loop_sect64(inf, seg + 1, nsects));
	return (1);
}

int		loop_sect(t_info *inf, void *curr, uint32_t nsects)
{
	struct section	*sect;

	if (!nsects)
		return (1);
	if (check(inf, curr, sizeof(*sect)))
		return (-1);
	sect = (struct section*)curr;
	if (!ft_strcmp(sect->sectname, SECT_TEXT)
			&& !ft_strcmp(sect->segname, SEG_TEXT))
	{
		ft_putendl("Contents of (__TEXT,__text) section");
		return (print_text_section(inf, inf->ptr + rev(sect->offset, 0,
					sizeof(uint32_t), inf->endian), 
				rev(sect->size, 0, sizeof(uint32_t), inf->endian), 1
				,rev(sect->addr, 0, sizeof(uint64_t), inf->endian)));
	}
	return (loop_sect(inf, sect + 1, nsects - 1));
}

int		section_mach(t_info *inf, void *curr)
{
	struct segment_command	*seg;
	uint32_t				nsects;

	if (check(inf, curr, sizeof(*seg)))
		return (-1);
	seg = (struct segment_command*)curr;
	nsects = rev(seg->nsects, 0, sizeof(uint32_t), inf->endian);
	if (nsects > 0)
		return (loop_sect(inf, seg + 1, nsects));
	return (1);
}
int		load(t_info *inf, void *curr, size_t ncmds, size_t acc)
{
	struct load_command	*lc;
	int					code;

	if (acc == rev(ncmds, 0, sizeof(size_t), inf->endian))
		return (-1);
	if (check(inf, curr, sizeof(*lc)))
		return (-1);
	lc = (struct load_command *)curr;
	if (rev(lc->cmd, 0, sizeof(uint32_t), inf->endian) == LC_SEGMENT_64
			|| rev(lc->cmd, 0, sizeof(uint32_t), inf->endian) == LC_SEGMENT)
	{
		code = inf->arch == I386 ?
			section_mach(inf, curr) : section64_mach(inf, curr);
		if (code == 0)
			return (0);
	}
	return (load(inf, curr + rev(lc->cmdsize, 0,
					sizeof(uint32_t), inf->endian), ncmds, acc + 1));
}

int		obj(t_info *inf)
{
	struct mach_header	*header;

	if (check(inf, inf->ptr, sizeof(*header)))
		return (-1);
	inf->arch = I386;
	header = (struct mach_header*)inf->ptr;
	return (load(inf, inf->ptr + sizeof(*header), header->ncmds, 0));
}

int		obj64(t_info *inf)
{
	struct mach_header_64	*header;

	if (check(inf, inf->ptr, sizeof(*header)))
		return (-1);
	inf->arch = X86_64;
	header = (struct mach_header_64*)inf->ptr;
	return (load(inf, inf->ptr + sizeof(*header), header->ncmds, 0));
}
