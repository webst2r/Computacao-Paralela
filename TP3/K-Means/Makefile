MPICC = mpicc
BIN = bin/
SRC = src/
CFLAGS = -lm -O2
INCLUDES = include/
EXEC= k_means

.DEFAULT_GOAL = k_means

k_means: $(SRC)k_means.c
	$(MPICC) $(SRC)k_means.c -o $(BIN)$(EXEC) $(CFLAGS)

clean:
	rm -r bin/*

run:
	mpirun -np 8 $(BIN)$(EXEC) 10000000 4 8
