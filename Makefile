CC = gcc
OBJ = file-o-req.o file-o-res.o FileHandler.o

all: client server

server : file-o-res.o
	$(CC) -o $@ file-o-res.o libsockets.a helpers.a

client : file-o-req.o
	$(CC) -o $@ file-o-req.o libsockets.a helpers.a

%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o client server
