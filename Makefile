# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyork <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/09 19:11:26 by kyork             #+#    #+#              #
#    Updated: 2018/06/11 16:14:22 by kyork            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ft_nm ft_otool

NM_SRC		+=

OTOOL_SRC	+= 
OTOOL_SRC	+= 
OTOOL_SRC	+= 
OTOOL_SRC	+= 
OTOOL_SRC	+= 

COMMON_SRC	+= main.c swap.c
COMMON_SRC	+= macho.c macho-segments.c
COMMON_SRC	+= debugseg.c debugsect.c debugcmd1.c stab.c

COMMONOBJS	= $(addprefix build/common-, $(COMMON_SRC:.c=.o))
NM_OBJS		= $(addprefix build/nm-, $(NM_SRC:.c=.o)) $(COMMONOBJS)
OTOOL_OBJS	= $(addprefix build/otool-, $(OTOOL_SRC:.c=.o)) $(COMMONOBJS)

LIBS		= libft/libft.a

CFLAGS		+= -Wall -Wextra -Wmissing-prototypes
#CFLAGS		= -Wall -Wextra -Wfloat-equal -Wundef -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors
CFLAGS		+= -I includes/ -I srcs/
LDFLAGS		+= -Wall -Wextra

ifndef NO_WERROR
	CFLAGS += -Werror
	LDFLAGS += -Werror
endif

ifdef DBUG
	CFLAGS += -fsanitize=address -g
	LDFLAGS += -fsanitize=address -g
endif

ifdef RELEASE
	CFLAGS += -O2
	LDFLAGS += -O2
endif

ifeq ($(SHELL),zsh)
	PRINTF	= printf
else
	PRINTF  = $(shell which printf)
endif

.PHONY: all clean fclean re

all: $(NAME)

ft_nm: $(NM_OBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^

ft_otool: $(OTOOL_OBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^

libft/libft.a: libft/.git/refs/heads/master
	$(MAKE) -C libft libft.a

libft/.git/refs/heads/master:
	# ignore

clean:
	rm -rf build
	$(MAKE) -C libft clean
	@echo "\e[33m\e[1m[CLEAN]\e[m $$(basename $$(pwd))\n" | tr '\\e' '\e'

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C libft fclean
	@echo "\e[33m\e[1m[FCLEAN]\e[m $$(basename $$(pwd))\n" | tr '\\e' '\e'

re: fclean
	$(MAKE) all

build/common-%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

build/nm-%.o: src/nm/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

build/otool-%.o: src/otool/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

