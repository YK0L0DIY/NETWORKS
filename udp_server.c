#include <stdio.h> 
#include <netdb.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#define PORT 39873
#define MSG_SIZE 1024 

int main() {

	int socketfd;
	char *server_msg = "Hello from the server.";

	//Contains the info about the servaddr I want to connect to.
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;

	//Creating the socket.
	if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("SERVER ERROR: socket not created. \nAborting.");
		exit(EXIT_FAILURE);
	}

	//Setting server and client info to 0.
	bzero(&servaddr, sizeof(servaddr));
	bzero(&cliaddr, sizeof(cliaddr));

	//Assigning the server info.
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	//Binding the server to the port.
	if((bind(socketfd, (const struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
		perror("SERVER ERROR: port not bind.");
		exit(EXIT_FAILURE);
	}

	int msg_received, msg_received_length;
	char buffer[MSG_SIZE];

	//Copies the message received to the buffer.
	msg_received = recvfrom(socketfd, (char *) buffer, MSG_SIZE, MSG_WAITALL, (struct sockaddr *) &cliaddr, (socklen_t*) &msg_received_length);
	buffer[msg_received] = '\0';

	//Prints the message received.
	printf("Client: %s\n", buffer);

	//Sends a message to the client.
	sendto(socketfd, (const char *)server_msg, strlen(server_msg),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            msg_received_length); 

   	printf("Message: \"%s\" sent.\n", server_msg);
      
    return 0; 



}