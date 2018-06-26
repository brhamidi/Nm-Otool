# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/06/26 18:54:30 by bhamidi           #+#    #+#              #
#    Updated: 2018/06/26 19:04:02 by bhamidi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PROG1_PATH=./nm
PROG2_PATH=./otool

all:
	@make -C $(PROG1_PATH)
	@make -C $(PROG2_PATH)

clean:
	@make clean -C $(PROG1_PATH)
	@make clean -C $(PROG2_PATH)

fclean:
	@make fclean -C $(PROG1_PATH)
	@make fclean -C $(PROG2_PATH)

re:
	@make re -C $(PROG2_PATH)
	@make re -C $(PROG2_PATH)

.PHONY: all clean fclean re
