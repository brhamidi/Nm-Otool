/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:56 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/30 14:41:11 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const cpu_type_t	g_cpu_tab[] = {
	CPU_TYPE_VAX,
	CPU_TYPE_MC680x0,
	CPU_TYPE_X86,
	CPU_TYPE_I386,
	CPU_TYPE_X86_64,
	CPU_TYPE_MC98000,
	CPU_TYPE_HPPA,
	CPU_TYPE_ARM,
	CPU_TYPE_SPARC,
	CPU_TYPE_POWERPC,
	CPU_TYPE_I860,
	CPU_TYPE_POWERPC64
};

const char	*g_cpu_name_tab[] = {
	"vax",
	"mc680",
	"i386",
	"i386",
	"x86_64",
	"mc98000",
	"hppa",
	"arm",
	"sparc",
	"ppc",
	"i860",
	"ppc64"
};

const char	*get_cputype(cpu_type_t cputype)
{
	unsigned long i;

	i = -1;
	while (++i < sizeof(g_cpu_name_tab) / sizeof(*g_cpu_name_tab))
		if (g_cpu_tab[i] == cputype)
			return (g_cpu_name_tab[i]);
	return ("?");
}

void		handle_full(const char *filename,
		const char *cpu_type, t_info *new, int arch)
{
	ft_putstr(filename);
	if (arch)
	{
		ft_putstr(" (for architecture ");
		ft_putstr(cpu_type);
		ft_putstr(")");
	}
	ft_putendl(":");
	analyse_object(new);
}

int			help_ft(t_info *inf, t_info *new,
		struct fat_arch *farch, unsigned int i)
{
	if (check(inf, farch + i, sizeof(*farch)))
		return (-1);
	if (rev(farch[i].cputype, 0,
				sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
		inf->mode = SINGLE;
	new->ptr = inf->ptr +
		rev(farch[i].offset, 0, sizeof(uint32_t), inf->endian);
	new->end = new->ptr + rev(farch[i].size, 0, sizeof(uint32_t), inf->endian);
	new->file_name = inf->file_name;
	new->opt = inf->opt;
	return (0);
}

/*
**	t:
**		index 0: i;
**		index 1: narch;
*/

int			obj_fat(t_info *inf)
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
		if (inf->mode == FULL && t[0] != t[1] && t[1] > 1)
			ft_putchar('\n');
		if (help_ft(inf, &new, farch, t[0]) || check(inf, new.ptr, 0)
				|| check(inf, new.end, 0))
			return (-1);
		if (inf->mode == SINGLE && rev(farch[t[0]].cputype, 0,
					sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			return (analyse_object(&new));
		if (inf->mode == FULL)
			handle_full(inf->file_name, get_cputype(rev(farch[t[0]].cputype,
			0, sizeof(cpu_type_t), inf->endian)), &new, t[1] > 1);
	}
	return (help_end(inf));
}

int			obj_fat64(t_info *inf)
{
	struct fat_header	*fheader;
	struct fat_arch_64	*farch;
	t_info				new;
	uint32_t			t[2];

	if ((t[0] = -1) && check(inf, inf->ptr, sizeof(*fheader) + sizeof(*farch)))
		return (-1);
	fheader = (struct fat_header *)inf->ptr;
	farch = (struct fat_arch_64 *)(fheader + 1);
	t[1] = rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian);
	while (++t[0] < t[1])
	{
		if (inf->mode == FULL && t[0] != t[1] && t[1] > 1)
			ft_putchar('\n');
		if (help_ft64(inf, &new, farch, t[0]) || check(inf, new.ptr, 0)
				|| check(inf, new.end, 0))
			return (-1);
		if (inf->mode == SINGLE && rev(farch[t[0]].cputype, 0,
					sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			return (analyse_object(&new));
		if (inf->mode == FULL)
			handle_full(inf->file_name, get_cputype(rev(farch[t[0]].cputype,
			0, sizeof(cpu_type_t), inf->endian)), &new, t[1] > 1);
	}
	return (help_end64(inf));
}
