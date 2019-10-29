#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h>        
#include <signal.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define PORT 1300

void strdate(char *buffer, int len)
{
    time_t now = time(NULL);
    struct tm *ptm = localtime(&now);
    
    if (ptm == NULL) {
        
        puts("The localtime() function failed");
        return;
    }

    strftime(buffer, len, "%c\n", ptm);

}

void teste(){
    int p=wait(NULL);
    printf("%d\n",p);
    return;
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket; 
    struct sockaddr_in address;
    
    int opt = 1;      // for setsockopt() SO_REUSEADDR, below
    int addrlen = sizeof(address); 
    char buffer[256];

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 1300 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt failed"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Bind the socket to the network address and port
    if (  bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0  ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen failed"); 
        exit(EXIT_FAILURE); 
    }

    struct sigaction sa;
    sa.sa_sigaction=teste;
    if(sigaction(SIGCHLD,&sa,NULL)==-1){
        perror("sigaction");
    }
    // Wait for a connection
    while(1){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                           (socklen_t*)&addrlen))<0) 
        { 
            perror("accept failed"); 
            if(errno==EINTR){
                printf("interrupt by signal\n");
                continue;
            }
            else{
                exit(EXIT_FAILURE); 
            }
        }
        int pid=fork();

        if(pid==0){
            printf("Client connected.\n");
            sleep(10);
            strdate(buffer, 256);
            send(new_socket, buffer, strlen(buffer), 0 ); 
            printf("Date sent to client\n");
            return 0;
        }
        close(new_socket);

    }
    return 0; 
} 