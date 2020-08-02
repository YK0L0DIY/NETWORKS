#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>

int main(int argc,char *argv[]){ 
	int sockfd, portno,n; 
	struct sockaddr_in serv_addr;
    struct hostent *server; 

    char buffer[256];
    portno=13;

	// socket create and varification --------------------------- step 1
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd <0 ) { 
		printf("socket creation failed...\n"); 
		exit(1); 
	} 
	
    server=gethostbyname("time.nist.gov");

    if (server == NULL){
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; 
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr,server->h_length);
	serv_addr.sin_port = htons(portno); 

	// connect the client socket to server socket ---------------- step 2
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0) { 
		printf("connection with the server failed...\n"); 
		exit(2); 
	} 
	else{ // send/receive ---------------------------------------- step 3
        bzero(buffer, sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer));  
        printf("From Server : %s", buffer); 
        close(sockfd);
    }
    
} 

/*
	A estrutura basica de um TCP cliente consiste em :
	1) criar um socket (valido), de seguida especificar o host a qual pretendemos fazer a coneção,
	2) fazer a coneção etre client - server
	3) neste caso dar print de response
*/
