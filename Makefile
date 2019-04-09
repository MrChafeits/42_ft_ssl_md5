# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: callen <callen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/23 22:04:17 by callen            #+#    #+#              #
#    Updated: 2019/04/05 23:08:16 by callen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DNAM := d_$(NAME)
ANAM := a_$(NAME)

CC ?= clang
CFLAGS := -Wall -Wextra -Werror -pipe
DFLAGS := -Wall -Wextra -g -pipe
AFLAGS := $(DFLAGS) -fsanitize=address
OFLAGS := $(CFLAGS) -O2

LIBDIR := libft
INCDIR := includes
OBJDIR := .obj/
SRCDIR := srcs/

INCFLAGS := -I$(INCDIR) -I$(LIBDIR)/$(INCDIR)
LIBFLAGS := -L$(LIBDIR) -lft
DEBGLIBS := $(LIBDIR)/d_libft.a
ASANLIBS := $(LIBDIR)/a_libft.a
FRAMWRKS :=

SRC := main.c ft_md5.c ft_sha1.c \
	ft_sha1_transform.c ft_sha224.c ft_sha256.c \
	ft_sha256_transform.c \
	ft_sha512.c ft_sha512_transform.c ft_ssl_utils.c \
	ft_ssl_hash_utils.c ft_md5_utils.c ft_sha384_utils.c \
	ft_hash_check_utils.c ft_ssl_cmd_utils.c ft_ssl_dgst_help.c
OBJ := $(addprefix $(OBJDIR), $(SRC:.c=.o))

NRM := $(shell which pynorme)
ifeq ($(NRM),)
	NRM = "$(shell \
	if [ -d ~/.usr_bin ] && [ -x ~/.usr_bin/norminette.py ];\
	then\
		echo ~/.usr_bin/norminette.py;\
	fi)"
endif
NORME := $(addsuffix *.h,$(INCDIR)/) $(addsuffix *.c,$(SRCDIR))

.PHONY: all debug clean dclean fclean re tags libft j k asan norme codesize

all: $(NAME)

libft:
	make -C libft

libdbg:
	make -C libft debug

libasan:
	make -C libft asan

$(NAME): libft $(OBJDIR) $(OBJ)
	$(CC) $(INCFLAGS) $(LIBFLAGS) -o $(NAME) $(OBJ)

j: debug

k: fclean dclean

asan: libasan
	$(CC) $(AFLAGS) $(INCFLAGS) $(ASANLIBS) -o $(ANAM) $(addprefix $(SRCDIR), $(SRC))

debug: libdbg
	$(CC) $(DFLAGS) $(INCFLAGS) $(DEBGLIBS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

dclean:
	rm -rf $(DNAM) $(DNAM).dSYM $(ANAM) $(ANAM).dSYM
	make -C libft dclean

clean:
	make -C libft clean
	rm -Rf $(OBJDIR)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

tags:
	ctags $(addsuffix *.h,$(INCDIR)/) $(addsuffix *.c,$(SRCDIR))

$(addprefix $(OBJDIR), %.o): $(addprefix $(SRCDIR), %.c)
	$(CC) $(INCFLAGS) $(CFLAGS) -o $@ -c $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

norme:
	$(NRM) $(NORME)

codesize:
	cat $(NORME) | grep -Ev '(^\/\*|^\*\*|^\*\/$$|^$$|\*\/)' | wc -l
