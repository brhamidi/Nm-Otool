/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:31:03 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/28 12:20:15 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <stdio.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <ar.h>
# include <mach-o/ranlib.h>

# include "libft.h"

typedef struct s_sym	t_sym;
typedef struct s_info	t_info;

typedef enum	e_mode
{
	CHECK,
	SINGLE,
	FULL
}				t_mode;

typedef enum	e_arch
{
	I386,
	X86_64,
	PPC,
	PPC64
}				t_arch;

struct			s_info
{
	void			*ptr;
	const char		*file_name;
	void			*end;
	int				endian;
	t_arch			arch;
	t_sym			*list;
	t_mode			mode;
	void			*lc;
	uint32_t		ncmds;
};

struct			s_sym
{
	void			*sym;
	struct s_sym	*next;
};

char				get_sign(t_info *inf, const struct nlist *nlist, int mode);
int					str_safe(t_info *inf, const void *str);
int					analyse_file(t_info *inf);
int					analyse_object(t_info *inf);
void				put_value(const uint64_t n, int padd);
int					check(t_info *inf, const void *addr, size_t size);
uint64_t			rev(uint64_t x, uint64_t r, size_t size, int little);
int					obj_fat(t_info *inf);
int					obj_fat64(t_info *inf);
int					obj(t_info *inf);
int					ranlib(t_info *inf);
int					obj64(t_info *inf);
int					free_list(t_sym *head);
int					push_front(t_sym **head, void *e);
int					basic_sort(t_sym *list, const char *strtable,
		int (*f)(t_sym*, t_sym*, const char *, t_info *), t_info *inf);
int					predicat64(t_sym *a, t_sym *b,
		const char *strtable, t_info *inf);

#endif
