BIN_DIR = bin
INC_DIR = inc
OBJ_DIR = obj
SRC_DIR = src

EXE_FILE = app

all: $(BIN_DIR)/app

$(BIN_DIR)/$(EXE_FILE): $(OBJ_DIR)/main.o $(OBJ_DIR)/rides.o $(OBJ_DIR)/vehicles.o $(OBJ_DIR)/clients.o $(OBJ_DIR)/managers.o $(OBJ_DIR)/auth.o $(OBJ_DIR)/menus.o $(OBJ_DIR)/utilities.o
	gcc -g -o $(BIN_DIR)/$(EXE_FILE) $(OBJ_DIR)/main.o $(OBJ_DIR)/rides.o $(OBJ_DIR)/vehicles.o $(OBJ_DIR)/clients.o $(OBJ_DIR)/managers.o $(OBJ_DIR)/auth.o $(OBJ_DIR)/menus.o $(OBJ_DIR)/utilities.o

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	gcc -g -o $(OBJ_DIR)/main.o $(SRC_DIR)/main.c -c

$(OBJ_DIR)/rides.o: $(SRC_DIR)/rides.c $(INC_DIR)/header.h
	gcc -g -o $(OBJ_DIR)/rides.o $(SRC_DIR)/rides.c -c

$(OBJ_DIR)/vehicles.o: $(SRC_DIR)/vehicles.c $(INC_DIR)/header.h
	gcc -g -o $(OBJ_DIR)/vehicles.o $(SRC_DIR)/vehicles.c -c

$(OBJ_DIR)/clients.o: $(SRC_DIR)/clients.c $(INC_DIR)/header.h
	gcc -g -o $(OBJ_DIR)/clients.o $(SRC_DIR)/clients.c -c

$(OBJ_DIR)/managers.o: $(SRC_DIR)/managers.c $(INC_DIR)/header.h
	gcc -g -o $(OBJ_DIR)/managers.o $(SRC_DIR)/managers.c -c

$(OBJ_DIR)/auth.o: $(SRC_DIR)/auth.c $(INC_DIR)/header.h
	gcc -g -o $(OBJ_DIR)/auth.o $(SRC_DIR)/auth.c -c

$(OBJ_DIR)/menus.o: $(SRC_DIR)/menus.c $(INC_DIR)/header.h
	gcc -g -o $(OBJ_DIR)/menus.o $(SRC_DIR)/menus.c -c

$(OBJ_DIR)/utilities.o: $(SRC_DIR)/utilities.c $(INC_DIR)/header.h
	gcc -g -o $(OBJ_DIR)/utilities.o $(SRC_DIR)/utilities.c -c

clean:
	del /Q /S $(BIN_DIR)\*.exe
	del /Q /S $(OBJ_DIR)\*.o

run:
	MinGW32-make clean
	MinGW32-make all
	./$(BIN_DIR)/$(EXE_FILE)
