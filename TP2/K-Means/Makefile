CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC_SEQ= k_means_seq
EXEC_PAR= k_means_par
THREADS = 2
CFLAGS = -lm -O2

DEFAULT_GOAL : k_means_seq k_means_par

k_means_seq: $(SRC)k_means_seq.c $(BIN)utils.o
	$(CC) $(SRC)k_means_seq.c $(BIN)utils.o -o $(BIN)$(EXEC_SEQ) $(CFLAGS)

k_means_par: $(SRC)k_means_par.c $(BIN)utils.o
	$(CC) $(SRC)k_means_par.c $(BIN)utils.o -o $(BIN)$(EXEC_PAR) $(CFLAGS)

$(BIN)utils.o: $(SRC)utils.c $(INCLUDES)utils.h
	$(CC) -c $(SRC)utils.c -o $(BIN)utils.o $(CFLAGS)

clean:
	rm -r bin/*

runseq:
	./$(BIN)$(EXEC_SEQ) 10000000 $(CP_CLUSTERS)

runpar:
	./$(BIN)$(EXEC_PAR) 10000000 $(CP_CLUSTERS) $(THREADS)
