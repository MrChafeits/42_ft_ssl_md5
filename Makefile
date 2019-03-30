# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    template.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: callen <callen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/23 22:04:17 by callen            #+#    #+#              #
<<<<<<< Updated upstream
#    Updated: 2019/03/27 16:11:59 by callen           ###   ########.fr        #
=======
#    Updated: 2019/03/28 00:10:15 by callen           ###   ########.fr        #
>>>>>>> Stashed changes
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DNAM := d_$(NAME)
ANAM := a_$(NAME)

CC ?= clang
CFLAGS := -Wall -Wextra -Werror
DFLAGS := -Wall -Wextra -g
AFLAGS := $(DFLAGS) -fsanitize=address
<<<<<<< Updated upstream
OFLAGS := $(CFLAGS) -Ofast

OBJDIR := .obj/
SRCDIR := srcs/
INCDIR := includes/
LIBDIR := libft
=======
OFLAGS := $(CFLAGS) -O2
INCDIR := includes/
INCFLAGS := -I $(INCDIR) -I libft/$(INCDIR)
LIBFLAGS := -L libft/ -lft
OBJDIR := .obj/
SRCDIR := srcs/

SRC := main.c
>>>>>>> Stashed changes

INCFLAGS := -I $(INCDIR) -I $(LIBDIR)/$(INCDIR)
LIBFLAGS := -L $(LIBDIR) -lft
DEBGLIBS := $(LIBDIR)/d_libft.a
ASANLIBS := $(LIBDIR)/a_libft.a
FRAMWRKS :=

SRC :=	main.c apple_openssl.c
OBJ := $(addprefix $(OBJDIR), $(SRC:.c=.o))

<<<<<<< Updated upstream
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
=======
.PHONY: all debug clean dclean fclean re tags libft j k asan norme codesize
>>>>>>> Stashed changes

all: $(NAME)

libft:
<<<<<<< Updated upstream
	@make -sC $(LIBDIR)

libfast:
	@make -sC $(LIBDIR) CFLAGS="-Wall -Wextra -Werror -Ofast"

$(NAME): libft $(OBJDIR) $(OBJ)
	@$(CC) $(INCFLAGS) $(LIBFLAGS) $(FRAMWRKS) -o $(NAME) $(OBJ)
=======
	make -C libft

$(NAME): libft $(OBJDIR) $(OBJ)
	$(CC) $(INCFLAGS) $(LIBFLAGS) -o $(NAME) $(OBJ)
>>>>>>> Stashed changes

j: debug

k: fclean dclean

<<<<<<< Updated upstream
asan:
	@make -sC libft/ asan
	@$(CC) $(AFLAGS) $(INCFLAGS) $(ASANLIBS) $(FRAMWRKS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

debug:
	@make -sC $(LIBDIR) debug
	@$(CC) $(DFLAGS) $(INCFLAGS) $(DEBGLIBS) $(FRAMWRKS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

fast: libfast
	@$(CC) $(CFLAGS) -Ofast $(INCFLAGS) $(LIBFLAGS) $(FRAMWRKS) -o $(NAME) $(addprefix $(SRCDIR), $(SRC))
=======
asan: libft
	$(CC) $(AFLAGS) $(INCFLAGS) $(LIBFLAGS) -o $(ANAM) $(addprefix $(SRCDIR), $(SRC))

debug: libft
	$(CC) $(DFLAGS) $(INCFLAGS) $(LIBFLAGS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))
>>>>>>> Stashed changes

dclean:
	rm -rf $(DNAM) $(DNAM).dSYM $(ANAM) $(ANAM).dSYM

clean:
<<<<<<< Updated upstream
	@make -sC $(LIBDIR) clean
	@rm -Rf $(OBJDIR)

fclean: clean
	@make -sC $(LIBDIR) dclean
	@make -sC $(LIBDIR) fclean
	@rm -f $(NAME)
=======
	make -C libft/ clean
	rm -Rf $(OBJDIR)

fclean: clean
	make -C libft/ fclean
	rm -f $(NAME)
>>>>>>> Stashed changes

re: fclean all

tags:
	ctags $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))

$(addprefix $(OBJDIR), %.o): $(addprefix $(SRCDIR), %.c)
	$(CC) $(INCFLAGS) $(CFLAGS) -o $@ -c $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

norme:
<<<<<<< Updated upstream
	@$(NRM) $(NORME)

codesize:
	@cat $(NORME) | grep -Ev '(^\/\*|^\*\*|^\*\/$$|^$$|\*\/)' | wc -l
=======
	norminette $(NORME)

codesize:
	cat $(NORME) | ggrep -Ev '(\/\*|\*\/|\*\*|^$$)' | wc -l
>>>>>>> Stashed changes
