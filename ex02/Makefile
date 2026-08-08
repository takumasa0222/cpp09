# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/07 04:23:07 by tamatsuu          #+#    #+#              #
#    Updated: 2026/08/08 10:25:23 by tamatsuu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = PmergeMe
CC = c++
INCLUDES = -I ./
CFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS    =   main.cpp \
			PmergeMe.cpp
OBJS    = $(SRCS:.cpp=.o)
LIBDIR  = 
LIBS    = 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBDIR) $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
all: $(NAME)

clean: 
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 