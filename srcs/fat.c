/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:56 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/17 21:19:45 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int obj_fat(t_info *inf)
{
	struct fat_header	*fheader;
	struct fat_arch		*farch;
	unsigned int		i;
	t_info				new;

	if (check(inf, inf->ptr, sizeof(*fheader) + sizeof(*farch)))
		return (-1);
	fheader = (struct fat_header *)inf->ptr;
	farch = (struct fat_arch *) (fheader + 1);
	i = 0;
	while (i < rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian))
	{
		if (check(inf, farch + i, sizeof(*farch)))
			return (-1);
		new.ptr = inf->ptr + rev(farch[i].offset, 0, sizeof(uint32_t), inf->endian);
		new.end = new.ptr + rev(farch[i].size, 0, sizeof(uint32_t), inf->endian);
		analyse_file(& new);
		i++;
	}
	return (0);
}

int obj_fat64(t_info *inf)
{
	(void)inf;
	return (0);
}
