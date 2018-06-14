/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:49:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/14 18:21:21 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char	*arch_name[4] = {
	"I386",
	"X86_64",
	"PPC",
	"PPC64"
};

int		ft_nlist(t_info *inf, void *curr, uint32_t nsyms, uint32_t stroff)
{
	struct nlist		*nlist;

	if (!nsyms)
		return (0);
	if (check(inf, curr, sizeof(*nlist)))
		return (-1);
	ft_putendl("sym");
	return (ft_nlist(inf, curr + sizeof(*nlist), nsyms - 1, stroff));
}

int		ft_nlist64(t_info *inf, void *curr, uint32_t nsyms, uint32_t stroff)
{
	struct nlist_64		*nlist;

	if (!nsyms)
		return (0);
	if (check(inf, curr, sizeof(*nlist)))
		return (-1);
	ft_putendl("sym");
	return (ft_nlist64(inf, curr + sizeof(*nlist), nsyms - 1, stroff));
}

int		symtab(t_info *inf, void *curr)
{
	struct symtab_command	*sym;
	uint32_t				symoff;
	uint32_t				stroff;

	if (check(inf, curr, sizeof(*sym)))
		return (-1);
	sym = (struct symtab_command*)curr;
	symoff = rev(sym->symoff, 0, sizeof(uint32_t), inf->endian);
	stroff = rev(sym->stroff, 0, sizeof(uint32_t), inf->endian);
	if ((inf->arch == X86_64 || inf->arch == PPC64) &&
			!check(inf, inf->ptr + symoff, sizeof(struct nlist_64)))
		return (ft_nlist64(inf, sym, rev(sym->nsyms, 0,
						sizeof(uint32_t), inf->endian), stroff));
	if (check(inf, inf->ptr + symoff, sizeof(struct nlist)))
		return (-1);
	return (ft_nlist(inf, sym, rev(sym->nsyms, 0, sizeof(uint32_t),
					inf->endian), stroff));

}

int 	load(t_info *inf, void *curr, size_t ncmds, size_t acc)
{
	struct load_command	*lc;

	if (acc == rev(ncmds, 0, sizeof(size_t), inf->endian))
		return (-1);
	if (check(inf, curr, sizeof(*lc)))
		return (-1);
	lc = (struct load_command *)curr;
	if (rev(lc->cmd, 0, sizeof(uint32_t), inf->endian) == LC_SYMTAB)
		return (symtab(inf, curr));
	return (load(inf, curr + lc->cmdsize, ncmds, acc + 1));
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
	return load(inf, inf->ptr + sizeof(*header), header->ncmds, 0);
}
