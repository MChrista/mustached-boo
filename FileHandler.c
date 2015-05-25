#define BUFSIZE 1000
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/errno.h>
#include<sys/signal.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <unistd.h>
#include <fcntl.h>


int openFile(char *buffer, char *file_name, int *length);

int main(int argc, char **argv){
    char buffer[BUFSIZE];
    char *file_name = argv[1];
    int *bufferlength;
    openFile(buffer,file_name,bufferlength);
    exit(0);
}


int openFile(char *buffer, char *file_name, int *length){
    int fd;
    fd = open(file_name,O_RDONLY);
    int cc;
    length = &cc;
    while ((cc = read(fd, buffer, BUFSIZE)) > 0) {
        printf("server to client: %.*s\n", *length, buffer);
    }
    
    close(fd);
    return fd;
}


