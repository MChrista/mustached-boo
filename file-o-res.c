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
#include<stdarg.h>
#include "libsockets/passive_tcp.h"
#include "libsockets/connect_tcp.h"
#include "libsockets/socket_info.h"
#include "libsockets/socket_io.h"

// declare here for usage before implementation
static int handle_client(int sd, char* ipAdress);
static int accept_client(int sd);
char *file_name;

int main(int argc, char **argv){
    if(argc != 3){ 
        // program name and two arguments aren't given
        printf("Invalid number of arguments!\n");
        exit(0);
    } else if(!(access( argv[2], F_OK ) != -1 )) {
        // file doesn't exist
        printf("Given file doesn't exist!\n");
        exit(0);
    }
    file_name = argv[2];
    int port,sd;
    port = atoi(argv[1]); //make first argument the port number
    
    sd = passive_tcp(port, 5);
    if(sd < 0){
        // server creation went wrong
        printf("Error creating server\n");
        exit(0);
    }
    accept_client(sd);
    
    exit(0);
}

static int accept_client(int sd) {
    printf("Server ready to accept clients!\n");
    int retcode,nsd;
    struct sockaddr_in from_client;
    
    while(1){
        int from_client_len = sizeof(from_client);
        nsd = accept(sd,(struct sockaddr*)&from_client, &from_client_len);

        // get ip adress and convert to string
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&from_client;
        int ipAddr = pV4Addr->sin_addr.s_addr;
        char str[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );

        handle_client(nsd, str);
        }
    return nsd;
}

static int handle_client(int sd, char* ipAdress) {
    printf("%s: %s\n", ipAdress, "client connected!");
    char buf[BUFSIZE];
    int cc; //Character count
    while( (cc = read(sd, buf, BUFSIZE)) > 0){
        //read drei fälle
        if(cc < 0){ // -> Das muss dann außerhalb der Schleife sein
            printf("Error");
        }else if(cc == 0){
            //Kommunikationspartner hat die Verbindung beendet
            printf("%s\n", "communication partner closed the connection!");
        }

        // read file and convert to string

        FILE *fp;
        long lSize;
        char *buffer;

        fp = fopen ( file_name , "rb" );
        if( !fp ) perror(file_name),exit(1);

        fseek( fp , 0L , SEEK_END);
        lSize = ftell( fp );
        rewind( fp );

        /* allocate memory for entire content */
        buffer = calloc( 1, lSize+1 );
        if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

        /* copy the file into the buffer */
        if( 1!=fread( buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

        /* do your work here, buffer is a string contains the whole text */

        // cc-2 weil vom telnet immer noch /n (Return) abgezogen werden muss!
        int length = cc-2;
        printf("%s: client to server: %.*s\n", ipAdress, length, buf);
        printf("%s: server to client: %s\n", ipAdress, buffer);
        
        if(write(sd,buffer,lSize)<0){
            printf("%s\n", "Writing to the client went wrong!");
        }
        // close file buffer
        fclose(fp);
        free(buffer);
    }
    printf("%s: %s\n", ipAdress, "client disconnected!");
    close(sd);
    return(sd);
}