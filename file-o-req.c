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
    exit(0);
    
}


static int accept_clients(int sd){
    int retcode,nsd; //New Socket Descriptor
    struct sockaddr_in from client; //in = ipv4
    
    
    while(1){
        from_client_len = sizeof(from_client);
        nsd = accept(sd,(struct sockaddr*)&from_client, &from_client_len) //generische Struktur wird erwartet, Länge der Socketaddresse
                
    }
}