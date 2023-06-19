# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/29 09:08:40 by abonnefo          #+#    #+#              #
#    Updated: 2023/01/17 10:43:37 by abonnefo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftGNL.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS = get_next_line.c \
	get_next_line_utils.c \

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
