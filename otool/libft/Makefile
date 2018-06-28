# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/11 15:09:11 by bhamidi           #+#    #+#              #
#    Updated: 2018/06/28 17:59:17 by bhamidi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 		= gcc
NAME	= libft.a
CFLAGS 	= -Wall -Wextra -Werror -O2
SRCS 	= ft_isalnum.c\
		ft_isalpha.c\
		ft_nbytes.c\
		ft_putnchar.c\
		ft_isascii.c\
		ft_putulongnbr.c\
		ft_isdigit.c\
		ft_isprint.c\
		ft_atoi.c\
		ft_memalloc.c\
		ft_memccpy.c	\
		ft_memmove.c	\
		ft_memchr.c		\
		ft_memcmp.c		\
		ft_memcpy.c		\
		ft_memdel.c		\
		ft_memset.c		\
		ft_putchar.c	\
		ft_putchar_fd.c	\
		ft_putendl.c	\
		ft_putendl_fd.c	\
		ft_putnbr.c		\
		ft_putnbr_fd.c	\
		ft_putstr.c		\
		ft_putstr_fd.c	\
		ft_strcat.c		\
		ft_strchr.c		\
		ft_strcmp.c		\
		ft_strcpy.c		\
		ft_strdel.c		\
		ft_strdup.c		\
		ft_strlen.c		\
		ft_strncat.c	\
		ft_strncmp.c	\
		ft_strncpy.c	\
		ft_strnew.c		\
		ft_strrchr.c	\
		ft_strrev.c		\
		ft_tolower.c	\
		ft_toupper.c

OBJ = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@echo "\033[32mlibft linked\033[0m"

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	@rm -f $(OBJ)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
