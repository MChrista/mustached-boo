CC = gcc
OBJ = file-o-req.o file-o-res.o

all: result

result : file-o-res.o
	$(CC) -o $@ file-o-res.o libsockets.a

%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o result req
