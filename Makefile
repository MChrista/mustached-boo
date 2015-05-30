CC = gcc
OBJ = file-o-req.o file-o-res.o

all: client server

libs :
	cd libsockets && $(MAKE)
	cd helpers && $(MAKE)

server : file-o-res.o
	$(CC) -o $@ file-o-res.o libsockets/libsockets.a helpers/helpers.a

client : file-o-req.o
	$(CC) -o $@ file-o-req.o libsockets/libsockets.a helpers/helpers.a

%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o client server
	cd libsockets && $(MAKE) clean
	cd helpers && $(MAKE) clean
