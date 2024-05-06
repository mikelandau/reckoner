#OBJS specifies which files to compile as part of the project
OBJS = $(wildcard src/*.cpp)

#CC specifies which compiler we're using
CC = em++

#COMPILER_FLAGS specifies the additional compilation options we're using
#COMPILER_FLAGS = -std=c++20 -Wall

#LINKER_FLAGS specifies the libraries we're linking against
#LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = bin/reckoner.html

#This is the target that compiles our executable
all : $(OBJS)
	mkdir -p bin
	$(CC) $(OBJS) -o $(OBJ_NAME)