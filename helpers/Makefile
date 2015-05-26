CC = gcc

OBJ = FileHandler.o

SRC = FileHandler.c

LIB = helpers.a

$(LIB) : $(OBJ)
	rm -f $(lib)
	ar rcs $(LIB) $(OBJ)

%.o : %.c
	$(CC) -c $<

clean:
	rm -f *.o *.a
