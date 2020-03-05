CC = gcc

EXEC = src/gamblersRuin
SRC = $(EXEC).c
OBJ = $(EXEC).o
FLAGS = -lm -lgsl -lgslcblas

fairGame: $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(FLAGS)

run: fairGame
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)
