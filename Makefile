# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: callen <callen@student.42.us.org>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/23 22:04:17 by callen            #+#    #+#              #
#    Updated: 2019/03/27 23:54:24 by callen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DNAM := d_$(NAME)

CC := clang
CFLAGS := -Wall -Wextra -Werror
DFLAGS := -Wall -Wextra -g
INCDIR := includes/
INCFLAGS := -I $(INCDIR) -I libft/$(INCDIR)
LIBFLAGS := -L libft/ -lft
OBJDIR := .obj/
SRCDIR := srcs/
SRC +=	main.c

NORME := $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))
OBJ := $(addprefix $(OBJDIR), $(SRC:.c=.o))

ASAN := -fsanitize=address

.PHONY: all debug clean dclean fclean re tags libft j k asan norme codesize

all: $(NAME)

libft:
	@make -sC libft

$(NAME): libft $(OBJDIR) $(OBJ)
	@$(CC) $(INCFLAGS) $(LIBFLAGS) -o $(NAME) $(OBJ)

j: debug

k: fclean dclean

asan: libft
	@$(CC) $(DFLAGS) $(ASAN) $(INCFLAGS) $(LIBFLAGS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

debug: libft
	@$(CC) $(DFLAGS) $(INCFLAGS) $(LIBFLAGS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

dclean:
	@rm -rf $(DNAM) $(DNAM).dSYM

clean:
	@make -s -C libft/ clean
	@rm -Rf $(OBJDIR)

fclean: clean
	@make -s -C libft/ fclean
	@rm -f $(NAME)

re: fclean all

tags:
	@ctags $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))

$(addprefix $(OBJDIR), %.o): $(addprefix $(SRCDIR), %.c)
	@$(CC) $(INCFLAGS) $(CFLAGS) -o $@ -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

norme:
	@norminette $(NORME)

codesize:
	@cat $(NORME) | ggrep -v '/\*' | wc -l
