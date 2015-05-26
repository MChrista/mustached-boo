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

#include "FileHandler.h"


int openFile(char *buffer, char *file_name, int *length);


int openFile(char *buffer, char *file_name, int *length){
    int fd;
    fd = open(file_name,O_RDONLY);
    int cc;
    while ((cc = read(fd, buffer, BUFSIZE)) > 0) {
        *length = cc;
    }
    close(fd);
    return fd;
}


