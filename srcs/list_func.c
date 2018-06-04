/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 18:24:50 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/04 19:20:10 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_sym *create_node(struct nlist_64 *elem)
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
	struct nlist_64	*tmp;

	tmp = a->sym;
	a->sym = b->sym;
	b->sym = tmp;
}

void	basic_sort(t_sym *list, const char *strtable)
{
	t_sym	*tmp;

	while (list)
	{
		tmp = list->next;
		while (tmp)
		{
			if (ft_strcmp(strtable + list->sym->n_un.n_strx,
						strtable + tmp->sym->n_un.n_strx) > 0)
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

int		push_front(t_sym **head, struct nlist_64 *e)
{
	t_sym	*ptr;

	ptr = *head;
	if (!(*head = create_node(e)))
		return (1);
	if (ptr)
		(*head)->next = ptr;
	return (0);
}
