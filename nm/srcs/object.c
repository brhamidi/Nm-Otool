/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 15:20:10 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 20:08:24 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		ft_nlist64(t_info *inf, struct nlist_64 *nlist,
		uint32_t nsyms, uint32_t stroff)
{
	if (!nsyms)
	{
		if (basic_sort(inf->list, inf->ptr + stroff, predicat64, inf) == -1)
			return (free_list(inf->list) + 1);
		display_info64(inf->list, inf, inf->ptr + stroff);
		return (free_list(inf->list));
	}
	if (check(inf, nlist, sizeof(*nlist)))
		return (-1);
	push_front(&inf->list, nlist);
	return (ft_nlist64(inf, nlist + 1, nsyms - 1, stroff));
}

/*
**	tab: offset array: index --> 0: symoff 1: stroff
*/

int		symtab(t_info *inf, void *curr)
{
	struct symtab_command	*sym;
	uint32_t				offarr[2];
	uint32_t				nsyms;
	const size_t			size_nlist = sizeof(struct nlist);
	const size_t			size_nlist64 = sizeof(struct nlist_64);

	if (check(inf, curr, sizeof(*sym)))
		return (-1);
	sym = (struct symtab_command*)curr;
	offarr[0] = rev(sym->symoff, 0, sizeof(uint32_t), inf->endian);
	offarr[1] = rev(sym->stroff, 0, sizeof(uint32_t), inf->endian);
	nsyms = rev(sym->nsyms, 0, sizeof(uint32_t), inf->endian);
	if (check(inf, inf->ptr + offarr[0],
				inf->arch == I386 ? size_nlist : size_nlist64))
		return (-1);
	if (inf->arch == I386)
		return (ft_nlist(inf, inf->ptr + offarr[0], nsyms, offarr[1]));
	else
		return (ft_nlist64(inf, inf->ptr + offarr[0], nsyms, offarr[1]));
}

int		load(t_info *inf, void *curr, size_t ncmds, size_t acc)
{
	struct load_command	*lc;

	if (acc == rev(ncmds, 0, sizeof(size_t), inf->endian))
		return (-1);
	if (check(inf, curr, sizeof(*lc)))
		return (-1);
	lc = (struct load_command *)curr;
	if (rev(lc->cmd, 0, sizeof(uint32_t), inf->endian) == LC_SYMTAB)
		return (symtab(inf, curr));
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
	inf->lc = inf->ptr + sizeof(*header);
	inf->ncmds = rev(header->ncmds, 0, sizeof(uint32_t), inf->endian);
	return (load(inf, inf->ptr + sizeof(*header), header->ncmds, 0));
}

int		obj64(t_info *inf)
{
	struct mach_header_64	*header;

	if (check(inf, inf->ptr, sizeof(*header)))
		return (-1);
	inf->arch = X86_64;
	header = (struct mach_header_64*)inf->ptr;
	inf->lc = inf->ptr + sizeof(*header);
	inf->ncmds = rev(header->ncmds, 0, sizeof(uint32_t), inf->endian);
	return (load(inf, inf->ptr + sizeof(*header), header->ncmds, 0));
}
