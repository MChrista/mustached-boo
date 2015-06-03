
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
#include<unistd.h>
#include "libsockets/passive_tcp.h"
#include "libsockets/connect_tcp.h"
#include "libsockets/socket_info.h"
#include "libsockets/socket_io.h"
#include "helpers/FileHandler.h"

int main(int argc, char **argv) {
    int port;
    port = atoi(argv[2]);
    char *file_name = argv[3];
    int fileSize=0;
    char buffer[BUFSIZE];

    //Hier koennte man noch überprüfen, ob das lesen erfolgreich war
    openFile(buffer,file_name,&fileSize);

    // cc-2 weil vom telnet immer noch /n (Return) abgezogen werden muss!
    //int length = cc-2;


    //Socket Descriptor
    //sd < 0 Fehler
    int sd = connect_tcp(argv[1], port);
    if (sd < 0) {
        printf("Failed to create Socket\n");
    } else {
        printf("Success by creating Socket\n");
    }

    if (write(sd, buffer, fileSize) < 0) {
        printf("%s\n", "Writing to the client went wrong!");
    }

    printf("Client to server: %.*s\n", fileSize, buffer);

    int cc;
    char buf[BUFSIZE];
    while ((cc = read(sd, buf, BUFSIZE)) > 0) {
        int length = cc;
        printf("server to client: %.*s\n", length, buf);
    }
    close(sd);
    exit(0);

}
