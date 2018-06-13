/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:49:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/13 18:50:39 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int obj(void *ptr, size_t size, int endian)
{
	(void)ptr;
	(void)size;
	ft_putnbr(endian);
	ft_putchar('\n');
	ft_putendl("obj");
	return (0);
}

int obj64(void *ptr, size_t size, int endian)
{
	(void)ptr;
	(void)size;
	ft_putnbr(endian);
	ft_putchar('\n');
	ft_putendl("obj 64");
	return (0);
}
