#include<stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "structure.h"
#define true (1 == 1)
#define false (!true)
void initArray(Array *a, size_t initialSize){
	a->array = (struct Player *)malloc(initialSize * sizeof(struct Player));
	a->used = 0;
	a->size = initialSize;
}

void insertArray(Array *a, struct Player element){
	// a->used is the number of used entries, because a->array[a->used++] updates a->used only after the array has been accessed.
	// Therefore a->used can go up to a->size
	if(a->used == a->size){
		a->size *= 2;
		a->array = (struct Player *)realloc(a->array, a->size * sizeof(struct Player));
	}

	a->array[a->used++] = element;
}

void freeArray(Array *a){
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}

void initArrayRound(ArrayRound *a, size_t initialSize){
	a->array = (struct Round *)malloc(initialSize * sizeof(struct Round));
	a->used = 0;
	a->size = initialSize;
}

void insertArrayRound(ArrayRound *a, struct Round element){
	// a->used is the number of used entries, because a->array[a->used++] updates a->used only after the array has been accessed.
	// Therefore a->used can go up to a->size
	if(a->used == a->size){
		a->size *= 2;
		a->array = (struct Round *)realloc(a->array, a->size * sizeof(struct Round));
	}

	a->array[a->used++] = element;
}

void freeArrayRound(ArrayRound *a){
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}

void waitForClients(int *current_game){
	struct Player aux;

	aux.score = 0;

	while(true){
		aux.id = accept(sd, (struct sockaddr *) &from, &length);

		/* error accepting the connection of a client */
		if(aux.id < 0){
			perror("[server]Error in accept().\n");
			continue;
		}

		if(recv(aux.id, &aux.username, sizeof(aux.username), 0) <= 0){
			perror("[server]Error in recv() from client.\n");
			close(aux.id);	/* close the connection with the client */
			continue;		/* keep looking for clients */
		}

		insertArray(&players[*current_game], aux);
		printf("Player connected: %s\n", aux.username);
		memset(aux.username, 0, sizeof(aux.username));
	}
}

void game(int *current_game){
	pthread_t searching;
	int i, j;
	int add_points;
	int current = *current_game;
	const int numberOfRounds = 10;
	char *result = "\0";
	char *resultUntilNow = "\0";
	char *newResult = "\0";
	
	printf("Waiting for at least two players\n");

	/* Creation of a thread that will keep creating players */
	int err;
	err = pthread_create(&(searching), NULL, &waitForClients, &(*current_game));

	if(err != 0){
		printf("\nCan't create thread :[%s]", strerror(err));
	}

	// The server will wait until there is at least two players
	while(players[*current_game].used < 2){
		/*
			For some reason, if it doesn't prints anything, it doesn't works.
			To fix this, I print something, but it is the empty character '\0',
			then, nothing is actually printed, but it works.
		*/
		printf("%c", EMPTY);
	}

	printf("Now, we will wait ten seconds to wait for more players\n");
	sleep(5);	// After two players have joined, we give 10 seconds to the rest of the players to join in time
	printf("Five more seconds to begin\n");
	sleep(5);
	printf("Let's go!\n");

	pthread_cancel(searching);	// We won't wait any more players in this game

	semaphore = 1;	// The next game can begin

	for(i = 0; i < numberOfRounds; i++){
		for(j = 0; j < players[current].used; j++){
			// We send a random question to the player
			if(send(players[current].array[j].id, &rounds.array[rand() % rounds.used], sizeof(rounds.array[rand() % rounds.used]), 0) <= 0){
				perror("[server]Error in send() to client.\n");
				break;
			}			
		}

		for(j = 0; j < players[current].used; j++){
			// We receive the points obtained with this question
			if(recv(players[current].array[j].id, &add_points, sizeof(int), 0) <= 0){
				perror("[server]Error in recv() from client.\n");
				break;
			}

			players[current].array[j].score += add_points;

			if(i < (numberOfRounds - 1)){
				int not_finish = 1;

				// We send the signal of not finishing the game
				if(send(players[current].array[j].id, &not_finish, sizeof(int), 0) <= 0){
					perror("[server]Error in send() to client.\n");
					break;
				}
			}
			else{
				int finish = -1;

				// The game finishes
				if(send(players[current].array[j].id, &finish, sizeof(int), 0) <= 0){
					perror("[server]Error in send() to client.\n");
					break;
				}
			}
		}
	}

	for(i = 0; i < players[current].used; i++){
		if((newResult = malloc((strlen(players[current].array[i].username) + 25) * 2)) != NULL){
			newResult[0] = '\0';

			sprintf(newResult, "%s –– %d points\n", players[current].array[i].username, players[current].array[i].score);
		}
		else{
			perror("Malloc failed!\n");
			exit(0);
		}
		
		if ((result = malloc((strlen(resultUntilNow) + strlen(newResult) + 1) * 2)) != NULL){
			result[0] = '\0';	// Ensures the memory is an empty string
			
			strcat(result, resultUntilNow);
			strcat(result, newResult);

			if((resultUntilNow = malloc((strlen(result) + 1) * 2)) != NULL){
				resultUntilNow[0] = '\0';

				strcpy(resultUntilNow, result);
			}
			else{
				perror("Malloc failed!\n");
				exit(0);
			}
			
		}
		else{
			perror("Malloc failed!\n");
			exit(0);
		}
	}

	for(i = 0; i < players[current].used; i++){
		if(send(players[current].array[i].id, result, strlen(result), 0) <= 0){
			perror("[client]Error in send() to server.\n");
			break;
		}
	}

	free(result);
	free(resultUntilNow);
	free(newResult);
}

