/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_same.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 19:11:13 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/05 19:19:33 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	put_value(const unsigned long int n, int padd)
{
	const unsigned int len = ft_nbytes(n, 16);

	ft_putnchar('0', padd - len);
	ft_putulongnbr(n, 16);
}

char	get_sign(void)
{
	return ('?');
}
