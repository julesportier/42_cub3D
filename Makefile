# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juportie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 09:06:58 by juportie          #+#    #+#              #
#    Updated: 2025/10/08 16:24:36 by vakozhev         ###   ########.fr        #
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
LIBFT_DIR := ./libft
LIBFT_AR := $(LIBFT_DIR)/libft.a
WARNINGS := -Wall -Werror -Wextra
INC_FLAGS := -I. -I$(LIBFT_DIR) -I$(LIBFT_DIR)/includes -Isrc/parsing
CFLAGS := $(WARNINGS) $(INC_FLAGS)
CFLAGS_DB := $(WARNINGS) -g3 $(INC_FLAGS) 
CFLAGS_NE := -g3 $(INC_FLAGS)
CFLAGS_REL := $(WARNINGS) -O2 $(INC_FLAGS)
DEP_FLAGS = -MMD



LIBS_FLAGS := -lft -L$(LIBFT_DIR)


# (en haut du Makefile)
INC_FLAGS := -I. -I$(LIBFT_DIR) -I$(LIBFT_DIR)/includes -Isrc/parsing

WARNINGS := -Wall -Werror -Wextra


# cibles pratiques
dbg: CFLAGS = $(CFLAGS_DB)
dbg: LDFLAGS = $(LDFLAGS_DB)
dbg: all


#########
# FILES #
#########
NAME := cub3D

SRC_DIR := src/parsing
vpath %.c $(SRC_DIR)
SRC := \
	src/parsing/main/parse_args.c\
	src/parsing/main/parse_utils.c\
	src/parsing/main/parsing_cleanup.c\
	src/parsing/main/parsing_main.c\
	src/parsing/main/parsing.c\
	src/parsing/header/parse_id.c\
	src/parsing/header/parse_rgb.c\
	src/parsing/header/parse_texture.c\
	src/parsing/header/rgb_lexer.c\
	src/parsing/map/parsing_map_builder.c\
	src/parsing/map/parsing_map_checks.c\
	src/parsing/map/parsing_map_grid.c

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


# Only print a message if actually removing files/folders
rm_wrapper = rm -r $(1) 2>/dev/null && echo $(2) || true
clean:
	make clean -C $(LIBFT_DIR)

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
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LIBS_FLAGS) $(LIBMLX_FLAGS)


$(BUILD_DIR)/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEP_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir $@
