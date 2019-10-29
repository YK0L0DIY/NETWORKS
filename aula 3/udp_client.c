#include <stdio.h> 
#include <netdb.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#define HOSTNAME "127.0.0.1"
#define PORT 39873
#define MSG_SIZE 1024 

int main() {

	int socketfd;
	char buffer[MSG_SIZE];
	char client_msg[] = "Hello from the client.";

	//Contains the info about the servaddr I want to connect to.
	struct sockaddr_in servaddr;
	struct hostent *server;

	//Creating the socket.
	if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("CLIENT ERROR: socket not created. \nAborting.");
		exit(EXIT_FAILURE);
	}

	//All the info about the servaddr is now 0.
	bzero(&servaddr, sizeof(servaddr));

	//Setting the info the of the servaddr I want to connect to.

	server = gethostbyname(HOSTNAME);
	if(server == NULL) {
		fprintf(stderr, "ERROR, no such host.\n");
		exit(1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&servaddr.sin_addr.s_addr, server->h_length);
	servaddr.sin_port = htons(PORT);
	int msg_received, msg_received_length;

	//Sending the message to the servaddr.
	sendto(socketfd, (const char *) client_msg, strlen(client_msg), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	printf("Message: \"%s\" sent.\n", client_msg);

	msg_received = recvfrom(socketfd, (char *) buffer, MSG_SIZE, MSG_WAITALL, (struct sockaddr *) &servaddr, (socklen_t*)&msg_received_length);
	buffer[msg_received] ='\0';
	printf("Server: %s\n", buffer);

	close(socketfd);

	return 0;
}