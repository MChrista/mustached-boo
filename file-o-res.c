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
#include<time.h>
#include "libsockets/passive_tcp.h"
#include "libsockets/connect_tcp.h"
#include "libsockets/socket_info.h"
#include "libsockets/socket_io.h"

#include "helpers/FileHandler.h"

// declare here for usage before implementation
static int handle_client(int sd, char* ipAdress);
static int accept_client(int sd);
static int return_http_message(int sd, char* ipAdress, int port);
char *file_name;

int main(int argc, char **argv) {
    if (argc != 3) {
        // program name and two arguments aren't given
        printf("Invalid number of arguments!\n");
        exit(0);
    } else if (!(access(argv[2], F_OK) != -1)) {
        // file doesn't exist
        printf("Given file doesn't exist!\n");
        exit(0);
    }
    file_name = argv[2];
    int port, sd;
    port = atoi(argv[1]); //make first argument the port number

    sd = passive_tcp(port, 5);
    if (sd < 0) {
        // server creation went wrong
        printf("Error creating server\n");
        exit(0);
    }
    accept_client(sd);

    exit(0);
}

static int accept_client(int sd) {
    printf("Server ready to accept clients!\n");
    int retcode, nsd;
    struct sockaddr_in from_client;

    while (1) {
        int from_client_len = sizeof (from_client);
        nsd = accept(sd, (struct sockaddr*) &from_client, &from_client_len);
        // get ip adress and convert to string
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*) &from_client;
        int ipAddr = pV4Addr->sin_addr.s_addr;
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

        int portNumber = ntohs(from_client.sin_port);

        int pid, i, j;
        pid = fork();

        if (pid == 0) {
            /*
             * Kindprozess
             */
            return_http_message(nsd, str, portNumber);
            //handle_client(nsd, str);
            exit(0);

        } else if (pid > 0) {
            /*
             * Elternprozess
             */

        } else {
            /*
             * Error
             */
            exit(0);

        }


    }
    return nsd;
}

static int return_http_message(int sd, char* ipAdress, int port) {
    
    printf("%s:%d: %s\n", ipAdress, port, "client connected!");

    // getting and formating the time to rfc 2616
    time_t rawtime;
    struct tm * timeinfo;
    char timeString [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (timeString,80,"%a, %d %b %Y %T %z %p.",timeinfo);
    //printf("%s\n", buffer);
    
    char body[BUFSIZE] = "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
            "</head>"
            "<body><h1>";  

    strcat(body, timeString);
    strcat(body, "</h1></body></html>\r\n");
    
    char header[] = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n";   
    
    char integer_string[32];
    int integer = strlen(body)-1;
    sprintf(integer_string, "%d", integer);
    
    char response[BUFSIZE];
    
    
    strcpy(response,header);
    strcat(response,"Content-Length: ");
    strcat(response, integer_string);
    strcat(response,"\r\n\r\n");
    strcat(response,body);
    
    
    
    /*
    char response[] = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Content-Length: 1000\r\n\r\n"
            "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
            "<style>body { background-color: #111 }"
            "h1 { font-size:4cm; text-align: center; color: black;"
            " text-shadow: 0 0 2mm red}</style></head>"
            "<body><h1>Goodbye, world!</h1></body></html>\r\n";
    */

    if (write(sd, response, strlen(response)-1) < 0) {
        printf("%s\n", "Writing to the client went wrong!");
    }
    printf("%s:%d: %s\n", ipAdress, port, "client disconnected!");
    close(sd);

}

static int handle_client(int sd, char* ipAdress) {
    printf("%s: %s\n", ipAdress, "client connected!");
    char buf[BUFSIZE];
    int cc; //Character count
    while ((cc = read(sd, buf, BUFSIZE)) > 0) {
        //read drei fälle
        if (cc < 0) { // -> Das muss dann außerhalb der Schleife sein
            printf("Error");
        } else if (cc == 0) {
            //Kommunikationspartner hat die Verbindung beendet
            printf("%s\n", "communication partner closed the connection!");
        }

        // read file and convert to string
        int fileSize=0;
        char buffer[BUFSIZE];

        //Hier koennte man noch überprüfen, ob das lesen erfolgreich war
        openFile(buffer,file_name,&fileSize);

        /* do your work here, buffer is a string contains the whole text */

        // cc-2 weil vom telnet immer noch /n (Return) abgezogen werden muss!
        int length = cc - 2;
        printf("%s: client to server: %.*s\n", ipAdress, length, buf);
        printf("%s: server to client: %.*s\n", ipAdress, fileSize,buffer);

        if (write(sd, buffer, fileSize) < 0) {
            printf("%s\n", "Writing to the client went wrong!");
        }
    }
    printf("%s: %s\n", ipAdress, "client disconnected!");
    close(sd);
    return (sd);
}