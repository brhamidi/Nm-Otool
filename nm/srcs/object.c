/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 15:20:10 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/27 17:27:48 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char	*sectTab[3] = {
	SECT_TEXT, SECT_DATA, SECT_BSS
};

const char	sectCharTab[3] = {
	'T', 'D', 'B'
};

char	get_str_sign(const char *sectname)
{
	int i;

	i = -1;
	while (++i < 3)
		if (!ft_strcmp(sectTab[i], sectname))
			return (sectCharTab[i]);
	return ('S');
}

char	segment_match(t_info *inf, void *seg, uint32_t *acc, uint8_t ordinal)
{
	struct segment_command_64	*curr;
	uint32_t					i;
	struct section_64			*sec;

	if (check(inf, seg, sizeof(*curr)))
		return (0);
	curr = (struct segment_command_64*)seg;
	if (ft_strcmp(curr->segname, SEG_TEXT)
			&& ft_strcmp(curr->segname, SEG_DATA))
		return (0);
	i = -1;
	if (check(inf, curr + 1, sizeof(*sec)))
		return (0);
	sec = (struct section_64*) (curr + 1);
	while (++i < rev(curr->nsects, 0, sizeof(curr->nsects), inf->endian))
	{
		if (check(inf, sec + 1, sizeof(*sec)))
			return (0);
		if (*acc == ordinal)
			return (get_str_sign(sec->sectname));
		sec = sec + 1;
		(*acc)++;
	}
	return (0);
}

char	segment_match32(t_info *inf, void *seg, uint32_t *acc, uint8_t ordinal)
{
	struct segment_command		*curr;
	uint32_t					i;
	struct section				*sec;

	if (check(inf, seg, sizeof(*curr)))
		return (0);
	curr = (struct segment_command*)seg;
	if (ft_strcmp(curr->segname, SEG_TEXT)
			&& ft_strcmp(curr->segname, SEG_DATA))
		return (0);
	i = -1;
	if (check(inf, curr + 1, sizeof(*sec)))
		return (0);
	sec = (struct section*) (curr + 1);
	while (++i < rev(curr->nsects, 0, sizeof(curr->nsects), inf->endian))
	{
		if (check(inf, sec + 1, sizeof(*sec)))
			return (0);
		if (*acc == ordinal)
			return (get_str_sign(sec->sectname));
		sec = sec + 1;
		(*acc)++;
	}
	return (0);
}

char	get_segment(t_info *inf, uint8_t ordinal, int mode)
{
	uint32_t			i;
	struct load_command	*lc;
	uint32_t			acc;
	char				c;
	unsigned int 		value = mode ? LC_SEGMENT_64 : LC_SEGMENT;


	acc = 1;
	i = -1;
	lc = (struct load_command*)inf->lc;
	c = 0;
	while (++i < inf->ncmds)
	{
		if (rev(lc->cmd, 0, sizeof(uint32_t), inf->endian) == value)
			c = mode ? segment_match(inf, lc, &acc, ordinal)
				: segment_match32(inf, lc, &acc, ordinal);
		if (c)
			return (c);
		if (check(inf, lc + rev(lc->cmdsize, 0, sizeof(uint32_t), inf->endian),
					sizeof(*lc)))
			return (1);
		lc = (void *)lc + rev(lc->cmdsize, 0, sizeof(uint32_t), inf->endian);
	}
	return ('S');
}

char	get_sign_char(t_info *inf, const struct nlist *nlist, int mode)
{
	if ((nlist->n_type & N_TYPE) == N_ABS)
		return ('A');
	if ((nlist->n_type & N_TYPE) == N_INDR)
		return ('I');
	if ((nlist->n_type & N_TYPE) == N_UNDF)
	{
		if (nlist->n_value)
			return ('C');
		return ('U');
	}
	if ((nlist->n_type & N_TYPE) == N_SECT)
		return get_segment(inf, nlist->n_sect, mode);
	return ('?');
}

char	get_sign(t_info *inf, const struct nlist *nlist, int mode)
{
	char	c;

	c = get_sign_char(inf, nlist, mode);
	if (!(nlist->n_type & N_EXT))
		c = ft_tolower(c);
	return (c);
}

void	display_info64(const t_sym *node, t_info *inf, const char *strtable)
{
	struct nlist_64	*list;
	uint8_t			type;

	if (!node)
		return ;
	if (check(inf, node->sym, sizeof(*list)))
		return ;
	list = (struct nlist_64 *)node->sym;
	type = rev(list->n_type, 0, 1, inf->endian);
	if (type & N_STAB)
		return (display_info64(node->next, inf, strtable));
	if ((type & N_TYPE) == N_UNDF)
		ft_putnchar(' ', 16);
	else
		put_value(rev(list->n_value, 0, sizeof(uint64_t), inf->endian), 16);
	ft_putchar(' ');
	ft_putchar(get_sign(inf, (const struct nlist*)list, 1));
	ft_putchar(' ');
	if (str_safe(inf, strtable + rev(list->n_un.n_strx, 0,
					sizeof(uint32_t), inf->endian)))
		return ;
	ft_putendl(strtable + rev(list->n_un.n_strx, 0,
				sizeof(uint32_t), inf->endian));
	return (display_info64(node->next, inf, strtable));
}

void	display_info(const t_sym *node, t_info *inf, const char *strtable)
{
	struct nlist	*list;
	uint8_t			type;

	if (!node)
		return ;
	if (check(inf, node->sym, sizeof(*list)))
		return ;
	list = (struct nlist *)node->sym;
	type = rev(list->n_type, 0, 1, inf->endian);
	if (type & N_STAB)
		return (display_info(node->next, inf, strtable));
	if ((type & N_TYPE) == N_UNDF)
		ft_putnchar(' ', 8);
	else
		put_value(rev(list->n_value, 0, sizeof(uint32_t), inf->endian), 8);
	ft_putchar(' ');
	ft_putchar(get_sign(inf, list, 0));
	ft_putchar(' ');
	if (str_safe(inf, strtable + rev(list->n_un.n_strx, 0,
					sizeof(uint32_t), inf->endian)))
		return ;
	ft_putendl(strtable + rev(list->n_un.n_strx, 0,
				sizeof(uint32_t), inf->endian));
	return (display_info(node->next, inf, strtable));
}

int		ft_nlist(t_info *inf, struct nlist *nlist,
		uint32_t nsyms, uint32_t stroff)
{
	if (!nsyms)
	{
		if (basic_sort(inf->list, inf->ptr + stroff, predicat64, inf) == -1)
			return (free_list(inf->list) + 1);
		display_info(inf->list, inf, inf->ptr + stroff);
		return (free_list(inf->list));
	}
	if (check(inf, nlist, sizeof(*nlist)))
		return (-1);
	push_front(&inf->list, nlist);
	return (ft_nlist(inf, nlist + 1, nsyms - 1, stroff));
}

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
