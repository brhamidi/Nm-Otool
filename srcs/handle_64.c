/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/01 17:55:13 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/02 16:22:21 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	print_sym(void *ptr, const struct symtab_command *symtab, const unsigned int acc)
{
	const struct nlist_64	*nlist = ptr + symtab->symoff;
	const char				*string_table = ptr + symtab->stroff;

	if (acc == symtab->nsyms)
		return (0);
	ft_putendl(string_table + nlist[acc].n_un.n_strx);
	return (print_sym(ptr, symtab, acc + 1));
}

int	load_cmd(void *ptr, const struct load_command *lc, const size_t size,
		const unsigned int acc, const uint32_t ncmds)
{
	if (acc == ncmds)
		return (1);
	if (lc->cmd == LC_SYMTAB)
		return (print_sym(ptr, (struct symtab_command *)lc, 1));
	return (load_cmd(ptr, (void *)lc + lc->cmdsize, size, acc + 1, ncmds));
}

int	handle_64(void *ptr, const size_t file_size)
{
	const struct mach_header_64 * header = (struct mach_header_64 *)ptr;

	if (sizeof(struct mach_header_64) > file_size)
		return (1);
	return (load_cmd(ptr, ptr + sizeof(*header), file_size, 0, header->ncmds));
}
