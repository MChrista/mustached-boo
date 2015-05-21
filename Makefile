CC = gcc
OBJ = file-o-req.o file-o-res.o

all: req result

req : file-o-req.o
	$(CC) -o $@ file-o-req.o

result : file-o-res.o
	$(CC) -o $@ file-o-res.o

%.o: %.c
	$(CC) -c $<

clean:
	rm -f *.o result req
