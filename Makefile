GCC = clang++
CC = clang
FLAGS =  -Wall -Wextra
LDFLAGS=  -lGL -lGLU -lGLEW -lglfw -ldl
NAME = scop
GLAD_SRC = ./resources/glad/src/glad.cpp
GLAD_HEADER = ./resources/glad/include/glad/glad.h

GREEN       = \033[32m
RED         = \033[31m
DEFAULT     = \033[37m

PATHOBJ = obj/
PATHSRC = src/
HEAD =   src/includes/scop.hpp src/classes/Shader.hpp src/classes/Matrix4.hpp src/classes/Vec4.hpp src/classes/Vec3.hpp src/classes/Camera.hpp src/classes/Vec2.hpp src/classes/Mesh.hpp src/classes/Parser.hpp src/classes/BmpImage.hpp  $(GLAD_HEADER)
FILES = main.cpp init.cpp render.cpp classes/Shader.cpp classes/Matrix4.cpp classes/Camera.cpp classes/Mesh.cpp classes/Parser.cpp classes/BmpImage.cpp hooks.cpp
SRCS = $(addprefix $(PATHSRC)/,$(FILES))
OBJ = $(patsubst %.cpp,$(PATHOBJ)/%.o,$(FILES))
DEP = Makefile $(HEAD)

ifeq ($(shell uname),Darwin)
	ECHO    = echo
else
	ECHO    = echo
endif

all: $(NAME)

$(PATHOBJ):
	@mkdir -p obj
	@mkdir -p obj/classes

$(NAME): $(DEP) $(SRCS) $(PATHOBJ) $(OBJ)
	@$(ECHO) "\r$(GREEN) The .o from $(NAME) are compiled.$(DEFAULT)"
	@$(GCC) $(FLAGS) $(OBJ) $(GLAD_SRC) $(LDFLAGS) -o $(NAME)
	@$(ECHO) "$(GREEN)$(NAME)$(DEFAULT) created."

$(PATHOBJ)/%.o: $(PATHSRC)/%.cpp $(DEP)
	@$(ECHO) "\r$(GREEN) Compiling $@                      \c\033[K"
	@$(GCC) $(FLAGS) -c $< -o $@

clean:
	@$(ECHO) "All $(RED).o$(DEFAULT) are now deleted for $(NAME)."
	@rm -rf $(PATHOBJ)

fclean: clean
	@$(ECHO) "$(RED)$(NAME)$(DEFAULT) is now deleted."
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re