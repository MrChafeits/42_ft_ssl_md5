# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: callen <callen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/16 21:01:37 by callen            #+#    #+#              #
#    Updated: 2019/02/20 20:02:55 by callen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DNAM := d_ft_ssl
CC := clang
CFLAGS := -Wall -Wextra -Werror
DFLAGS := -Wall -Wextra -g
INCDIR := includes/
INCFLAGS := -I $(INCDIR) -I libft/includes
LIBFLAGS := -L libft/ -lft
OBJDIR := .obj/
SRCDIR := src/
SRC := main.c
NORME := $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))
OBJ := $(addprefix $(OBJDIR), $(SRC:.c=.o))

.PHONY: all clean fclean re tags dclean debug norme fsan

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJ)
	@make -s -C libft
	@$(CC) $(INCFLAGS) $(LIBFLAGS) -o $(NAME) $(OBJ)

debug: $(addprefix $(SRCDIR), $(SRC))
	@make -s -C libft
	@$(CC) $(DFLAGS) $(INCFLAGS) $(LIBFLAGS) -o $(DNAM) $<

fsan: $(addprefix $(SRCDIR), $(SRC))
	@make -s -C libft
	@$(CC) $(DFLAGS) -fsanitize=address $(INCFLAGS) $(LIBFLAGS) -o $(DNAM) $<

clean:
	@make -s -C libft clean
	@/bin/rm -Rf $(OBJDIR)

dclean:
	@/bin/rm -rf $(DNAM) $(DNAM).dSYM

fclean: clean
	@make -s -C libft fclean
	@/bin/rm -f $(NAME)

re: fclean all

tags:
	@ctags $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))

norme:
	@$(call disp_title,Norme,$(NORM_COLOR_T))
	@norminette $(NORME)

$(addprefix $(OBJDIR), %.o): $(addprefix $(SRCDIR), %.c)
	@$(CC) $(INCFLAGS) $(CFLAGS) -o $@ -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)
