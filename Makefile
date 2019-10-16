# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: callen <callen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/23 22:04:17 by callen            #+#    #+#              #
#    Updated: 2019/04/16 08:19:41 by callen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DNAM := d_$(NAME)
ANAM := a_$(NAME)

# CC ?= clang
CCFLAGS := -Wall -Wextra -Werror
DFLAGS := -Wall -Wextra -g
AFLAGS := $(DFLAGS) -fsanitize=address

LIBDIR := libft
INCDIR := includes
OBJDIR := .obj/
SRCDIR := srcs/

INCLUDES := -I$(INCDIR) -I$(LIBDIR)/$(INCDIR)
LDFLAGS := -L$(LIBDIR) -lft
DEBGLIBS := $(LIBDIR)/d_libft.a
ASANLIBS := $(LIBDIR)/a_libft.a
FRAMWRKS :=

SRC := main.c ft_md5.c ft_sha1.c \
	ft_sha1_transform.c ft_sha224.c ft_sha256.c \
	ft_sha256_transform.c ft_ssl_std_cmds.c \
	ft_sha512.c ft_sha512_transform.c ft_ssl_utils.c \
	ft_ssl_hash_utils.c ft_md5_utils.c ft_sha384_utils.c \
	ft_hash_check_utils.c ft_ssl_cmd_utils.c ft_ssl_dgst_help.c

# OBJ := $(addprefix $(OBJDIR), $(SRC:.c=.o))
SRCS = $(addprefix $(SRCDIR), $(SRC))
OBJS := $(SRCS:.c=.o)


NRM := $(shell which pynorme)
ifeq ($(NRM),)
	NRM = "$(shell \
	if [ -d ~/.usr_bin ] && [ -x ~/.usr_bin/norminette.py ];\
	then\
		echo ~/.usr_bin/norminette.py;\
	fi)"
endif
NORME := $(addsuffix *.h,$(INCDIR)/) $(addsuffix *.c,$(SRCDIR))

.PHONY: all debug clean dclean fclean re tags j k asan norme codesize

all: $(LIBDIR)/libft.a $(NAME)

$(LIBDIR)/libft.a:
	make -C $(LIBDIR) all

$(NAME): CFLAGS = $(CCFLAGS) $(INCLUDES)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS)

j: debug

k: dclean

asan: CFLAGS = -Wall -Wextra -g -fsanitize=address $(INCLUDES) $(ASANLIBS)
asan:
	make -C libft asan
	$(CC) $(CFLAGS) -o $(ANAM) $(addprefix $(SRCDIR), $(SRC))

debug: CFLAGS = -Wall -Wextra -g $(INCLUDES) $(DEBGLIBS)
debug:
	make -C libft debug
	$(CC) $(CFLAGS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

dclean:
	rm -rf $(DNAM) $(DNAM).dSYM $(ANAM) $(ANAM).dSYM
	make -C libft dclean

clean:
	make -C libft clean
	$(RM) $(OBJS)

# rm -Rf $(OBJDIR)

fclean: clean
	make -C libft fclean
# rm -f $(NAME)

re: fclean all

tags:
	ctags $(addsuffix *.h,$(INCDIR)/) $(addsuffix *.c,$(SRCDIR))

# $(addprefix $(OBJDIR), %.o): $(addprefix $(SRCDIR), %.c)
# 	@$(CC) $(INCFLAGS) $(CFLAGS) -o $@ -c $<

# $(OBJDIR):
# 	@mkdir -p $(OBJDIR)

norme:
	$(NRM) $(NORME)

codesize:
	cat $(NORME) | grep -Ev '(^\/\*|^\*\*|^\*\/$$|^$$|\*\/)' | wc -l
