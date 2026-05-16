
NAME = ircserv

SRCS = main.cpp \
	src/Channel/Channel.cpp \
	src/Channel/ChannelUser.cpp \
	src/Client/Client.cpp \
	src/Client/ClientRegister.cpp \
	src/Server/Server.cpp \
	src/Server/ServerClient.cpp \
	src/Server/ServerCommands.cpp \
	src/Server/ServerGet.cpp \
	src/Server/ServerInput.cpp \
	src/Server/ServerOutput.cpp \
	src/Server/ServerService.cpp \
	src/Server/ServerSockets.cpp \
	src/utils.cpp

OBJ_DIR = build

OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

CPP = c++

CPPFLAGS = -g -Wall -Wextra -Werror -std=c++98

INCLUDE = -Ilib

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(OBJS) -o $(NAME)
	@echo "Executable created!"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CPP) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/src

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		rm -rf $(OBJ_DIR); \
		echo "Build directory deleted!"; \
	else \
		echo "No build directory to delete!"; \
	fi

fclean: clean
	@rm -f $(NAME)
	@echo "Executable deleted!"

re: fclean
	@echo "Recreating build directory..."
	@echo "Remaking executables..."
	@make --no-print-directory

run: re
	@./$(NAME) 8080 pass

val: re
	@valgrind --leak-check=full ./$(NAME) 8080 pass

.PHONY: all clean fclean re run val*