# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mterkhoy <mterkhoy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/17 10:25:07 by mterkhoy          #+#    #+#              #
#    Updated: 2022/02/26 14:55:10 by mterkhoy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC 	= gcc
CFLAGS = -Wall -Wextra -Werror

INC_DIR = ./includes
LIB_DIR = ./libft

SRCS =		pipex.c	\
			validation.c	\
			utils.c 	\
			
OBJS = $(addprefix srcs/, $(SRCS:.c=.o))

.c.o:
	$(CC) $(CFLAGS) -I includes -I libft -c $< -o $(<:.c=.o) 

$(NAME): $(OBJS) $(INC_DIR)/pipex.h
		make -s -C $(LIB_DIR)
		$(CC) $(CFLAGS) $(OBJS) -L$(LIB_DIR) -lft -o $(NAME)

all: $(NAME)

clean :
	make clean -s -C $(LIB_DIR)
	rm -rf $(OBJS)

fclean:	clean
	$(RM) libft/libft.a
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
