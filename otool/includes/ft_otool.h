/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 15:00:47 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/29 21:09:06 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

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

typedef enum		e_mode
{
	CHECK,
	SINGLE,
	FULL
}					t_mode;

typedef enum		e_arch
{
	I386,
	X86_64,
	PPC,
	PPC64
}					t_arch;

struct				s_info
{
	void			*ptr;
	const char		*file_name;
	void			*end;
	int				endian;
	t_arch			arch;
	t_sym			*list;
	t_mode			mode;
};

struct				s_sym
{
	void			*sym;
	struct s_sym	*next;
};

int					section64_mach(t_info *inf, void *curr);
int					loop_sect64(t_info *inf, void *curr, uint32_t nsects);
int					print_text_section(t_info *inf, void *curr,
		uint64_t acc, uint64_t *tab);
void				put_value(const unsigned long int n,
		const unsigned long int padd);
int					get_endianess(t_info *inf);
void				print_archive(const char *file_name);
void				print_name(const char *file_name);
void				add_off(uint32_t off, uint32_t *taboff);
int					analyse_file(t_info *inf);
int					analyse_object(t_info *inf);
int					check(t_info *inf, const void *addr, size_t size);
uint64_t			rev(uint64_t x, uint64_t r, size_t size, int little);
int					obj_fat(t_info *inf);
int					obj_fat64(t_info *inf);
int					obj(t_info *inf);
int					ranlib(t_info *inf);
int					obj64(t_info *inf);
int					custum_analyse(const char *file_name, t_info *new);
void				handle_full(const char *filename,
		const char *cpu_type, t_info *new, int arch);
const char			*get_cputype(cpu_type_t cputype);

#endif
