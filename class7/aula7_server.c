//
// Created by yk0l0diy on 29/10/19.
//

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

#define TIM 10
#define PORT 1300

void strdate(char *buffer, int len) {
    time_t now = time(NULL);
    struct tm *ptm = localtime(&now);

    if (ptm == NULL) {

        puts("The localtime() function failed");
        return;
    }

    strftime(buffer, len, "%c\n", ptm);

}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;

    int opt = 1;      // for setsockopt() SO_REUSEADDR, below
    int addrlen = sizeof(address);
    char buffer[256];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 1300
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    //variaveis para o select
    struct timeval tv;
    fd_set sockets;
    int retval;

    tv.tv_sec = TIM;
    tv.tv_usec = 0;

    // inicializacao do conjunto de descritores de leitura
    FD_ZERO(&sockets);
    FD_SET(server_fd, &sockets);

    int maior=0;
    while (1) {

        // o 1. argumento e' o Maximo descritor do conjunto +1
        retval = select(server_fd + 1, &sockets, NULL, NULL, &tv);

        // em seguida verifica-se qual dos descritores ficou pronto
        // para leitura (ou se houve erro ou timeout)

        if (retval == -1) {
            perror("select");
        } else if (FD_ISSET(server_fd, &sockets)) {
            // consumir aquele input (Ã‰ IMPORTANTE CONSUMIR, ou ficarÃ¡ sempre disponÃ­vel)
            if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                     (socklen_t * ) & addrlen)) < 0) {
                perror("accept failed");
                if (errno == EINTR) {
                    printf("interrupt by signal\n");
                } else {
                    exit(EXIT_FAILURE);
                }
            }
            printf("New client connected.\n");

            FD_SET(server_fd, &sockets);
            FD_SET(new_socket,&sockets);
            tv.tv_sec = TIM;
            if(new_socket>maior) maior=new_socket;

        } else {
            strdate(buffer, 256);
            for (int x=0;x<=maior;x++){
                if(FD_ISSET(x,&sockets)) send(x, buffer, strlen(buffer), 0);

            }
            printf("Date sent to client\n");
            FD_SET(server_fd, &sockets);
            tv.tv_sec = TIM;

        }
    }
    close(new_socket);


    return 0;
}