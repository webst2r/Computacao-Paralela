CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC= k_means
THREADS = 16
CFLAGS = -lm -O2 -fopenmp

.DEFAULT_GOAL = k_means

k_means: $(SRC)k_means.c $(BIN)utils.o
	$(CC) $(SRC)k_means.c $(BIN)utils.o -o $(BIN)$(EXEC) $(CFLAGS)

$(BIN)utils.o: $(SRC)utils.c $(INCLUDES)utils.h
	$(CC) -c $(SRC)utils.c -o $(BIN)utils.o $(CFLAGS)

clean:
	rm -r bin/*

runseq:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS)

runpar:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS) $(THREADS)
