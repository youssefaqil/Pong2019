CC =		gcc

RM =		rm -f


CFLAGS =	-g -Wall -W -Wextra -lpthread -lSDL -lSDL_ttf


NAME_SERVER =   pong_multiplayer


SRCS_SERVER =   ./main.c			\
								./server.c    \
								./client.c    \
								./lib.c 			\
								./sdl_lib.c    \
 


OBJS_SERVER =   $(SRCS_SERVER:%.c=%.o)

all:		$(NAME_SERVER)

server:		$(NAME_SERVER)


$(NAME_SERVER): $(OBJS_SERVER)
		@echo "-> Compilation de server_bin en cours"
		$(CC) $(OBJS_SERVER) $(CFLAGS) -o $(NAME_SERVER) 



clean_server:
		@echo "-> clean du server"
		@$(RM) $(OBJS_SERVER)
		@$(LIB_CLEAN)

clean:		clean_server

fclean_server: 		clean_server
		@echo "-> fclean du server"
		@$(RM) $(NAME_SERVER)
		@$(LIB_FCLEAN)

re:		fclean clean


re_server:	fclean_server server clean_server

.PHONY:		all clean fclean re clean_client clean_server fclean_client fclean_server re_client re_server