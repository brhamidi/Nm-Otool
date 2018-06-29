/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 20:02:25 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 20:07:57 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		get_endianess(t_info *inf)
{
	unsigned int	m;

	if (check(inf, inf->ptr, 8))
		return (-1);
	m = *(unsigned int *)inf->ptr;
	return (m == FAT_CIGAM || m == FAT_CIGAM_64
			|| m == MH_CIGAM_64 || m == MH_CIGAM) ? 1 : 0;
}

int		error_file(const char *prog_name, const char *file_name)
{
	ft_putstr_fd(prog_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putendl_fd(": No such file or directory.", 2);
	return (1);
}

char	get_sign_char(t_info *inf, const struct nlist *nlist, int mode)
{
	const uint8_t	type = rev(nlist->n_type, 0, sizeof(uint8_t), inf->endian);
	const uint8_t	sect = rev(nlist->n_sect, 0, sizeof(uint8_t), inf->endian);

	if ((type & N_TYPE) == N_ABS)
		return ('A');
	if ((type & N_TYPE) == N_INDR)
		return ('I');
	if ((type & N_TYPE) == N_UNDF)
	{
		if (nlist->n_value)
			return ('C');
		return ('U');
	}
	if ((type & N_TYPE) == N_SECT)
		return (get_segment(inf, sect, mode));
	return ('?');
}

void	add_off(uint32_t off, uint32_t *taboff)
{
	if (!*taboff)
		*taboff = off;
	else if (off != *taboff)
		return (add_off(off, taboff + 1));
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
