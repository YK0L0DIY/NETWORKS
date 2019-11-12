//
// Created by yk0l0diy on 12/11/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define STDIN 0  // file descriptor for standard input

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argv[2]) {
        portno = atoi(argv[2]);
    } else portno = 1300;

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    if (argv[1]) {
        server = gethostbyname(argv[1]);
    } else server = gethostbyname("localhost");

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(2);
    }

    fd_set readfds;
    int retval;

    // inicializacao do conjunto de descritores de leitura
    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);
    FD_SET(sockfd, &readfds);


    while (1) {
        // o 1. argumento e' o Maximo descritor do conjunto +1
        retval = select(sockfd + 1, &readfds, NULL, NULL, NULL);

        // em seguida verifica-se qual dos descritores ficou pronto
        if (retval == -1) {
            perror("select");
        } else if (FD_ISSET(STDIN, &readfds)) {
            // consumir aquele input (Ã‰ IMPORTANTE CONSUMIR, ou ficarÃ¡ sempre disponÃ­vel)
            int lidos = read(STDIN, buffer, 256);
            buffer[lidos - 1] = '\0';
            // que depois podia usar ou escrever noutro lado...
            FD_SET(STDIN, &readfds);
            send(sockfd, buffer, strlen(buffer), 0);
//            printf("%s\n", buffer);
            bzero(buffer, 256);

        } else if (FD_ISSET(sockfd, &readfds)) {
            n = read(sockfd, buffer, 255);

            if (n < 0) {
                perror("ERROR reading from socket");
                exit(3);
            }

            printf("%s", buffer);
            FD_SET(sockfd, &readfds);
        }
    }
    return 0;
}