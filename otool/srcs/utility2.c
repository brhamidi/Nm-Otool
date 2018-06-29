/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 21:03:37 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 21:06:03 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int		get_endianess(t_info *inf)
{
	unsigned int	m;

	if (check(inf, inf->ptr, 8))
		return (-1);
	m = *(unsigned int *)inf->ptr;
	return (m == FAT_CIGAM || m == FAT_CIGAM_64
			|| m == MH_CIGAM_64 || m == MH_CIGAM) ? 1 : 0;
}

int		analyse_object(t_info *inf)
{
	unsigned int		magic;
	int					i;
	const unsigned int	magic_tab[2] = {
		MH_MAGIC, MH_MAGIC_64
	};
	int					(*const func_tab[2])(t_info *) = {
		obj, obj64
	};
	char				*smagic;

	inf->list = NULL;
	if ((inf->endian = get_endianess(inf)) == -1)
		return (-1);
	smagic = (char *)inf->ptr;
	if (!ft_strncmp(smagic, ARMAG, SARMAG))
		return (ranlib(inf));
	magic = *(unsigned int *)inf->ptr;
	i = -1;
	while (++i < 2)
		if (magic_tab[i] == rev(magic, 0, 4, inf->endian))
			return (func_tab[i](inf));
	ft_putendl_fd("The file was not recognized as a valid object file", 2);
	return (-2);
}

void	put_value(const unsigned long int n, const unsigned long int padd)
{
	const unsigned int len = ft_nbytes(n, 16);

	if (len > padd)
		return ;
	ft_putnchar('0', padd - len);
	ft_putulongnbr(n, 16);
	ft_putchar('\t');
}

/*
**	tab:
**		index 0: size
**		index 1: value
*/

int		print_text_section(t_info *inf, void *curr, uint64_t acc, uint64_t *tab)
{
	unsigned char	c;

	if (acc > tab[0])
	{
		ft_putchar('\n');
		return (0);
	}
	if (check(inf, curr, 0))
		return (-1);
	c = *(unsigned char*)curr;
	if (acc == 1)
		put_value(tab[1] + acc - 1, inf->arch == I386 ? 8 : 16);
	if (ft_nbytes(c, 16) == 1)
		ft_putchar('0');
	ft_putulongnbr(c, 16);
	ft_putchar(' ');
	if ((acc % 16 == 0) && acc + 1 <= tab[0])
	{
		ft_putchar('\n');
		if (acc + 1 <= tab[0])
			put_value(tab[1] + acc, inf->arch == I386 ? 8 : 16);
	}
	return (print_text_section(inf, curr + 1, acc + 1, tab));
}

int		loop_sect64(t_info *inf, void *curr, uint32_t nsects)
{
	struct section_64	*sect;
	uint64_t			tab[2];

	if (!nsects)
		return (1);
	if (check(inf, curr, sizeof(*sect)))
		return (-1);
	sect = (struct section_64*)curr;
	if (!ft_strcmp(sect->sectname, SECT_TEXT)
			&& !ft_strcmp(sect->segname, SEG_TEXT))
	{
		ft_putendl("Contents of (__TEXT,__text) section");
		tab[0] = rev(sect->size, 0, sizeof(uint64_t), inf->endian);
		tab[1] = rev(sect->addr, 0, sizeof(uint64_t), inf->endian);
		return (print_text_section(inf, inf->ptr + rev(sect->offset, 0,
					sizeof(uint64_t), inf->endian), 1, tab));
	}
	return (loop_sect64(inf, sect + 1, nsects - 1));
}
