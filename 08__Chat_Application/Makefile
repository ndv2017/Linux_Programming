.PHONY: objs shared cp_lib run check_valgrind clean

LIB_NAME	:= chat_app

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

PORT		?= 8080
CC			:= gcc
INC_FLAGS	:= -I $(INC_DIR)
CFLAGS		:= -c -fPIC -o

all: objs shared cp_lib
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_SHARED) -l$(LIB_NAME) -o $(BIN_DIR)/chat_app
	@echo "----------------------------------------"
	@echo "Chat Application Build Successfully"
	@echo "Run '$(BIN_DIR)/chat_app <port_no>' or 'make run PORT=<port_no>' to start the chat application"
	@echo "----------------------------------------"

objs:
	$(CC) $(CFLAGS) $(OBJ_DIR)/main.o $(CUR_DIR)/main.c $(INC_FLAGS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/network.o $(SRC_DIR)/network.c $(INC_FLAGS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/chat.o $(SRC_DIR)/chat.c $(INC_FLAGS)

shared:
	$(CC) -shared $(OBJ_DIR)/network.o $(OBJ_DIR)/chat.o -o $(LIB_SHARED)/lib$(LIB_NAME).so

cp_lib:
	sudo cp -f $(LIB_SHARED)/lib$(LIB_NAME).so /usr/lib

run: all
	@echo "Running chat_app with PORT=$(PORT)..."
	@LD_LIBRARY_PATH=$(LIB_SHARED) ./$(BIN_DIR)/chat_app $(PORT)


check_valgrind: all
	@echo "Running valgrind on chat_app with PORT=$(PORT)..."
	@LD_LIBRARY_PATH=$(LIB_SHARED) valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_DIR)/chat_app $(PORT)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(BIN_DIR)
	sudo rm -rf /usr/lib/lib$(LIB_NAME).so
