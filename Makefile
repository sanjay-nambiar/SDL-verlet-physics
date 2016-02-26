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
CC_OPTS = -c -pipe -Wall -MMD -std=gnu++11 -O3
CC_DEBUG_OPTS =
LN_OPTS =
CC = g++

## SDL options
SDL_LD_PATH = /usr/local/lib
SDL_INC_PATH = /usr/local/include/SDL2
CC_SDL = -I$(SDL_INC_PATH) -I include -D_REENTRANT
LN_SDL = -L$(SDL_LD_PATH) -Wl,-rpath,$(SDL_LD_PATH) -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf -lpthread



## Add debug opts to compiler
# debug: CC_DEBUG_OPTS = -g3 -O0
# debug: make_dir $(EXEC)

## This is the default action
all: make_dir $(EXEC)

## Executable program
$(EXEC): $(OBJ_FILES)
	$(CC) $(LN_OPTS) $^ -o $(EXEC_DIR)/$@ $(LN_SDL)

## Object list
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	$(CC) $(CC_OPTS) $(CC_DEBUG_OPTS) -o $@ -c $< $(CC_SDL)


## Create build artifact directories
make_dir: $(OBJ_DIR) $(EXEC_DIR)

$(OBJ_DIR):
	@$(MKDIR_P) $(OBJ_DIR)

$(EXEC_DIR):
	@$(MKDIR_P) $(EXEC_DIR)

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(EXEC_DIR)


-include $(OBJ_FILES:.o=.d)
