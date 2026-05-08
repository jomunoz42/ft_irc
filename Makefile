
NAME = ircserv
SRCS = main.cpp src/Channel.cpp src/Client.cpp src/Server.cpp src/utils.cpp
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