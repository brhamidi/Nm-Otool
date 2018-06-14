/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:56 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/14 15:06:17 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int obj_fat(t_info *inf)
{
	ft_putnbr(inf->endian);
	ft_putchar('\n');
	ft_putendl("fat");
	return (0);
}

int obj_fat64(t_info *inf)
{
	ft_putnbr(inf->endian);
	ft_putchar('\n');
	ft_putendl("fat 64");
	return (0);
}
