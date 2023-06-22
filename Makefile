# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/17 10:44:39 by abonnefo          #+#    #+#              #
#    Updated: 2023/06/22 13:02:50 by abonnefo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

CC = gcc

CFLAGS = -Wall #-Wextra -Werror -g3 -g

OBJ_DIR_PHILOSOPHERS = srcs/obj_philosophers

SRCS = srcs/philo.c \
	srcs/utils.c \
	srcs/libft_philo.c \

OBJS = $(SRCS:%.c=$(OBJ_DIR_PHILOSOPHERS)/%.o) \

AR = ar rcs

RM = rm -f

$(OBJ_DIR_PHILOSOPHERS)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[5;36m-gcc *.c Philosophers done\033[0m"
	@echo "\033[1;32m[Make : 'Philosophers' is done]\033[0m"

all : $(NAME)

clean :
	@$(RM) $(OBJS)
	@echo "\033[1;33m[.o] Object files removed\033[0m"

fclean : clean
	@$(RM) $(NAME)
	@echo "\033[1;33m[.a] Binary file removed\033[0m"

re : fclean all

.PHONY: all clean fclean re
