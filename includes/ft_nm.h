/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:31:03 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/18 15:19:22 by bhamidi          ###   ########.fr       */
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

# include "libft.h"

# define RANLIB_MAGIC	"!<arch>\n"

typedef struct s_sym	t_sym;
typedef struct s_info	t_info;

typedef enum e_mode {
	CHECK,
	SINGLE,
	FULL
}			t_mode;


typedef enum e_arch {
	I386,
	X86_64,
	PPC,
	PPC64
}			t_arch;

struct s_info
{
	void		*ptr;
	const char	*file_name;
	void		*end;
	int			endian;
	t_arch		arch;
	t_sym		*list;
	t_mode		mode;
};

struct	s_sym
{
	void			*sym;
	struct s_sym	*next;
};

int			analyse_file(t_info *inf);
void		put_value(const uint64_t n, int padd);
int			check(t_info *inf, void *addr, size_t size);
uint64_t	rev(uint64_t x, uint64_t r, size_t size, int little);
int 		obj_fat(t_info *inf);
int 		obj_fat64(t_info *inf);
int 		obj(t_info *inf);
int			obj64(t_info *inf);
void		free_list(t_sym *head);
int			push_front(t_sym **head, void *e);
void		basic_sort(t_sym *list, const char *strtable,
		int (*f)(t_sym*, t_sym*, const char *, t_info *), t_info *inf);
int			predicat64(t_sym *a, t_sym *b, const char *strtable, t_info *inf);

#endif
