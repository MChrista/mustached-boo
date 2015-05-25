CC = gcc
OBJ = file-o-req.o file-o-res.o FileHandler.o

all: server client fileHandler

server : file-o-res.o
	$(CC) -o $@ file-o-res.o libsockets.a

client : file-o-req.o
	$(CC) -o $@ file-o-req.o libsockets.a

fileHandler : FileHandler.o
	$(CC) -o $@ FileHandler.o


%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o client server fileHandler
