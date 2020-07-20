# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wquinoa <wquinoa@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/07 23:33:20 by wquinoa           #+#    #+#              #
#    Updated: 2020/07/20 19:56:09 by wquinoa          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Source files
SRCS :=			minishell.c		envlist.c		
				
BUILT :=		echo.c			cd_pwd.c		env_export_unset.c	ft_exit.c	\
				path_search.c

PARSE :=		env_paste.c		parse_args.c	parse_utils.c

#	Utilities
WHT = \033[0m
GRN = \033[32m
RED = \033[31m
WHT1 = \033[0;1m
GRN1 = \033[32;1m
RED1 = \033[31;1m
DRK = \033[2m
MADE_MSG = \r   $(WHT1)Created $(GRN1)
DEL_MSG = \r   $(WHT1)Removed $(DRK)$(RED1)
ERROR_MSG = "\n   $(WHT1)$(DRK)Nothing to $@$(WHT)\n"
NORME = norminette $(S_FILES) | awk '{sub(/Norme/,"$(GRN)Norme$(WHT)")}1' | awk '{sub(/Error/,"$(RED)Error$(WHT)")}1'

#	Variables
NAME = minishell

#	Dirs
BIN = ./obj/
S_DIR = ./src/
BU_DIR = builtins/
PA_DIR = parser/
I_DIR = ./include

#	Files
BU_FILES = $(addprefix $(BU_DIR), $(BUILT))
PA_FILES = $(addprefix $(PA_DIR), $(PARSE))
S_FILES = $(addprefix $(S_DIR), $(SRCS) $(BU_FILES) $(PA_FILES))

CC = gcc
CF = -Wall -Wextra -Werror
.PHONY: all libft bonus norme clean fclean re

#	Rules
all: $(NAME)

$(NAME): $(S_FILES) | libft
	@gcc $^ ./libft/libft.a -I $(I_DIR) -o $(NAME)
	@echo "$(MADE_MSG)$(NAME)$(WHT)\n"
	@./$(NAME)
ifeq ($(WITH_BONUS),true)
	@echo "	$(WHT1)...added $(GRN1)ft_printf$(WHT)\n"
endif

libft:
	@$(MAKE) -C libft

bonus:
	@$(MAKE) 'WITH_BONUS = true' all

norme:
	@$(MAKE) -C libft norme
	@echo "$(DRK)$(BLU1)\n\t$(NAME)$(WHT)\n"
	@$(NORME)

#	Trash removal rules
clean:
	@$(MAKE) -C libft clean

fclean: clean
	@$(MAKE) -C libft fclean
	@if test -f $(NAME); \
	then rm -rf $(NAME); \
	rm -rf a.out; \
	echo "$(DEL_MSG)$(NAME)$(WHT)\n"; \
	else echo $(ERROR_MSG); \
	fi

re: fclean all
