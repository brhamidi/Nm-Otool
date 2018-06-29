/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 21:06:26 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 21:09:36 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int			section64_mach(t_info *inf, void *curr)
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

int			custum_analyse(const char *file_name, t_info *new)
{
	ft_putstr(file_name);
	ft_putendl(":");
	return (analyse_object(new));
}

int			help_end64(t_info *inf)
{
	if (inf->mode == FULL)
		return (0);
	inf->mode = FULL;
	return (obj_fat64(inf));
}

int			help_ft64(t_info *inf, struct fat_arch *farch, t_info *new)
{
	if (check(inf, farch, sizeof(*farch)))
		return (-1);
	if (rev(farch->cputype, 0,
				sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
		inf->mode = SINGLE;
	new->ptr = inf->ptr +
		rev(farch->offset, 0, sizeof(uint64_t), inf->endian);
	new->end = new->ptr +
		rev(farch->size, 0, sizeof(uint64_t), inf->endian);
	new->file_name = inf->file_name;
	return (0);
}

int			obj_fat64(t_info *inf)
{
	struct fat_header	*fheader;
	struct fat_arch		*farch;
	t_info				new;
	uint32_t			t[2];

	if ((t[0] = -1) && check(inf, inf->ptr, sizeof(*fheader) + sizeof(*farch)))
		return (-1);
	fheader = (struct fat_header *)inf->ptr;
	farch = (struct fat_arch *)(fheader + 1);
	t[1] = rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian);
	while (++t[0] < t[1])
	{
		if (help_ft64(inf, farch + t[0], &new))
			return (-1);
		if (check(inf, new.ptr, 0) || check(inf, new.end, 0))
			return (-1);
		if (inf->mode == SINGLE && rev(farch[t[0]].cputype, 0,
					sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			return (custum_analyse(inf->file_name, &new));
		if (inf->mode == FULL)
			handle_full(inf->file_name, get_cputype(rev(farch[t[0]].cputype,
			0, sizeof(cpu_type_t), inf->endian)), &new, t[1] > 1);
	}
	return (help_end64(inf));
}
