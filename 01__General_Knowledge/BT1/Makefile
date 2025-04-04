.PHONY: objs static 

LIB_NAME 	:= teststatic

CUR_DIR 	:= .
INC_DIR 	:= $(CUR_DIR)/inc
SRC_DIR 	:= $(CUR_DIR)/src
OBJ_DIR 	:= $(CUR_DIR)/obj
$(shell mkdir -p $(OBJ_DIR))
BIN_DIR 	:= $(CUR_DIR)/bin
$(shell mkdir -p $(BIN_DIR))

LIB_DIR 	:= $(CUR_DIR)/libs
# LIB_SHARED 	:= $(LIB_DIR)/shared
LIB_STATIC 	:= $(LIB_DIR)/static
$(shell mkdir -p $(LIB_STATIC))

CC 		:= gcc
# Tells the compiler to add 'INC_DIR' to the list of directories
# to search for header files
INC_FLAGS 	:= -I $(INC_DIR)
CFLAGS	:= -c -o

all: objs static
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_STATIC) -l$(LIB_NAME) -o $(BIN_DIR)/statically-linked

# Create the object files for the static library (without -fPIC)
objs:	
	$(CC) $(CFLAGS) $(OBJ_DIR)/main.o $(CUR_DIR)/main.c $(INC_FLAGS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/my_SumOperation.o $(SRC_DIR)/my_SumOperation.c $(INC_FLAGS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/my_Hi.o $(SRC_DIR)/my_Hi.c $(INC_FLAGS)

# Create static library
static:
	ar rcs $(LIB_STATIC)/lib$(LIB_NAME).a $(OBJ_DIR)/my_SumOperation.o $(OBJ_DIR)/my_Hi.o

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_STATIC)/*
	rm -rf $(BIN_DIR)/*

clean_all_dirs:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(BIN_DIR)
