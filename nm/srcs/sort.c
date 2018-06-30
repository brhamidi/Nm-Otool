/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 18:39:44 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/30 14:59:07 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	ft_swap(t_sym *a, t_sym *b)
{
	void	*tmp;

	tmp = a->sym;
	a->sym = b->sym;
	b->sym = tmp;
}

int		predicat64(t_sym *a, t_sym *b, const char *strtable, t_info *inf)
{
	const struct nlist_64	*a_nlist = (struct nlist_64 *)a->sym;
	const struct nlist_64	*b_nlist = (struct nlist_64 *)b->sym;
	uint32_t				an_strx;
	uint32_t				bn_strx;

	an_strx = rev(a_nlist->n_un.n_strx, 0, sizeof(uint32_t), inf->endian);
	bn_strx = rev(b_nlist->n_un.n_strx, 0, sizeof(uint32_t), inf->endian);
	if (check(inf, (void*)strtable + an_strx, 1)
			|| check(inf, (void*)strtable + bn_strx, 1))
		return (-1);
	if (str_safe(inf, (void *)strtable + an_strx)
			|| str_safe(inf, (void *)strtable + bn_strx))
		return (-1);
	if (inf->opt & OPT_R)
	{
		if (ft_strcmp(strtable + an_strx, strtable + bn_strx) == 0)
			return (a_nlist->n_value < b_nlist->n_value);
		return (ft_strcmp(strtable + an_strx, strtable + bn_strx) < 0 ? 1 : 0);
	}
	if (ft_strcmp(strtable + an_strx, strtable + bn_strx) == 0)
		return (a_nlist->n_value > b_nlist->n_value);
	return (ft_strcmp(strtable + an_strx, strtable + bn_strx) > 0 ? 1 : 0);
}

int		basic_sort(t_sym *list, const char *strtable,
		int (*f)(t_sym*, t_sym*, const char *, t_info *), t_info *inf)
{
	t_sym	*tmp;
	int		code;

	while (list)
	{
		tmp = list->next;
		while (tmp)
		{
			code = f(list, tmp, strtable, inf);
			if (code == -1)
				return (-1);
			if (code)
				ft_swap(tmp, list);
			tmp = tmp->next;
		}
		list = list->next;
	}
	return (0);
}
