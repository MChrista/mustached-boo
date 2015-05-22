CC = gcc
OBJ = file-o-req.o file-o-res.o

all: server client

server : file-o-res.o
	$(CC) -o $@ file-o-res.o libsockets.a

client : file-o-req.o
	$(CC) -o $@ file-o-req.o libsockets.a


%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o result req
