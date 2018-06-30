/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 18:24:50 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/30 14:30:02 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_sym	*create_node(void *elem)
{
	t_sym	*node;

	if (!(node = (t_sym *)malloc(sizeof(t_sym))))
		return (NULL);
	node->sym = elem;
	node->next = NULL;
	return (node);
}

int		free_list(t_sym *head)
{
	t_sym *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
	return (0);
}

int		push_back(t_sym **head, void *e)
{
	t_sym	*ptr;
	t_sym	*new;

	if (!(new = create_node(e)))
		return (1);
	ptr = *head;
	if (!ptr)
		*head = new;
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (0);
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
