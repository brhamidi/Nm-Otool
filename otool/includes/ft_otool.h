/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 15:00:47 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/28 15:54:39 by bhamidi          ###   ########.fr       */
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
};

struct			s_sym
{
	void			*sym;
	struct s_sym	*next;
};

int					str_safe(t_info *inf, const void *str);
int					analyse_file(t_info *inf);
int					analyse_object(t_info *inf);
int					check(t_info *inf, const void *addr, size_t size);
uint64_t			rev(uint64_t x, uint64_t r, size_t size, int little);
int					obj_fat(t_info *inf);
int					obj_fat64(t_info *inf);
int					obj(t_info *inf);
int					ranlib(t_info *inf);
int					obj64(t_info *inf);

#endif
