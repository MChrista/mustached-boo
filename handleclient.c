#include <stdio.h>
#define BUFSIZE 1000
int handle_client(int sd){
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
