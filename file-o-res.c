#define BUFSIZE 1000

#include<stdio.h>
#include<stdlib.h>
//#include<string.h>
//#include<sys/socket.h>
#include<sys/types.h>
//#include<sys/errno.h>
#include<sys/signal.h>
#include<sys/time.h>
#include<sys/resource.h>
//#include<netinet/in.h>
//#include<arpa/inet.h>
//#include<netdb.h>
#include<unistd.h>
//#include<stdarg.h>
#include<time.h>
#include <bits/signum.h>
#include <sys/wait.h>
//#include "libsockets/passive_tcp.h"
//#include "libsockets/connect_tcp.h"
#include "libsockets/socket_info.h"
//#include "libsockets/socket_io.h"

//#include "helpers/FileHandler.h"

// declare here for usage before implementation
static int accept_client(int sd);
static int handle_client(int sd, char* ipAddress);
static int return_http_message(int sd, char* ipAddress, int port);
void handler(int sig);
char *file_name; /* pointer to the response file */

/*
 * Main function
 * arguments: port number and response file
 */
int main(int argc, char **argv) {
    if (argc != 3) { /* too few or too many arguments */
        printf("Invalid number of arguments!\n");
        exit(0);
    } else if (!(access(argv[2], F_OK) != -1)) { /* file doesn't exist */
        printf("Given file doesn't exist!\n");
        exit(0);
    } else { /* creating socket */
        int port, sd;           /* port and socket descriptor */
        port = atoi(argv[1]);   /* first argument is the port number */
        file_name = argv[2];    /* second argument is the response file */
        sd = passive_tcp(port, 5);
        if (sd < 0) { /* socket creation went wrong */
            printf("Error creating socket\n");
            exit(0);
        }
        /* start accepting clients on the socket */
        accept_client(sd);
    }
    exit(0);
}

/*
 * accept clients on socket
 * arguments: socket descriptor
 */
static int accept_client(int sd) {
    printf("Server ready to accept clients!\n");
    int retcode, nsd; /* return code and scocket descriptor */
    struct sockaddr_in from_client; /* an internet endpoint address */
    int from_client_len = sizeof (from_client); /* size of struct */
    struct socket_info si;
    signal(SIGCHLD, handler);
    while (1) { /* while true accept clients */
        nsd = accept(sd, (struct sockaddr*) &from_client, &from_client_len);
        
        /* get client ip adress and convert it */
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*) &from_client;
        int ipAddr = pV4Addr->sin_addr.s_addr;
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
        /* get client port number */
        int portNumber = ntohs(from_client.sin_port);

        int pid; /* process id */
        pid = fork();

        if (pid == 0) { /* child process */
            close(sd);
            struct rusage ru;
            struct timeval utime;
            struct timeval stime;
            struct timeval tval_before, tval_after, tval_result;
            gettimeofday(&tval_before, NULL);
            return_http_message(nsd, str, portNumber);
            //handle_client(nsd, str);
            gettimeofday(&tval_after, NULL);
            timersub(&tval_after, &tval_before, &tval_result);
            getrusage(RUSAGE_SELF, &ru);
            utime = ru.ru_utime;
            stime = ru.ru_stime;
            printf("%s %d\tuser time: %ld.%06ld\n\t\tsyst time: %ld.%06ld\n\t\treal time: %ld.%06ld\n", "process\t", getpid(), 
                (long int)utime.tv_sec, (long int)utime.tv_usec, 
                (long int)stime.tv_sec, (long int)stime.tv_usec,
                (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
            exit(0);

        } else if (pid > 0) { /* parent process */
            close(nsd);
            // TODO: SIGHANDLER SIGCHILD
        } else { /* error while forking */
            exit(0);
        }
    }
    return nsd;
}

static int return_http_message(int sd, char* ipAddress, int port) {
    printf("%s:%d: %s\n", ipAddress, port, "client connected!");

    /* get time and format to rfc 2616 */
    time_t rawtime;
    struct tm * timeinfo;
    char timeString [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (timeString,80,"%a, %d %b %Y %T %z %p.",timeinfo);
    //printf("%s\n", buffer);
    
    char body[BUFSIZE] = "<!DOCTYPE html><html><head><title>Timestamp</title>"
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

    if (write(sd, response, strlen(response)-1) < 0) {
        printf("%s\n", "Writing to the client went wrong!");
    }
    printf("%s:%d: %s\n", ipAddress, port, "client disconnected!");
    close(sd);
    return (sd);

}

static int handle_client(int sd, char* ipAddress) {
    printf("%s: %s\n", ipAddress, "client connected!");
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

        // cc-2 weil vom telnet immer noch /n (Return) abgezogen werden muss!
        int length = cc - 2;
        printf("%s: client to server: %.*s\n", ipAddress, length, buf);
        printf("%s: server to client: %.*s", ipAddress, fileSize, buffer);

        if (write(sd, buffer, fileSize) < 0) {
            printf("%s\n", "Writing to the client went wrong!");
        }
    }
    printf("%s: %s\n", ipAddress, "client disconnected!");
    close(sd);
    exit(sd);
}

void handler(int sig)
{
    while(waitpid(-1,NULL,WNOHANG)>0){
        
    }
}
