#OBJS specifies which files to compile as part of the project
OBJS = $(wildcard src/*.cpp)

#CC specifies which compiler we're using
CC = em++

SHELL_FILE = html_template/shell.html

SHELL_FILE_ARG = --shell-file $(SHELL_FILE)

COMPILER_FLAGS = -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']"

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = bin/reckoner.html

#This is the target that compiles our executable
all : $(OBJS)
	mkdir -p bin
	rm -rf bin/*
	$(CC) $(OBJS) -o $(OBJ_NAME) $(SHELL_FILE_ARG) $(COMPILER_FLAGS)