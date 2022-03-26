struct Player{
	int id;				// ID to connect to the player
	int score;			// Score obtained during the game
	char username[50];	// Username
};

struct Round{
	char question[100];
	char A[50];
	char B[50];
	char C[50];
	char D[50];
	char correct_answer;
};

/********* https://stackoverflow.com/a/3536261/7071193 *********/
typedef struct{
	struct Player *array;
	size_t used;
	size_t size;
} Array;


/*********************************/

/********* https://stackoverflow.com/a/3536261/7071193 *********/
typedef struct{
	struct Round *array;
	size_t used;
	size_t size;
} ArrayRound;


/*********************************/
struct sockaddr_in server;	// la estructura utilizada por el servidor
struct sockaddr_in from;
int sd;			//descriptor de socket
int semaphore = -1;
const char EMPTY = '\0';
socklen_t length = sizeof(from);
Array players[50];
ArrayRound rounds;

