/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 12:19:43 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/28 13:01:54 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char	*sectTab[3] = {
	SECT_TEXT, SECT_DATA, SECT_BSS
};

const char	sectCharTab[3] = {
	'T', 'D', 'B'
};

char	get_str_sign(const char *sectname, const char *segname)
{
	int i;

	i = -1;
	if (ft_strcmp(segname, SEG_TEXT) && ft_strcmp(segname, SEG_DATA))
		return ('S');
	while (++i < 3)
		if (!ft_strcmp(sectTab[i], sectname))
			return (sectCharTab[i]);
	return ('S');
}

char	segment_match(t_info *inf, void *seg, uint32_t *acc, uint8_t ordinal)
{
	struct segment_command_64	*curr;
	uint32_t					i;
	struct section_64			*sec;

	if (check(inf, seg, sizeof(*curr)))
		return (0);
	curr = (struct segment_command_64*)seg;
	i = -1;
	if (check(inf, curr + 1, sizeof(*sec)))
		return (0);
	sec = (struct section_64*) (curr + 1);
	while (++i < rev(curr->nsects, 0, sizeof(curr->nsects), inf->endian))
	{
		if (check(inf, sec + 1, sizeof(*sec)))
			return (0);
		if (*acc == ordinal)
			return (get_str_sign(sec->sectname, sec->segname));
		sec = sec + 1;
		(*acc)++;
	}
	return (0);
}

char	segment_match32(t_info *inf, void *seg, uint32_t *acc, uint8_t ordinal)
{
	struct segment_command		*curr;
	uint32_t					i;
	struct section				*sec;

	if (check(inf, seg, sizeof(*curr)))
		return (0);
	curr = (struct segment_command*)seg;
	i = -1;
	if (check(inf, curr + 1, sizeof(*sec)))
		return (0);
	sec = (struct section*) (curr + 1);
	while (++i < rev(curr->nsects, 0, sizeof(curr->nsects), inf->endian))
	{
		if (check(inf, sec + 1, sizeof(*sec)))
			return (0);
		if (*acc == ordinal)
			return (get_str_sign(sec->sectname, sec->segname));
		sec = sec + 1;
		(*acc)++;
	}
	return (0);
}

char	get_segment(t_info *inf, uint8_t ordinal, int mode)
{
	uint32_t			i;
	struct load_command	*lc;
	uint32_t			acc;
	char				c;
	unsigned int 		value = mode ? LC_SEGMENT_64 : LC_SEGMENT;

	acc = 1;
	i = -1;
	lc = (struct load_command*)inf->lc;
	c = 0;
	while (++i < inf->ncmds)
	{
		if (rev(lc->cmd, 0, sizeof(uint32_t), inf->endian) == value)
			c = mode ? segment_match(inf, lc, &acc, ordinal) : segment_match32(inf, lc, &acc, ordinal);
		if (c)
			return (c);
		if (check(inf, (void*)lc + rev(lc->cmdsize, 0, sizeof(uint32_t), inf->endian), sizeof(*lc)))
			return (0);
		lc = (void *)lc + rev(lc->cmdsize, 0, sizeof(uint32_t), inf->endian);
	}
	return ('S');
}

char	get_sign_char(t_info *inf, const struct nlist *nlist, int mode)
{
	const uint8_t	type = rev(nlist->n_type, 0, sizeof(uint8_t), inf->endian);
	const uint8_t	sect = rev(nlist->n_sect, 0, sizeof(uint8_t), inf->endian);

	if ((type & N_TYPE) == N_ABS)
		return ('A');
	if ((type & N_TYPE) == N_INDR)
		return ('I');
	if ((type & N_TYPE) == N_UNDF)
	{
		if (nlist->n_value)
			return ('C');
		return ('U');
	}
	if ((type & N_TYPE) == N_SECT)
		return (get_segment(inf, sect, mode));
	return ('?');
}

char	get_sign(t_info *inf, const struct nlist *nlist, int mode)
{
	char	c;

	c = get_sign_char(inf, nlist, mode);
	if (!(rev(nlist->n_type, 0, sizeof(nlist->n_type), inf->endian) & N_EXT))
		c = ft_tolower(c);
	return (c);
}
