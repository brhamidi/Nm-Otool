/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:49:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/14 19:04:05 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char	*arch_name[4] = {
	"I386",
	"X86_64",
	"PPC",
	"PPC64"
};

void	put_value(const unsigned long int n, int padd)
{
	const unsigned int len = ft_nbytes(n, 16);

	ft_putnchar('0', padd - len);
	ft_putulongnbr(n, 16);
}

void	display_info(const t_sym *node, const char *strtable)
{
	if (!node)
		return ;
	const struct nlist_64	*list = (struct nlist_64 *)node->sym;

	if ((list->n_type & N_TYPE) == N_UNDF)
		ft_putnchar(' ', 16);
	else
		put_value(list->n_value, 16);
	ft_putchar(' ');
	ft_putchar('X');
	ft_putchar(' ');
	ft_putendl(strtable + list->n_un.n_strx);
	return display_info(node->next, strtable);
}

int		ft_nlist(t_info *inf, void *curr, uint32_t nsyms, uint32_t stroff)
{
	if (!nsyms)
	{
		//display_info(inf->list, inf->ptr + stroff);
		//free_list(inf->list);
		return (0);
	}
	if (check(inf, curr, sizeof(*nlist)))
		return (-1);
	push_front(& inf->list, curr);
	return (ft_nlist(inf, curr + sizeof(*nlist), nsyms - 1, stroff));
}

int		symtab(t_info *inf, void *curr)
{
	struct symtab_command	*sym;
	uint32_t				symoff;
	uint32_t				stroff;
	uint32_t				nsyms;
	const size_t			size_nlist = sizeof(struct nlist);
	const size_t			size_nlist64 = sizeof(struct nlist_64);

	if (check(inf, curr, sizeof(*sym)))
		return (-1);
	sym = (struct symtab_command*)curr;
	symoff = rev(sym->symoff, 0, sizeof(uint32_t), inf->endian);
	stroff = rev(sym->stroff, 0, sizeof(uint32_t), inf->endian);
	nsyms = rev(sym->nsyms, 0, sizeof(uint32_t), inf->endian);
	if (check(inf, inf->ptr + symoff, inf->arch == I386 ? size_nlist : size_nlist64))
		return (-1);
	return (ft_nlist(inf, sym, nsyms, stroff));
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
