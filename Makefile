#OBJS specifiles which files to compile
OBJS = main.cpp src/DungeonMap.cpp src/GeneratedMap.cpp src/Room.cpp src/Vector2i.cpp

#CC specifies which compiler to use
CC = g++

#COMPILER_FLAGS specifies additional compilation options
# -w suppress all warnings
COMPILER_FLAGS = -w -std=gnu++0x

DEBUGFLAGS = -g -O0 -ggdb
RELEASEFLAGS = -O3

# LINKER_FLAGS specifies the libraries we're linking to
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of the executable
OBJ_NAME = bin/dungeongame.sh

#include volders
INCLUDE_FOLDERS = -I include/

#this is the target that compiles our executable
#all : $(OBJS)
#	$(CC) $(OBJS) $(COMPILER_FLAGS) $(INCLUDE_FOLDERS) $(LINKER_FLAGS) -o $(OBJ_NAME)

ifeq ($(CFG),debug)
CXXFLAGS = -O0 -g
else
CXXFLAGS = -O3
endif

all:

ifneq ($(CFG),release)
ifneq ($(CFG),debug)
	@echo "invalid configuration "$(CFG)" specified."
	@echo "You must specify either debug or release, e.g."
	@echo "make CFG=debug"
	@exit 1
endif
endif

all : $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) $(COMPILER_FLAGS) $(INCLUDE_FOLDERS) $(LINKER_FLAGS) -o $(OBJ_NAME)


