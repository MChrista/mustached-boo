int main(int argc, char **argv) {
	int port;
	port = atoi(argv[1]);
	int sd = passive_tcp(port);
	if(sd<0) {
		// Fehlerhandling
	}
	accept_clients(sd);
	exit(0);
}
static int accept_client(int sd) {
	int returnCode,d
