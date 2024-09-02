# define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# define libraries to be linked (for example -lm)
LIB = 

# define sets of header source files and object files
SRC = driver2.c driver3.c data.c list.c array.c radixTree.c utils.c
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)

# define the executable name(s)
EXE_DICT2 = dict2
EXE_DICT3 = dict3

# targets:
$(EXE_DICT2): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE_DICT2) driver2.o data.o list.o array.o utils.o $(LIB)

$(EXE_DICT3): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE_DICT3) driver3.o data.o list.o array.o radixTree.o utils.o $(LIB)

driver2.o: driver2.c data.h list.h array.h
	$(CC) $(CFLAGS) -c driver2.c -o driver2.o

driver3.o: driver3.c data.h list.h array.h
	$(CC) $(CFLAGS) -c driver3.c -o driver3.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

data.o: data.c data.h
	$(CC) $(CFLAGS) -c data.c -o data.o

list.o: list.c list.h data.h
	$(CC) $(CFLAGS) -c list.c -o list.o

array.o: array.c array.h data.h utils.h
	$(CC) $(CFLAGS) -c array.c -o array.o

radixTree.o: radixTree.c radixTree.h list.h data.h utils.h
	$(CC) $(CFLAGS) -c radixTree.c -o radixTree.o

clean:
	rm -f $(OBJ) $(EXE_DICT2) $(EXE_DICT3)

