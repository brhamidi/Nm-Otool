/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:56 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/13 18:49:10 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int obj_fat(void *ptr, size_t size, int endian)
{
	(void)ptr;
	(void)size;
	ft_putnbr(endian);
	ft_putchar('\n');
	ft_putendl("fat");
	return (0);
}

int obj_fat64(void *ptr, size_t size, int endian)
{
	(void)ptr;
	(void)size;
	ft_putnbr(endian);
	ft_putchar('\n');
	ft_putendl("fat 64");
	return (0);
}
