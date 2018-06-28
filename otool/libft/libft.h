/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/04 11:18:26 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/28 18:52:30 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# define BUFF_SIZE 1

# include <string.h>
# include <unistd.h>
# include <stdlib.h>

void				ft_putulongnbr(unsigned long long int nb, unsigned int base);
void				ft_putnchar(char c, int n);
unsigned int		ft_nbytes(const unsigned long int n, int base);
int					ft_atoi(const char *str);
size_t				ft_natoui(const char *str, size_t n);
char				*ft_strrev(char *src);
void				ft_putnbr(int nb);
void				ft_putnbr_fd(int nb, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_putchar(char c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);
void				*ft_memset(void *b, int c, size_t len);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memccpy(void *dst, const void *src, int c, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t len);
size_t				ft_strlen(const char *str);
char				*ft_strcpy(char *dst, const char *src);
char				*ft_strncpy(char *dst, const char *src, size_t len);
char				*ft_strcat(char *dst, const char *src);
int					ft_isprint(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
char				*ft_strncat(char *dst, const char *src, size_t n);
char				*ft_strrchr(const char *str, int c);
char				*ft_strchr(const char *str, int c);
char				*ft_strdup(const char *src);
int					ft_tolower(int c);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_toupper(int c);
void				*ft_memalloc(size_t size);
void				ft_memdel(void **ap);
char				*ft_strnew(size_t size);
void				ft_strdel(char **as);

#endif
