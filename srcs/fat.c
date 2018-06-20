/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:56 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/20 16:14:38 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char            *get_cputype(cpu_type_t cputype)
{
	if (cputype == CPU_TYPE_VAX)
		return ("vax");
	else if (cputype == CPU_TYPE_MC680x0)
		return ("mc680");
	else if (cputype == CPU_TYPE_X86 || cputype == CPU_TYPE_I386)
		return ("i386");
	else if (cputype == CPU_TYPE_X86_64)
		return ("x86_64");
	else if (cputype == CPU_TYPE_MC98000)
		return ("mc98000");
	else if (cputype == CPU_TYPE_HPPA)
		return ("hppa");
	else if (cputype == CPU_TYPE_ARM)
		return ("arm");
	else if (cputype == CPU_TYPE_SPARC)
		return ("sparc");
	else if (cputype == CPU_TYPE_I860)
		return ("i860");
	else if (cputype == CPU_TYPE_POWERPC)
		return ("ppc");
	else if (cputype == CPU_TYPE_POWERPC64)
		return ("ppc64");
	else
		return ("?");
}

int obj_fat(t_info *inf)
{
	struct fat_header	*fheader;
	struct fat_arch		*farch;
	unsigned int		i;
	t_info				new;

	if (check(inf, inf->ptr, sizeof(*fheader) + sizeof(*farch)))
		return (-1);
	fheader = (struct fat_header *)inf->ptr;
	farch = (struct fat_arch *) (fheader + 1);
	i = 0;
	while (i < rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian))
	{
		if (inf->mode == FULL && i != rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian))
			ft_putchar('\n');
		if (check(inf, farch + i, sizeof(*farch)))
			return (-1);
		if (rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			inf->mode = SINGLE;
		new.ptr = inf->ptr + rev(farch[i].offset, 0, sizeof(uint32_t), inf->endian);
		new.end = new.ptr + rev(farch[i].size, 0, sizeof(uint32_t), inf->endian);
		if (inf->mode == SINGLE && rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			return analyse_file(& new);
		if (inf->mode == FULL)
		{
			ft_putstr(inf->file_name);
			ft_putstr(" (for architecture ");
			ft_putstr(get_cputype(rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian)));
			ft_putendl("):");
			analyse_file(& new);
		}
		i++;
	}
	if (inf->mode == CHECK)
		return (-1);
	if (inf->mode == FULL)
		return (0);
	inf->mode = FULL;
	return obj_fat(inf);
}

int obj_fat64(t_info *inf)
{
	struct fat_header	*fheader;
	struct fat_arch_64	*farch;
	unsigned int		i;
	t_info				new;

	if (check(inf, inf->ptr, sizeof(*fheader) + sizeof(*farch)))
		return (-1);
	fheader = (struct fat_header *)inf->ptr;
	farch = (struct fat_arch_64 *) (fheader + 1);
	i = 0;
	while (i < rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian))
	{
		if (inf->mode == FULL && i != rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian))
			ft_putchar('\n');
		if (check(inf, farch + i, sizeof(*farch)))
			return (-1);
		if (rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			inf->mode = SINGLE;
		new.ptr = inf->ptr + rev(farch[i].offset, 0, sizeof(uint64_t), inf->endian);
		new.end = new.ptr + rev(farch[i].size, 0, sizeof(uint64_t), inf->endian);
		if (inf->mode == SINGLE && rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			return analyse_file(& new);
		if (inf->mode == FULL)
		{
			ft_putstr(inf->file_name);
			ft_putstr(" (for architecture ");
			ft_putstr(get_cputype(rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian)));
			ft_putendl("):");
			analyse_file(& new);
		}
		i++;
	}
	if (inf->mode == CHECK)
		return (-1);
	if (inf->mode == FULL)
		return (0);
	inf->mode = FULL;
	return obj_fat(inf);
}
