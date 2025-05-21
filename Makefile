# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 15:01:26 by jyniemit          #+#    #+#              #
#    Updated: 2025/05/21 11:04:12 by jyniemit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER = server
CLIENT = client
CC = cc
CFLAGS = -Wall -Werror -Wextra

LIBFTDIR = ./libft/
LIBFT = $(LIBFTDIR)libft.a

SERVER_SRC = ./src/server.c
CLIENT_SRC = ./src/client.c

OBJDIR = ./build/
SERVER_OBJ = $(addprefix $(OBJDIR), $(notdir $(SERVER_SRC:.c=.o)))
CLIENT_OBJ = $(addprefix $(OBJDIR), $(notdir $(CLIENT_SRC:.c=.o)))

INCLUDEDIR = ./include/
HEADERS = $(INCLUDEDIR)minitalk.h

all: $(SERVER) $(CLIENT)

$(SERVER): $(LIBFT) $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -L$(LIBFTDIR) -lft -o $(SERVER)

$(CLIENT): $(LIBFT) $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -L$(LIBFTDIR) -lft -o $(CLIENT)

$(LIBFT):
	@make -C $(LIBFTDIR)

$(OBJDIR)%.o: ./src/%.c $(HEADERS) | $(OBJDIR)
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

debug: fclean
	@make -C $(LIBFTDIR) debug
	$(MAKE) CFLAGS="$(CFLAGS) -g" all

re: fclean all

.PHONY: all clean fclean re debug
