/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 18:24:50 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/18 14:47:42 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_sym *create_node(void *elem)
{
	t_sym	*node;

	if (!(node = (t_sym *)malloc(sizeof(t_sym))))
		return (NULL);
	node->sym = elem;
	node->next = NULL;
	return (node);
}

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
	const uint32_t			an_strx = 
		rev(a_nlist->n_un.n_strx, 0, sizeof(uint32_t), inf->endian);
	const uint32_t			bn_strx = 
		rev(b_nlist->n_un.n_strx, 0, sizeof(uint32_t), inf->endian);

	return (ft_strcmp(strtable + an_strx, strtable + bn_strx) > 0);
}

void	basic_sort(t_sym *list, const char *strtable,
		int (*f)(t_sym*, t_sym*, const char *, t_info *), t_info *inf)
{
	t_sym	*tmp;

	while (list)
	{
		tmp = list->next;
		while (tmp)
		{
			if (f(list, tmp, strtable, inf))
				ft_swap(tmp, list);
			tmp = tmp->next;
		}
		list = list->next;
	}
}

void	free_list(t_sym *head)
{
	t_sym *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

int		push_front(t_sym **head, void *e)
{
	t_sym	*ptr;

	ptr = *head;
	if (!(*head = create_node(e)))
		return (1);
	if (ptr)
		(*head)->next = ptr;
	return (0);
}
