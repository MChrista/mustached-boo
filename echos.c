#include <netinet/in.h>
static int accept_clients(int sd){
    int retcode,nsd; //New Socket Descriptor
    struct sockaddr_in from_client; //in = ipv4
    
    
    while(1){
        int from_client_len = sizeof(from_client);
        nsd = accept(sd/*in*/,(struct sockaddr*)&from_client/*in out*/, &from_client_len/*in out*/); //generische Struktur wird erwartet, Länge der Socketaddresse
                
        //Fehler bei nsd <0 -> Fehler: break
                
                
        //Immer nur ein Client kann am Server arbeiten. Es empfiehlt sich über fork den neuen Prozess einzuhängen        
        handle_client(nsd);
        
    }
    return nsd;
}

int main(int argc, char **argv){
    int port;
    port = atoi(argv[1]);
    
    //Socket Descriptor
    //sd < 0 Fehler
    int sd= passive_tcp(port);
    if(sd<0){
        print("error");
    }else{
        
    }
    
    accept_clients(sd);
    //exit(0);
    return 0;
    
}

//Static bedeutet, dass es in anderen Modulen nicht sichtbar ist
/*
static int handle_client(int sd){
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
*/
