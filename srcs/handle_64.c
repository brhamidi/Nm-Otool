/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/01 17:55:13 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/03 15:12:36 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	put_value(const unsigned long int n)
{
	const unsigned int len = ft_nbytes(n, 16);
	ft_putnchar('0', 16 - len);
	ft_putulongnbr(n, 16);
}

char	get_sign(const struct nlist_64 *nlist, const unsigned int acc)
{
	if ((nlist[acc].n_type & N_TYPE) == N_UNDF)
		return ('U');
	if ((nlist[acc].n_type & N_TYPE) == N_ABS)
		return ('A');
	if ((nlist[acc].n_type & N_TYPE) == N_SECT)
		return ('T');
	if ((nlist[acc].n_type & N_TYPE) == N_PBUD)
		return ('D');
	if ((nlist[acc].n_type & N_TYPE) == N_INDR)
		return ('B');
	return ('?');
}

void	display_info(const struct nlist_64 *nlist, const unsigned int acc, const char *strtable)
{
	const char mask = nlist[acc].n_type & N_TYPE;
	const char sign = nlist[acc].n_type & N_EXT ? 
		get_sign(nlist, acc) : ft_tolower(get_sign(nlist, acc));

	if (mask ==  N_UNDF)
		write(1, "                ", 16);
	else
		put_value(nlist[acc].n_value);
	printf(" %c %s\n", sign, strtable + nlist[acc].n_un.n_strx);
}

int	print_sym(void *ptr, const struct symtab_command *symtab, const unsigned int acc)
{
	const struct nlist_64	*nlist = ptr + symtab->symoff;
	const char				*strtable = ptr + symtab->stroff;

	if (acc == symtab->nsyms)
		return (0);
	display_info(nlist, acc, strtable);
	return (print_sym(ptr, symtab, acc + 1));
}

int	load_cmd(void *ptr, const struct load_command *lc, const size_t size,
		const unsigned int acc, const uint32_t ncmds)
{
	if (acc == ncmds)
		return (1);
	if (lc->cmd == LC_SYMTAB)
		return (print_sym(ptr, (struct symtab_command *)lc, 0));
	return (load_cmd(ptr, (void *)lc + lc->cmdsize, size, acc + 1, ncmds));
}

int	handle_64(void *ptr, const size_t file_size)
{
	const struct mach_header_64 * header = (struct mach_header_64 *)ptr;

	if (sizeof(struct mach_header_64) > file_size)
		return (1);
	return (load_cmd(ptr, ptr + sizeof(*header), file_size, 0, header->ncmds));
}
