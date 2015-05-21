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


static int handle_client(int sd);
static int accept_client(int sd);


int main(int argc, char **argv){
    if(argc != 3){ //Programm name and two arguments
        printf("Invalid number of arguments\n");
        exit(0);
    }
    int port,sd;
    port = atoi(argv[1]); //tested
    
    sd = passive_tcp(port, 5);
    if(sd < 0){
        printf("Error creating server\n");
        exit(0);
    }
    accept_client(sd);
    
    exit(0);
}

static int accept_client(int sd) {
    printf("We accept Clients\n");
    int retcode,nsd;
    struct sockaddr_in from_client;
    
    while(1){
        int from_client_len = sizeof(from_client);
        nsd = accept(sd,(struct sockaddr*)&from_client, &from_client_len);
        handle_client(nsd);
        
    }
    return nsd;
}

static int handle_client(int sd) {
    printf("We handle Clients\n");
    char buf[BUFSIZE];
    int cc; //Character count
    while( (cc = read(sd, buf, BUFSIZE)) > 0){
        //read drei fälle
        if(cc < 0){ // -> Das muss dann außerhalb der Schleife sein
            printf("Error");
        }else if(cc == 0){
            //Kommunikationspartner hat die Verbindung beendet
        }else{
            
        }
        
        if(write(sd,buf,cc)<0){
            // Fehler
        }
        
        
        
    }
    close(sd);
    return(sd);
}