/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 15:42:54 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/17 15:24:20 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	put_value(const uint64_t n, int padd)
{
	const unsigned int len = ft_nbytes(n, 16);

	(void)len;
	(void)padd;
	ft_putnchar('0', padd - len);
	ft_putulongnbr(n, 16);
}

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
