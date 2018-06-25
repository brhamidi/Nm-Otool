/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ranlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 15:14:59 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/25 20:40:23 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	ft_putnstr(const char *str, size_t n)
{
	if (! *str || ! n)
		return;
	ft_putchar(*str);
	ft_putnstr(str + 1, n - 1);
}

void	add_off(uint32_t off, uint32_t *taboff)
{
	if (! *taboff)
		*taboff = off;
	else if (off != *taboff)
		return add_off(off, taboff + 1);
}

int		rec_symtab(struct ranlib *ar, int acc, uint32_t *offset_tab)
{
	if (!acc)
		return (0);
	add_off(ar->ran_off, offset_tab);
	return rec_symtab(ar + 1, acc - 1, offset_tab);
}

int		get_noff(uint32_t *tab, size_t len, int acc)
{
	if (!len || ! *tab)
		return (acc);
	return get_noff(tab + 1, len - 1, acc + 1);
}

int		nsymbol(struct ranlib *ar, int acc)
{
	uint32_t	*offset_tab;
	int			nsym;

	offset_tab = (uint32_t *)malloc(acc * sizeof(uint32_t));
	ft_memset((void *)offset_tab, 0, acc * sizeof(uint32_t));
	rec_symtab(ar, acc, offset_tab);
	nsym = get_noff(offset_tab, acc, 0);
	free(offset_tab);
	return (nsym);
}

int		rec_arr(t_info *inf, struct ar_hdr *ar, int acc)
{
	t_info	new;

	if (!acc)
		return (0);
	ft_putchar('\n');
	new.ptr = (void *)(ar + 1) + ft_atoi(ar->ar_name + 3);
	new.end = new.ptr + ft_atoi(ar->ar_size);
	new.file_name = (char *)ar + 1;
	ft_putstr(inf->file_name);
	ft_putchar('(');
	ft_putstr((char *)(ar + 1));
	ft_putendl("):");
	printf("inf->end: %p\n", inf->end);
	printf("new->end: %p\n", new.end);
	if (check(inf, new.ptr, 0)) //TODO verif ptr.end
		return (-1);
	analyse_object(& new);
	return rec_arr(inf, (void *)(ar + 1) + ft_atoi(ar->ar_size), acc - 1);
}

int		ranlib(t_info *inf)
{
	struct ar_hdr	*custom;
	size_t 			len;
	void			*symtab;
	int				nsyms;

	if (check(inf, inf->ptr + SARMAG, sizeof(struct ar_hdr)))
		return (1);
	custom = (struct ar_hdr *)(inf->ptr + SARMAG);
	len = ft_atoi(custom->ar_name + 3);
	symtab =  (void *)(custom + 1) + len;
	if (check(inf, symtab, 4) || check(inf, symtab + 4,
				(*(int *)symtab / 8) * sizeof(struct ranlib)))
		return (-1);
	nsyms = nsymbol(symtab + 4, *(int *)symtab / 8);
	if (check(inf, (void *)(custom + 1) + ft_atoi(custom->ar_size),
				nsyms * sizeof(struct ar_hdr)))
		return (-1);
	return rec_arr(inf, (void *)(custom + 1) + ft_atoi(custom->ar_size), nsyms);
}
