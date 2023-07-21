# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gsever <gsever@student.42kocaeli.com.tr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 18:46:27 by gsever            #+#    #+#              #
#    Updated: 2023/07/21 22:41:59 by gsever           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!!!!!!!!! NOTE: < here > you need change with your customize. !!!!!!!
NAME		= ircserv

#	Flags for compile
CC			= c++
FLAGS		= -Wall -Werror -Wextra -std=c++98 -Wshadow -O3 $(INCLUDE_FLAGS)
# INCLUDE_FLAGS = -I $(HEADERS_DIRECTORY)/

#	All process use for compiling.
UNAME		:= $(shell uname -s)
NUMPROC		:= 8

OS			:= NULL

INCLUDE_FLAGS	= \
	-I$(HEADERS_DIRECTORY)

HEADERS_DIRECTORY = includes
HEADERS		= $(wildcard $(HEADERS_DIRECTORY)/*.hpp)
SOURCES_DIRECTORY = sources
SOURCES		= $(wildcard $(SOURCES_DIRECTORY)/*.cpp)
OBJECTS_DIRECTORY = objects
# OBJECTS		= $(SOURCES:%.cpp=%.o)
OBJECTS		= $(addprefix $(OBJECTS_DIRECTORY)/, $(notdir $(SOURCES:%.cpp=%.o)))

#	COLORS --> 🟥 🟩 🟦
BLACK	= \033[0;30m
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
PURPLE	= \033[0;35m
CYAN	= \033[0;36m
WHITE	= \033[0;37m
END		= \033[m
RESET	= \033[0m
X		= \033[m

#	COLORS BOLD--> B🟥 B🟩 B🟦
B_CYAN		= \033[1;36m
B_BLUE		= \033[1;34m
B_YELLOW	= \033[1;33m
B_GREEN		= \033[1;32m
B_RED		= \033[1;31m
B_RESET		= \033[1m
#NOTE: \033[ ile derlenince calisiyor \e[ ile derlenince bozuk calisiyor.

#	Compiling with all threads.
ifeq ($(UNAME), Linux)
	NUMPROC	:= $(shell grep -c ^processor /proc/cpuinfo)
	OS = "You are connected from -$(CYAN)$(UNAME)$(X)- 🐧 Welcome -$(CYAN)$(USER)$(X)- 😄!"
else ifeq ($(UNAME), Darwin)
	NUMPROC	:= $(shell sysctl -n hw.ncpu)
	OS = "You are connected from 42 school's iMac 🖥 ! Welcome $(CYAN)$(USER)$(X)"
	ifeq ($(USER), yuandre)
		OS = "You are connected from -$(CYAN)MacBook$(X)- 💻 Welcome -$(CYAN)$(USER)$(X)-!"
	endif
endif
# You can use --> man sysctl -> shell: sysctl -a | grep "hw.ncpu"

all:
	@$(MAKE) $(NAME) -j $(NUMPROC) --no-print-directory
#	@$(MAKE) -s $(NAME) -j $(NUMPROC)

#	Compiling
$(OBJECTS_DIRECTORY)/%.o: $(SOURCES_DIRECTORY)/%.cpp | $(OBJECTS_DIRECTORY)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "%-57b %b" "$(BLUE)COMPILED $(CYAN)$@" "$(GREEN)[✓]$(X)\n"
# NOTE: $@ -> First $(<ARGUMAN>)
# NOTE: $< -> Second $(<ARGUMAN>)

$(NAME): $(OBJECTS_DIRECTORY) $(OBJECTS)
	@$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)
	@printf "%-50b %b" "$(GREEN)CREATED $@" "[FINISHED]$(X)\n"
	@echo $(OS) Compiled with $(NUMPROC) cores!

#	Objects file creating
$(OBJECTS_DIRECTORY):
	@if [ ! -d $@ ]; then \
		mkdir -p $@; \
		echo "$(NAME): $(GREEN)$@ folder was created$(RESET)"; \
	fi
# NOTE: $@ -> @(OBJECTS_DIRECTORY)
# @mkdir -p $(OBJECTS_DIRECTORY);
# @echo "$(NAME): $(GREEN)$(OBJECTS_DIRECTORY) was created$(RESET)";

clean: 
	@rm -rf $(OBJECTS_DIRECTORY)
	@echo "$(NAME): $(RED)$(OBJECTS) was deleted$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME): $(RED)$(NAME) was deleted$(RESET)"

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

.PHONY: all clean fclean re