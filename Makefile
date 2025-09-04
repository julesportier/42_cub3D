# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juportie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 09:06:58 by juportie          #+#    #+#              #
#    Updated: 2025/09/03 11:43:03 by juportie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###########
# ALIASES #
###########
MAKE := make
CC := cc

#########
# FLAGS #
#########
WARNINGS := -Wall -Werror -Wextra
CFLAGS := $(WARNINGS)
CFLAGS_DB := $(WARNINGS) -g3
CFLAGS_NE := -g3
CFLAGS_REL := $(WARNINGS) -O2
DEP_FLAGS = -MMD

LIBFT_DIR := ./libft
LIBFT_AR := $(LIBFT_DIR)/libft.a
LIBFT_FLAGS := -lft -L$(LIBFT_DIR)

LIBMLX_DIR := ./minilibx
LIBMLX_AR := $(LIBMLX_DIR)/libmlx.a
LIBMLX_FLAGS := -lmlx -lXext -lX11 -L$(LIBMLX_DIR)

#########
# FILES #
#########
NAME := cub3D

SRC_DIR := src
vpath %.c $(SRC_DIR)
SRC := main.c

BUILD_DIR := build
OBJ := $(SRC:%.c=$(BUILD_DIR)/%.o)

DEP_FILES := $(OBJ:.o=.d)

###########
# TARGETS #
###########
.PHONY: all ft mlx clean fclean re noerr dbg rel

all: $(BUILD_DIR) ft mlx $(NAME)
.NOTPARALLEL: all

dbg: CFLAGS = $(CFLAGS_DB)
dbg: all
noerr: CFLAGS = $(CFLAGS_NE)
noerr: all
rel: CFLAGS = $(CFLAGS_REL)
rel: all

ft:
	@git submodule init
	@git submodule update
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(LIBFT_DIR)
mlx:
	$(MAKE) -C $(LIBMLX_DIR)

# Only print a message if actually removing files/folders
rm_wrapper = rm -r $(1) 2>/dev/null && echo $(2) || true
clean:
	make clean -C $(LIBFT_DIR)
	make clean -C $(LIBMLX_DIR)
	@$(call rm_wrapper,$(BUILD_DIR),"remove $(BUILD_DIR)/")
fclean: clean
	make fclean -C $(LIBFT_DIR)
	@$(call rm_wrapper,$(NAME),"remove $(NAME)")

re: fclean all

-include $(DEP_FILES)

#########
# RULES #
#########
$(NAME): $(OBJ) $(LIBFT_AR)
	$(CC) $^ -o $@ $(LIBFT_FLAGS)

$(BUILD_DIR)/%.o: %.c Makefile
	$(CC) $(CFLAGS) $(DEP_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir $@
