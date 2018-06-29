/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ranlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 15:14:59 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 21:00:18 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int		rec_symtab(t_info *inf, struct ranlib *ar, int acc, uint32_t *off_tab)
{
	if (!acc)
		return (0);
	if (check(inf, ar, sizeof(*ar)))
		return (1);
	add_off(ar->ran_off, off_tab);
	return (rec_symtab(inf, ar + 1, acc - 1, off_tab));
}

int		get_noff(uint32_t *tab, size_t len, int acc)
{
	if (!len || !*tab)
		return (acc);
	return (get_noff(tab + 1, len - 1, acc + 1));
}

int		nsymbol(t_info *inf, struct ranlib *ar, int acc)
{
	uint32_t	*offset_tab;
	int			nsym;

	offset_tab = (uint32_t *)malloc(acc * sizeof(uint32_t));
	ft_memset((void *)offset_tab, 0, acc * sizeof(uint32_t));
	if (rec_symtab(inf, ar, acc, offset_tab))
		return (-1);
	nsym = get_noff(offset_tab, acc, 0);
	free(offset_tab);
	return (nsym);
}

int		rec_arr(t_info *inf, struct ar_hdr *ar, int acc)
{
	t_info	new;

	if (!acc)
		return (0);
	if (check(inf, ar, sizeof(*ar)))
		return (1);
	if (check(inf, ar, sizeof(*ar) + ft_natoui(ar->ar_name + 3, 13)))
		return (1);
	new.ptr = (void *)(ar + 1) + ft_natoui(ar->ar_name + 3, 13);
	new.end = (void *)(ar + 1) + ft_natoui(ar->ar_size, 10);
	new.file_name = (char *)ar + 1;
	ft_putstr(inf->file_name);
	ft_putchar('(');
	ft_putstr((char *)(ar + 1));
	ft_putendl("):");
	if (check(inf, new.ptr, 0) || check(inf, new.end, 0))
		return (-1);
	analyse_object(&new);
	return (rec_arr(inf, (void *)(ar + 1) +
				ft_natoui(ar->ar_size, 10), acc - 1));
}

int		ranlib(t_info *inf)
{
	struct ar_hdr	*custom;
	size_t			len;
	void			*symtab;
	int				nsyms;

	if (check(inf, inf->ptr + SARMAG, sizeof(struct ar_hdr)))
		return (1);
	custom = (struct ar_hdr *)(inf->ptr + SARMAG);
	len = ft_natoui(custom->ar_name + 3, 13);
	symtab = (void *)(custom + 1) + len;
	if (check(inf, symtab, 4) || check(inf, symtab + 4,
				(*(int *)symtab / 8) * sizeof(struct ranlib)))
		return (-1);
	nsyms = nsymbol(inf, symtab + 4, *(int *)symtab / 8);
	if (nsyms == -1)
		return (1);
	if (check(inf, (void *)(custom + 1) + ft_natoui(custom->ar_size, 10),
				nsyms * sizeof(struct ar_hdr)))
		return (-1);
	return (rec_arr(inf, (void *)(custom + 1) +
				ft_natoui(custom->ar_size, 10), nsyms));
}
