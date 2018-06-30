/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 19:57:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/30 16:48:57 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		help_end(t_info *inf)
{
	if (inf->mode == FULL)
		return (0);
	inf->mode = FULL;
	return (obj_fat(inf));
}

int		help_ft64(t_info *inf, t_info *new,
		struct fat_arch_64 *farch, unsigned int i)
{
	if (check(inf, farch + i, sizeof(*farch)))
		return (-1);
	if (rev(farch[i].cputype, 0,
				sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
		inf->mode = SINGLE;
	new->ptr = inf->ptr +
		rev(farch[i].offset, 0, sizeof(uint64_t), inf->endian);
	new->end = new->ptr + rev(farch[i].size, 0, sizeof(uint64_t), inf->endian);
	new->file_name = inf->file_name;
	return (0);
}

int		help_end64(t_info *inf)
{
	if (inf->mode == FULL)
		return (0);
	inf->mode = FULL;
	return (obj_fat64(inf));
}

void	display_info(const t_sym *node, t_info *inf, const char *strt)
{
	struct nlist	*list;
	uint8_t			type;

	if (!node || check(inf, node->sym, sizeof(*list)))
		return ;
	list = (struct nlist *)node->sym;
	type = rev(list->n_type, 0, 1, inf->endian);
	if (((type & N_STAB) && !(inf->opt & OPT_A))
		|| (((type & N_TYPE) == N_UNDF) && (inf->opt & OPT_U)))
		return (display_info(node->next, inf, strt));
	if (!(inf->opt & OPT_J))
	{
		if ((type & N_TYPE) == N_UNDF)
			ft_putnchar(' ', 8);
		else
			put_value(rev(list->n_value, 0, sizeof(uint32_t), inf->endian), 8);
		ft_putchar(' ');
		ft_putchar(get_sign(inf, list, 0));
		ft_putchar(' ');
	}
	if (str_safe(inf, strt + rev(list->n_un.n_strx, 0,
					sizeof(uint32_t), inf->endian)))
		return ;
	ft_putendl(strt + rev(list->n_un.n_strx, 0, sizeof(uint32_t), inf->endian));
	return (display_info(node->next, inf, strt));
}

int		ft_nlist(t_info *inf, struct nlist *nlist,
		uint32_t nsyms, uint32_t stroff)
{
	if (!nsyms)
	{
		if (!(inf->opt & OPT_P))
			if (basic_sort(inf->list, inf->ptr + stroff, predicat64, inf) == -1)
				return (free_list(inf->list) + 1);
		display_info(inf->list, inf, inf->ptr + stroff);
		return (free_list(inf->list));
	}
	if (check(inf, nlist, sizeof(*nlist)))
		return (-1);
	push_back(&inf->list, nlist);
	return (ft_nlist(inf, nlist + 1, nsyms - 1, stroff));
}
