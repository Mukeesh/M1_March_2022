/**
 * @file Server.c
 * @author Mukeesh K (mmuki651@gmail.com)
 * @brief This is the main program where the project is run
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "structure.h"



void initArray();
void insertArray();
void freeArray();
void initArrayRound();
void insertArrayRound();
void freeArrayRound();
void waitForClients();
void game();
void XMLParser();


/* el puerto usado */
#define PORT 2024

/* el código de error devuelto por ciertas llamadas */
extern int errno;

/* Threads */
pthread_t tid[5];		// Max 50 simultaneous games (with infinite players each)

/* struct for the players */

/* global variables to be able to catch players all the time */



/* Start of the application */
/**
 * @brief Main program
 * @return int
 * @return int 
 */
int main(){
	int i, j;
	int current_game = 0;
	FILE *XML_questions;

	for(i = 0; i < 50; i++){
		initArray(&players[i], 2);
	}

	initArrayRound(&rounds, 2);

	srand(time(NULL));

	/* Opening the file with the questions */
	if(!(XML_questions = fopen("/Users/jesusjimsa/Dropbox/Documentos/Universidad/3 - Primer cuatrimestre/Computer Networks/Teoría/Ejercicios/Quiz-Game/data/Q&A.xml", "r"))){
		perror("[server]Error opening file with questions\n");
		return errno;
	}

	XMLParser(XML_questions);	// We parse the file with the questions
	fclose(XML_questions);		// We don't need this file anymore, so we can close it

	/* creando un socket */
	if((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
		perror("[server]Error in socket().\n");
		return errno;
	}

	/* preparación de estructuras de datos */
	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));

	/* llene la estructura utilizada por el servidor */
	/* estableciendo la familia de sockets */
	server.sin_family = AF_INET;

	/* aceptamos cualquier dirección */
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	/* usamos un puerto de usuario */
	server.sin_port = htons(PORT);

	/* adjuntamos el socket */
	if(bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1){
		perror("[server]Error in bind().\n");
		return errno;
	}

	/* le pedimos al servidor que escuche si los clientes vienen a conectarse */
	if(listen(sd, 5) == -1){
		perror ("[server]Error in listen().\n");
		return errno;
	}

	/* Creation of the first thread that will handle the game */
	int errG[100];
	
	for(i = 0; i < 100; i++){
		current_game = i;

		errG[i] = pthread_create(&(tid[i]), NULL, &game, &current_game);

		if(errG[i] != 0){
			printf("\nCan't create thread :[%s]", strerror(errG[i]));
		}

		/*
			Waiting for the signal that last game has begun,
			next one can begin looking for players
		*/
		while(semaphore == -1){
			printf("%c", EMPTY);
		}

		sleep(2);

		semaphore = -1;
	}

	for(j = 0; j < 50; j++){
		for(i = 0; i < players[j].used; i++){
			close(players[j].array[i].id);
		}
	}

	for(i = 0; i < 50; i++){
		freeArray(&players[i]);
	}

	freeArrayRound(&rounds);
	close(sd);
}
