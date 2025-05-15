# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 15:01:26 by jyniemit          #+#    #+#              #
#    Updated: 2025/05/15 17:31:34 by jyniemit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER = server
CLIENT = client
CC = cc
CFLAGS = -Wall -Werror -Wextra

LIBFTDIR = ./libft/
LIBFT = $(LIBFTDIR)libft.a

SERVER_SRC = ../server/src/server.c
CLIENT_SRC = ../client/src/client.c

OBJDIR = ./build/
SERVER_OBJ = $(addprefix $(OBJDIR), $(notdir $(SERVER_SRC:.c=.o)))
CLIENT_OBJ = $(addprefix $(OBJDIR), $(notdir $(CLIENT_SRC:.c=.o)))

INCLUDEDIR = ./include/
HEADERS = $(INCLUDEDIR)minitalk.h

all: $(SERVER) $(CLIENT)

$(SERVER): $(LIBFT) $(OBJDIR) $(SERVER_OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -L$(LIBFTDIR) -lft -o $(SERVER)

$(CLIENT): $(LIBFT) $(OBJDIR) $(CLIENT_OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -L$(LIBFTDIR) -lft -o $(CLIENT)

$(LIBFT):
	@make -C $(LIBFTDIR)

$(OBJDIR)%.o: ../server/src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -I$(LIBFTDIR) -c $< -o $@

$(OBJDIR)%.o: ../client/src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -I$(LIBFTDIR) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)
	@make -C $(LIBFTDIR) clean

fclean: clean
	@rm -f $(SERVER)
	@rm -f $(CLIENT)
	@make -C $(LIBFTDIR) fclean

debug: CFLAGS += -g
debug: re
	@echo "Recompiling with debugflags: $(CFLAGS)"

re: fclean all

.PHONY: all clean fclean re debug
