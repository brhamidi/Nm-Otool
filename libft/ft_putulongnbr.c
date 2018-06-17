/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putulongnbr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 19:16:50 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/17 15:19:24 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putulongnbr(unsigned long int nb, unsigned int base)
{
	const char tab[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};

	if (nb >= base)
	{
		ft_putulongnbr(nb / base, base);
		ft_putulongnbr(nb % base, base);
	}
	else
		ft_putchar(tab[nb]);
}