void XMLParser(FILE *XML_questions){
	char line[256];
	char lineQ[13];
	char lineOA[13];
	char lineOB[13];
	char lineOC[13];
	char lineOD[13];
	char lineAns[11];
	struct Round ronda;
	int i;

 	while(fgets(line, sizeof(line), XML_questions)) {
		//This is done to choose detect the labels
		for(i = 0; i < 12; i++){
			lineQ[i] = line[i];
			lineOA[i] = line[i];
			lineOB[i] = line[i];
			lineOC[i] = line[i];
			lineOD[i] = line[i];

			if(i < 10){
				lineAns[i] = line[i];
			}
		}

		lineQ[12] = '\0';
		lineOA[12] = '\0';
		lineOB[12] = '\0';
		lineOC[12] = '\0';
		lineOD[12] = '\0';
		lineAns[10] = '\0';

		if(!strcmp("<quiz>\n", line)){
			continue;
		}
		else{
			if(!strcmp("\t<round>\n", line)){
				continue;
			}
			else{
				if(!strcmp("\t\t<question>", lineQ)){
					int j = 12;

					while(line[j] != '<' && line[j + 1] != '/'){
						ronda.question[j - 12] = line[j];

						j++;
					}

					ronda.question[j - 12] = '\0';
				}
				else{
					if(!strcmp("\t\t<options>\n", line)){
						continue;
					}
					else{
						if(!strcmp("\t\t\t<optionA>", lineOA)){
							int j = 12;

							while(line[j] != '<' && line[j + 1] != '/'){
								ronda.A[j - 12] = line[j];

								j++;
							}

							ronda.A[j - 12] = '\0';
						}
						else{
							if(!strcmp("\t\t\t<optionB>", lineOB)){
								int j = 12;

								while(line[j] != '<' && line[j + 1] != '/'){
									ronda.B[j - 12] = line[j];

									j++;
								}

								ronda.B[j - 12] = '\0';
							}
							else{
								if(!strcmp("\t\t\t<optionC>", lineOC)){
									int j = 12;

									while(line[j] != '<' && line[j + 1] != '/'){
										ronda.C[j - 12] = line[j];

										j++;
									}

									ronda.C[j - 12] = '\0';
								}
								else{
									if(!strcmp("\t\t\t<optionD>", lineOD)){
										int j = 12;

										while(line[j] != '<' && line[j + 1] != '/'){
											ronda.D[j - 12] = line[j];

											j++;
										}

										ronda.D[j - 12] = '\0';
									}
									else{
										if(!strcmp("\t\t</options>\n", line)){
											continue;
										}
										else{
											if(!strcmp("\t\t<answer>", lineAns)){
												ronda.correct_answer = line[10];
											}
											else{
												if(!strcmp("\t</round>\n", line)){
													insertArrayRound(&rounds, ronda);
												}
												else{
													if(!strcmp("\n", line)){
														continue;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
    }
}

void printRound(struct Round ronda){
	printf("%s\n", ronda.question);
	printf("A. %s\tB. %s\nC. %s\tD. %s\n", ronda.A, ronda.B, ronda.C, ronda.D);
	printf("Which option is the right answer, A, B, C or D?\n");
}

int printResult(struct Round ronda, int answer){
	int points_obtained;
	printf("The right answer was: %s\n", &ronda.correct_answer);
	
	if(answer == true){
		printf("\nYou get 10 points in this round\n");
		printf("\n–––––––––––––––––––––––––––––––––––––––––––––––\n\n");
		points_obtained = 10;
	}
	else{
		printf("\nYou lose 2 points in this round\n");
		printf("\n–––––––––––––––––––––––––––––––––––––––––––––––\n\n");
		points_obtained = -2;
	}

	return points_obtained;
}
