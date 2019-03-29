# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    template.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: callen <callen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/23 22:04:17 by callen            #+#    #+#              #
#    Updated: 2019/03/27 16:11:59 by callen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DNAM := d_$(NAME)
ANAM := a_$(NAME)

CC ?= clang
CFLAGS := -Wall -Wextra -Werror
DFLAGS := -Wall -Wextra -g
AFLAGS := $(DFLAGS) -fsanitize=address
OFLAGS := $(CFLAGS) -Ofast

OBJDIR := .obj/
SRCDIR := srcs/
INCDIR := includes/
LIBDIR := libft

INCFLAGS := -I $(INCDIR) -I $(LIBDIR)/$(INCDIR)
LIBFLAGS := -L $(LIBDIR) -lft
DEBGLIBS := $(LIBDIR)/d_libft.a
ASANLIBS := $(LIBDIR)/a_libft.a
FRAMWRKS :=

SRC :=	main.c apple_openssl.c
OBJ := $(addprefix $(OBJDIR), $(SRC:.c=.o))

NRM := $(shell which pynorme)
ifeq ($(NRM),)
	NRM = "$(shell \
	if [ -d ~/.usr_bin ] && [ -x ~/.usr_bin/norminette.py ];\
	then\
		echo ~/.usr_bin/norminette.py;\
	fi)"
endif
NORME := $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))

.PHONY: all debug clean dclean fclean re tags libft j k asan norme codesize libfast fast

all: $(NAME)

libft:
	@make -sC $(LIBDIR)

libfast:
	@make -sC $(LIBDIR) CFLAGS="-Wall -Wextra -Werror -Ofast"

$(NAME): libft $(OBJDIR) $(OBJ)
	@$(CC) $(INCFLAGS) $(LIBFLAGS) $(FRAMWRKS) -o $(NAME) $(OBJ)

j: debug

k: fclean dclean

asan:
	@make -sC libft/ asan
	@$(CC) $(AFLAGS) $(INCFLAGS) $(ASANLIBS) $(FRAMWRKS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

debug:
	@make -sC $(LIBDIR) debug
	@$(CC) $(DFLAGS) $(INCFLAGS) $(DEBGLIBS) $(FRAMWRKS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

fast: libfast
	@$(CC) $(CFLAGS) -Ofast $(INCFLAGS) $(LIBFLAGS) $(FRAMWRKS) -o $(NAME) $(addprefix $(SRCDIR), $(SRC))

dclean:
	@rm -rf $(DNAM) $(DNAM).dSYM

clean:
	@make -sC $(LIBDIR) clean
	@rm -Rf $(OBJDIR)

fclean: clean
	@make -sC $(LIBDIR) dclean
	@make -sC $(LIBDIR) fclean
	@rm -f $(NAME)

re: fclean all

tags:
	@ctags $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))

$(addprefix $(OBJDIR), %.o): $(addprefix $(SRCDIR), %.c)
	@$(CC) $(INCFLAGS) $(CFLAGS) -o $@ -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

norme:
	@$(NRM) $(NORME)

codesize:
	@cat $(NORME) | grep -Ev '(^\/\*|^\*\*|^\*\/$$|^$$|\*\/)' | wc -l
