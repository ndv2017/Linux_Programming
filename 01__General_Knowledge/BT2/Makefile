.PHONY: objs shared install

LIB_NAME	:= testshared

CUR_DIR		:= .
INC_DIR		:= $(CUR_DIR)/inc
SRC_DIR		:= $(CUR_DIR)/src
OBJ_DIR		:= $(CUR_DIR)/obj
$(shell mkdir -p $(OBJ_DIR))
BIN_DIR		:= $(CUR_DIR)/bin
$(shell mkdir -p $(BIN_DIR))

LIB_DIR		:= $(CUR_DIR)/libs
LIB_SHARED	:= $(LIB_DIR)/shared
$(shell mkdir -p $(LIB_SHARED))
# LIB_STATIC	:= $(LIB_DIR)/static

CC		:= gcc
# Tells the compiler to add 'INC_DIR' to the list of directories
# to search for header files
INC_FLAGS 	:= -I $(INC_DIR)
# Compile with -fPIC to generate position-independent code (PIC),
# allowing the object files to be loaded at any address in memory.
CFLAGS	:= -c -fPIC -o

all: objs shared cp_lib
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_SHARED) -l$(LIB_NAME) -o $(BIN_DIR)/use-shared-library

objs:
	$(CC) $(CFLAGS) $(OBJ_DIR)/main.o $(CUR_DIR)/main.c $(INC_FLAGS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/my_SumOperation.o $(SRC_DIR)/my_SumOperation.c $(INC_FLAGS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/my_Hi.o $(SRC_DIR)/my_Hi.c $(INC_FLAGS)

# Create shared library
# Lib's name must be: lib + name_of_lib; e.g. libtestshared
shared:
	$(CC) -shared $(OBJ_DIR)/my_SumOperation.o $(OBJ_DIR)/my_Hi.o -o $(LIB_SHARED)/lib$(LIB_NAME).so

# Move/Copy the library to default location
# Need permission for this step
cp_lib:
	sudo cp -f $(LIB_SHARED)/lib$(LIB_NAME).so /usr/lib

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_SHARED)/*
	rm -rf $(BIN_DIR)/*
	sudo rm -rf /usr/lib/lib$(LIB_NAME).so

clean_all_dirs: clean
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(BIN_DIR)
