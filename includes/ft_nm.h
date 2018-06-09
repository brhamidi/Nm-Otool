/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:31:03 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/09 15:36:03 by bhamidi          ###   ########.fr       */
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

struct	s_sym
{
	void			*sym;
	struct s_sym	*next;
};

int		ranlib(void *ptr, size_t size);
uint64_t reverse(uint64_t x, uint64_t r, size_t size, int little);
int		nm_object(void * ptr, size_t file_size);
void	put_value(const unsigned long int n, int pad);
char	get_sign(void);
int		handle_64(void *ptr, const size_t file_size);
int		handle_32(void *ptr, const size_t file_size);
void	free_list(t_sym *head);
int		push_front(t_sym **head, void *e);
void	basic_sort(t_sym *list, const char *strtable, int (*f)(t_sym*, t_sym*, const char *));
int		predicat(t_sym *a, t_sym *b, const char *strtable);

#endif
