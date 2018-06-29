/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:49:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 21:06:38 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int		loop_sect(t_info *inf, void *curr, uint32_t nsects)
{
	struct section	*sect;
	uint64_t		tab[2];

	if (!nsects)
		return (1);
	if (check(inf, curr, sizeof(*sect)))
		return (-1);
	sect = (struct section*)curr;
	if (!ft_strcmp(sect->sectname, SECT_TEXT)
			&& !ft_strcmp(sect->segname, SEG_TEXT))
	{
		ft_putendl("Contents of (__TEXT,__text) section");
		tab[0] = rev(sect->size, 0, sizeof(uint32_t), inf->endian);
		tab[1] = rev(sect->addr, 0, sizeof(uint32_t), inf->endian);
		return (print_text_section(inf, inf->ptr + rev(sect->offset, 0,
					sizeof(uint32_t), inf->endian), 1, tab));
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
