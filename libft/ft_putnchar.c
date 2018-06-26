/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 19:10:26 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/26 17:31:55 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	rec_putnchar(char c, int n, int acc)
{
	if (acc == n)
		return ;
	ft_putchar(c);
	rec_putnchar(c, n, acc + 1);
}

void		ft_putnchar(char c, int n)
{
	rec_putnchar(c, n, 0);
}
