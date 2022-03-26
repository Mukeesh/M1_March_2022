#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "structure.h"

#define true (1 == 1)
#define false (!true)

void printRound();
int printResult();

/* Normalmente el puerto es el 2024 y la dirección es 127.0.0.1 */

/* el código de error devuelto por ciertas llamadas */
extern int errno;

/* puerto de acceso al servidor */
int port;



void test_quiz()
{
    printf("Successfully the project is tested");
}