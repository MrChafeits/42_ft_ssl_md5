NAME = ft_ssl
DNAM = d_$(NAME)
ANAM = a_$(NAME)

CCFLAGS = -Wall -Wextra -Werror
DFLAGS = -Wall -Wextra -g
AFLAGS = $(DFLAGS) -fsanitize=address

LIBDIR = libft
INCDIR = include
OBJDIR = .obj/
SRCDIR = src/

HAVE_USR_INC = $(shell test -d /usr/include; echo "$$?")
ifneq ("$(HAVE_USR_INC)", "0")
	SYSINCDIR = -I$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/include
	CCFLAGS += -Wno-nullability-completeness
	DFLAGS += -Wno-nullability-completeness
	AFLAGS += -Wno-nullability-completeness
else
	SYSINCDIR :=
endif

INCLUDES = -I$(INCDIR) -I$(LIBDIR)/$(INCDIR) $(SYSINCDIR)
LDFLAGS = -L$(LIBDIR) -lft

SRC = main.c ft_md5.c ft_sha1.c \
	ft_sha1_transform.c ft_sha224.c ft_sha256.c \
	ft_sha256_transform.c ft_ssl_std_cmds.c \
	ft_sha512.c ft_sha512_transform.c ft_ssl_utils.c \
	ft_ssl_hash_utils.c ft_md5_utils.c ft_sha384_utils.c \
	ft_hash_check_utils.c ft_ssl_cmd_utils.c ft_ssl_dgst_help.c strsplit_str.c

SRCS = $(addprefix $(SRCDIR), $(SRC))
OBJS = $(SRCS:.c=.o)


NRM = $(shell which pynorme)
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
$(NAME): $(OBJS) $(LIBDIR)/libft.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS)

j: debug

k: dclean

asan: CFLAGS = $(AFLAGS) $(INCLUDES) $(LDFLAGS)
asan: $(LIBDIR)/libft.a
	$(CC) $(CFLAGS) -o $(ANAM) $(addprefix $(SRCDIR), $(SRC))

debug: CFLAGS = $(DFLAGS) $(INCLUDES) $(LDFLAGS)
debug: $(LIBDIR)/libft.a
	$(CC) $(CFLAGS) -o $(DNAM) $(addprefix $(SRCDIR), $(SRC))

dclean:
	rm -rf $(DNAM) $(DNAM).dSYM $(ANAM) $(ANAM).dSYM

clean:
	make -s -C libft clean
	$(RM) $(OBJS)

fclean: clean
	make -s -C libft fclean
	$(RM) $(NAME)

re: fclean all

tags:
	ctags $(addsuffix *.h,$(INCDIR)/) $(addsuffix *.c,$(SRCDIR))

norme:
	$(NRM) $(NORME)

codesize:
	cat $(NORME) | grep -Ev '(^\/\*|^\*\*|^\*\/$$|^$$|\*\/)' | wc -l
