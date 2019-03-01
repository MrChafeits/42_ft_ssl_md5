# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: callen <callen@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/16 21:01:37 by callen            #+#    #+#              #
#    Updated: 2018/11/13 20:59:59 by callen           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = clang
CFLAGS = -Wall -Wextra -Werror
INCDIR = includes/
OBJDIR = .obj/
SRCDIR = src/
SRC =	ft_atoi.c ft_bzero.c ft_nbrlen_base.c ft_isalnum.c ft_isalpha.c \
ft_isascii.c ft_isblank.c ft_isdigit.c ft_islower.c ft_isprint.c ft_isspace.c \
ft_isupper.c ft_itoa.c ft_itoa_base.c ft_lstadd_tail.c ft_lstadd.c \
ft_lstdel.c ft_lstdelone.c ft_lstiter.c ft_lstmap.c ft_lstnew.c ft_lstrev.c \
ft_memalloc.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c \
ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putchar.c ft_putendl_fd.c \
ft_putendl.c ft_puterr.c ft_putnbr_fd.c ft_putnbr.c ft_putstr_fd.c \
ft_putstr.c ft_realloc.c ft_strcat.c ft_strclr.c ft_strchr.c ft_strchri.c \
ft_strcmp.c ft_strcpy.c ft_strdel.c ft_strdup.c ft_strequ.c ft_striter.c \
ft_striteri.c ft_strjoin.c ft_strjoin_free.c ft_strlcat.c ft_strlen.c \
ft_strmap.c ft_strmapi.c ft_strncat.c ft_strncmp.c ft_strncpy.c ft_strnequ.c \
ft_strnew.c ft_strnstr.c ft_strrchr.c ft_strstr.c ft_strsplit.c ft_strsub.c \
ft_strtrim.c ft_toupper.c ft_tolower.c ft_wstrcpy.c

SRC += get_next_line.c

SRC += ft_buttfucc.c ft_color.c ft_conv.c ft_convspe.c ft_flags.c \
ft_getstype.c ft_getstype2.c ft_getutype.c ft_getutype2.c ft_pars.c ft_per.c \
ft_printf.c ft_printf_a.c ft_printf_b.c ft_printf_c.c ft_printf_d.c \
ft_printf_dtoa_a.c ft_printf_dtoa_e.c ft_printf_dtoa_ent.c ft_printf_dtoa_f.c \
ft_printf_dtoa_g_e.c ft_printf_dtoa_g_f.c ft_printf_dtoa_la.c \
ft_printf_dtoa_le.c ft_printf_dtoa_lent.c ft_printf_dtoa_lf.c \
ft_printf_dtoa_lg_le.c ft_printf_dtoa_lg_lf.c ft_printf_e.c ft_printf_f.c \
ft_printf_g.c ft_printf_g_e.c ft_printf_g_f.c ft_printf_getdaymonth.c \
ft_printf_ind.c ft_printf_itoa.c ft_printf_itoa_a.c ft_printf_itoa_base.c \
ft_printf_k.c ft_printf_ktoa.c ft_printf_la.c ft_printf_le.c ft_printf_lf.c \
ft_printf_lg.c ft_printf_lg_le.c ft_printf_lg_lf.c ft_printf_ls.c \
ft_printf_o.c ft_printf_p.c ft_printf_putchar_fd.c ft_printf_s.c \
ft_printf_strnew.c ft_printf_u.c ft_printf_x.c

NORME = $(addsuffix *.h,$(INCDIR)) $(addsuffix *.c,$(SRCDIR))
OBJ = $(addprefix $(OBJDIR), $(SRC:.c=.o))

CL_CYAN = \033[0;36m
CL_GREED = \033[0;32m
CL_RED = \033[0;31m
CL_WHITE = \033[0m
NORM_COLOR = \033[0;38;5;153m
NORM_COLOR_T = \033[0;1;141m
NORM_COLOR_ERR = \033[38;5;160m
NORM_COLOR_WAR = \033[38;5;214m
#@echo "  + libft : Compiling $(CL_CYAN)$(OBJDIR) < $^$(CL_WHITE)"
.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJ)
	@ar -rcsu $@ $(OBJ)
	@ranlib $@
	@echo " $(CL_WHITE)+ libft : Compiled $(CL_GREED)$@$(CL_WHITE) $(shell sleep 0.01)"

clean:
	@echo " $(shell \
		if [ -d $(OBJDIR) ]; \
		then \
			echo " $(CL_WHITE)- libft : Removing $(CL_RED)$(OBJDIR)$(CL_WHITE) with$(CL_RED)"; \
			ls $(OBJDIR) | wc -w | tr -s ' '; echo "$(CL_WHITE)*.o"; \
			rm -Rf $(OBJDIR); \
		else \
			echo " $(CL_WHITE)# libft : Nothing to clean"; \
		fi) "

fclean: clean
	@echo " $(shell \
		if [ -f $(NAME) ]; \
		then \
			echo " $(CL_WHITE)- libft : Removing  $(CL_RED)$(NAME)$(CL_WHITE)"; \
			rm -f $(NAME); \
		else \
			echo " $(CL_WHITE)# libft : Nothing to fclean"; \
		fi) "

re: fclean all

norme:
	@$(call disp_title,Norme,$(NORM_COLOR_T))
	@norminette $(NORME) | sed "s/Norme/$(NORM_COLOR_T)➤  $(NORM_COLOR)Norme/g;s/Warning/$(NORM_COLOR_WAR)Warning/g;s/    Error/$(NORM_COLOR_ERR)Error/g"

$(addprefix $(OBJDIR), %.o): $(addprefix $(SRCDIR), %.c)
	@$(CC) -I $(INCDIR) $(CFLAGS) -o $@ -c $<

$(OBJDIR):
	@$(shell \
	if [ -d $(OBJDIR) ]; \
	then \
		mkdir -p $(OBJDIR); \
		echo " + libft : Created  $(CL_GREED)$(OBJDIR)$(CL_WHITE)$(CL_WHITE)"; \
	fi)
	@mkdir -p $(OBJDIR)
