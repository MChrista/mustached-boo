
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

#include "libsockets/connect_tcp.h"



int main(int argc, char **argv){
    int port;
    port = atoi(argv[2]);
    char *file_name = argv[3];
    
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
    //int length = cc-2;

    
    // close file buffer
    
    //Socket Descriptor
    //sd < 0 Fehler
    int sd= connect_tcp(argv[1],port);
    if(sd<0){
        printf("Failed to create Socket\n");
    }else{
        printf("Success by creating Socket\n");
        
    }
    
    if(write(sd,buffer,lSize)<0){
        printf("%s\n", "Writing to the client went wrong!");
    }
    
    printf("client to Server: %s\n\n", buffer);
    
    int cc;
    char buf[BUFSIZE];
    while( (cc = read(sd, buf, BUFSIZE)) > 0){
        int length = cc;
        printf("server to client: %.*s\n", length, buf);
    }
    fclose(fp);
    free(buffer);
    close(sd);
    exit(0);
    
}
