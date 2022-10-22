CC = gcc
BIN = bin/
SRC = src/
CFLAGS = -lm
INCLUDES = include/
EXEC= k_means

.DEFAULT_GOAL = k_means

k_means: $(SRC)k_means.c $(BIN)utils.o
	$(CC) $(SRC)k_means.c $(BIN)utils.o -o $(BIN)$(EXEC) $(CFLAGS)

$(BIN)utils.o: $(SRC)utils.c $(INCLUDES)utils.h
	$(CC) -c $(SRC)utils.c -o $(BIN)utils.o $(CFLAGS)

clean:
	rm -r bin/*

run:
	./$(BIN)$(EXEC)