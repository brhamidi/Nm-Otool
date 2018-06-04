/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/01 17:55:13 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/04 19:54:51 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	put_value(const unsigned long int n)
{
	const unsigned int len = ft_nbytes(n, 16);

	ft_putnchar('0', 16 - len);
	ft_putulongnbr(n, 16);
}

char	get_sign(const struct nlist_64 *nlist)
{
	if ((nlist->n_type & N_TYPE) == N_ABS)
		return ('A');
	if ((nlist->n_type & N_TYPE) == N_UNDF)
		return ('U');
	if (!(nlist->n_sect == NO_SECT))
		return ('T');
	return ('?');
}

void	display_info(const t_sym *node, const char *strtable)
{
	if (!node)
		return ;
	const struct nlist_64	*list = (struct nlist_64 *)node->sym;
	const char 				sign = get_sign(list);

	if ((list->n_type & N_TYPE) == N_UNDF)
		write(1, "                ", 16);
	else
		put_value(list->n_value);
	ft_putchar(' ');
	ft_putchar(sign);
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

	list = NULL;
	i = -1;
	while (++i < symtab->nsyms)
		if (push_front(&list, nlist + i))
		{
			free_list(list);
			return (1);
		}
	basic_sort(list, strtable);
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
