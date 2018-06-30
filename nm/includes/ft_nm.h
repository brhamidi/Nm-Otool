/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:31:03 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/30 14:56:43 by bhamidi          ###   ########.fr       */
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

# define CHARS_OPT	"anpr"
# define DELIMIT	"--"

# define OPT_A		0b00000001
# define OPT_N		0b00000010
# define OPT_P		0b00000100
# define OPT_R		0b00001000

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
	void			*lc;
	uint32_t		ncmds;
	uint8_t			opt;
};

struct				s_sym
{
	void			*sym;
	struct s_sym	*next;
};

int					push_back(t_sym **head, void *e);
int					get_opt(unsigned char *opt, int ac, char **av);
int					ft_nlist(t_info *inf, struct nlist *nlist,
		uint32_t nsyms, uint32_t stroff);
void				display_info(const t_sym *node, t_info *inf,
		const char *strtable);
void				display_info64(const t_sym *node, t_info *inf,
		const char *strtable);
void				add_off(uint32_t off, uint32_t *taboff);
char				get_segment(t_info *inf, uint8_t ordinal, int mode);
char				get_sign_char(t_info *inf,
		const struct nlist *nlist, int mode);
int					error_file(const char *prog_name, const char *file_name);
int					get_endianess(t_info *inf);
int					help_ft64(t_info *inf, t_info *new,
		struct fat_arch_64 *farch, unsigned int i);
int					help_end(t_info *inf);
int					help_end64(t_info *inf);
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
