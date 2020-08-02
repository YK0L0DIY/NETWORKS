/*
** Exemplo com a funcao select
*  Neste caso monitorizamos um conjunto descritores de leitura
*  a funcao vai retornar quando um dos decritores tiver conteudo pronto
*  para ser lido (ou no caso de timeout)
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN 0  // file descriptor for standard input

int main(void) {
    char buffer[256];
    struct timeval tv;
    fd_set readfds;
    int retval;

    tv.tv_sec = 6;
    tv.tv_usec = 0;

    // inicializacao do conjunto de descritores de leitura
    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);

    /*
      este exemplo monitoriza apenas um descritor (stdin)
      bastaria acrecentar outros descritores a readfds, com FD_SET
     */

    printf("> a espera de input (seguido de enter)\n");
    fflush(0);

    while (1) {
        // o 1. argumento e' o Maximo descritor do conjunto +1
        retval = select(STDIN + 1, &readfds, NULL, NULL, &tv);

        // em seguida verifica-se qual dos descritores ficou pronto
        // para leitura (ou se houve erro ou timeout)

        if (retval == -1) {
            perror("select");
        } else if (FD_ISSET(STDIN, &readfds)) {
            // consumir aquele input (Ã‰ IMPORTANTE CONSUMIR, ou ficarÃ¡ sempre disponÃ­vel)
            int lidos = read(STDIN, buffer, 256);
            buffer[lidos - 1] = '\0';
            // que depois podia usar ou escrever noutro lado...
            printf(" data from STDIN descriptior: %s\n", buffer);
            // ...
            FD_SET(STDIN, &readfds);
            tv.tv_sec = 6;
        } else {
            printf("Timed out (6 segundos)\n");
            break;
        }
    }

    return 0;
}
