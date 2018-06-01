/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/01 17:55:13 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/01 18:56:06 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		load_cmd(const struct load_command *lc, const size_t file_size,
		const unsigned int acc, const uint32_t ncmds)
{
	if (acc == ncmds)
	{
		ft_putendl_fd("LC_SYMTAB macro not found", 2);
		return (1);
	}
	if (lc->cmd == LC_SYMTAB)
	{
		ft_putendl("match");
		return (0);
	}
	return (load_cmd((void *)lc + lc->cmdsize, file_size, acc + 1, ncmds));
}

int		handle_64(void *ptr, const size_t file_size)
{
	struct mach_header_64	*header;

	if (sizeof(struct mach_header_64) > file_size)
		return (1);
	header = (struct mach_header_64 *)ptr;
	if (sizeof(*header) + header->sizeofcmds > file_size)
		return (1);
	return (load_cmd(ptr + sizeof(*header), file_size, 0, header->ncmds));
}
