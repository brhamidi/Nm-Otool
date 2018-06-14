/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 15:42:54 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/14 17:32:14 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int			check(t_info *inf, void *addr, size_t size)
{
	return ((addr >= inf->ptr && (addr + size) < inf->end
			&& (addr + size) > addr) ? 0 : 1);
}

uint64_t	rev(uint64_t x, uint64_t r, size_t size, int endian)
{
	if (!endian)
		return (x);
	if (!(size - 1))
		return (r | (x & 0xFF));
	return (rev(x >> 8, (r | (x & 0xFF)) << 8, size - 1, endian));
}
