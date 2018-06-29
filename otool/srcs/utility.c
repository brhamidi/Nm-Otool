/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 15:42:54 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 21:03:24 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void		print_archive(const char *file_name)
{
	ft_putstr("Archive : ");
	ft_putendl(file_name);
}

void		print_name(const char *file_name)
{
	ft_putstr(file_name);
	ft_putendl(":");
}

void		add_off(uint32_t off, uint32_t *taboff)
{
	if (!*taboff)
		*taboff = off;
	else if (off != *taboff)
		return (add_off(off, taboff + 1));
}

int			check(t_info *inf, const void *addr, size_t size)
{
	return ((addr >= inf->ptr && (addr + size) <= inf->end
			&& (addr + size) >= addr) ? 0 : 1);
}

uint64_t	rev(uint64_t x, uint64_t r, size_t size, int endian)
{
	if (!endian)
		return (x);
	if (!(size - 1))
		return (r | (x & 0xFF));
	return (rev(x >> 8, (r | (x & 0xFF)) << 8, size - 1, endian));
}
