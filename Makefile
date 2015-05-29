CC = gcc
OBJ = file-o-req.o file-o-res.o FileHandler.o

all: client server

server : file-o-res.o
	$(CC) -o $@ file-o-res.o libsockets/libsockets.a helpers/helpers.a

client : file-o-req.o
	$(CC) -o $@ file-o-req.o libsockets/libsockets.a helpers/helpers.a

%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o client server
