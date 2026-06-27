NAME := ft_ping

CC := cc
CFLAGS := -Wall -Wextra -Werror -MMD -MP -g3
RM := rm -rf

SRC_DIR := src
INCLD_DIR := include
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC))
DEP := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.d,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -I $(INCLD_DIR) $^ -o $@

-include $(DEP)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(INCLD_DIR) -c $^ -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

re: fclean $(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
