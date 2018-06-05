/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/01 17:55:13 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/05 19:20:11 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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
	ft_putchar(get_sign());
	ft_putchar(' ');
	ft_putendl(strtable + list->n_un.n_strx);
	return display_info(node->next, strtable);
}

int	print_sym(void *ptr, const struct symtab_command *symtab)
{
	struct nlist_64	*nlist = ptr + symtab->symoff;
	char			*strtable = ptr + symtab->stroff;
	unsigned int	i;
	t_sym			*list;

	if (!symtab->nsyms)
		return (1);
	list = NULL;
	i = -1;
	while (++i < symtab->nsyms)
		if (push_front(&list, nlist + i))
		{
			free_list(list);
			return (1);
		}
	basic_sort(list, strtable, & predicat);
	display_info(list, strtable);
	free_list(list);
	return (0);
}

int	load_cmd(void *ptr, const struct load_command *lc, const size_t size,
		const unsigned int acc, const uint32_t ncmds)
{
	t_sym	*list;

	list = NULL;
	if (acc == ncmds)
		return (1);
	if (lc->cmd == LC_SYMTAB)
		return (print_sym(ptr, (struct symtab_command *)lc));
	return (load_cmd(ptr, (void *)lc + lc->cmdsize, size, acc + 1, ncmds));
}

int	handle_64(void *ptr, const size_t file_size)
{
	struct mach_header_64	*header;
	t_sym					*list;

	if (sizeof(struct mach_header_64) > file_size)
		return (1);
	list = NULL;
	header = (struct mach_header_64 *)ptr;
	return (load_cmd(ptr, ptr + sizeof(*header), file_size, 0, header->ncmds));
}
