## Configurations 
SRC_DIR = src
OBJ_DIR = build
EXEC_DIR = bin
EXEC = verlet-magic


## Files
SRC_EXT = cpp
SRC_FILES = $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OBJ_DIR)/%.o,$(SRC_FILES))

## Commands
MKDIR_P = mkdir

## Build setup
CC_OPTS = -c -pipe -Wall -Wno-switch -ggdb -g3 -MMD -std=gnu++11
LN_OPTS =
CC = g++

## SDL options
CC_SDL = -I/usr/local/include/SDL2 -I include -D_REENTRANT
LN_SDL = -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf -lpthread



## This is the default action
all: make_dir $(EXEC)

## Executable program
$(EXEC): $(OBJ_FILES)
	$(CC) $(LN_OPTS) $^ -o $(EXEC_DIR)/$@ $(LN_SDL)

## Object list
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	$(CC) $(CC_OPTS) -o $@ -c $< $(CC_SDL)


## Create build artifact directories
make_dir: $(OBJ_DIR) $(EXEC_DIR)

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(EXEC_DIR):
	$(MKDIR_P) $(EXEC_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)


-include $(OBJ_FILES:.o=.d)
