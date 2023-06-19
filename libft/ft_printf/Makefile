# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/29 09:08:40 by abonnefo          #+#    #+#              #
#    Updated: 2023/01/17 11:02:22 by abonnefo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS = ft_printf.c \
	ft_print_%.c \
	ft_print_c.c \
	ft_print_d.c \
	ft_print_p.c \
	ft_print_s.c \
	ft_print_u.c \
	ft_print_x.c \

OBJS = $(SRCS:.c=.o)

AR = ar rcs

RM = rm -f

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

all : $(NAME)

clean : 
	$(RM) $(OBJS) 

fclean : clean 
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
