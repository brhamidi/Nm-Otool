/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbytes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 18:08:40 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/28 17:53:44 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	rec_ft_nbytes(const unsigned long int n,
		int base, const unsigned int acc)
{
	return (n < (unsigned long int)base ? acc :
			rec_ft_nbytes(n / base, base, acc + 1));
}

unsigned int		ft_nbytes(const unsigned long int n, int base)
{
	return (rec_ft_nbytes(n, base, 1));
}
