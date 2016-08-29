/* File: AMStartup.c
 *
 * Authors: Michael Ortiz, Galen Brown, and Alexander Wolf
 *
 * Description: AMStartup initializes a connection with the server and connects
 * each Avatar to the maze server. This program sends and receives message to 
 * and from the server. These messages received from the server are passed on to 
 * functions that determine the direction an Avatar will take. Also, AMStartup
 * is provided with a fucntions from GUI.c that draws the maze as it is being
 * explored. 
 *
 * Summer 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include "amazing.h"
#include "Functions.h"
#include "GUI.h"
#include <pthread.h>
#include <time.h>


/*************** Local functions ***************/
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
	gpointer user_data);
static gboolean time_handler(GtkWidget *widget);
void realtimedraw(cairo_t *cr, int x, int y, cell_t map[x][y], 
	int nAvatars, XYPos avPos[10], int turns);
void *initAvatar(void *data);
static void doDrawing(cairo_t *cr, gpointer user_data);


/*************** Global variables ***************/
cairo_surface_t* pic = NULL;
int gtkMazeDrawn=0;
int nAvatars;
GtkWidget *darea;
cairo_t *crGlobal;
uint32_t turns;
XYPos avPos[10];

/*************** Local types ***************/
struct info{
	int lastDir;
	int avatarID;
	int comm_sock;
	int mzPort;
	char *hostname;
	AM_Message *mzResponse;
};


int main (const int argc, char *argv[])
{
	int difficulty;
	char *hostname;
	int comm_sock;
	int port;
	int mazePort;
	struct sockaddr_in server;
	struct hostent *hostp;



	// Check argument count
	if (argc != 4) {
		printf("Error: Invalid number of arguments.\n");
		return 1;
	}
	
	else{
		nAvatars = atoi(argv[1]);
		difficulty = atoi(argv[2]);
		hostname = argv[3];
		port = atoi(AM_SERVER_PORT);

	}
	
	// Create Socket
	comm_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (comm_sock <= 0){
		printf("Error Opening Socket");
		return 2;
	}
	
	// Initialize the server address
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	
	hostp = gethostbyname(hostname);
	if (hostp == NULL){
		printf("Host not found\n");
		return 2;
	}
	
	memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);
	// Connect socket to server
	if (connect(comm_sock, (struct sockaddr *) &server, sizeof(server)) < 0){
		printf("Error connecting\n");
		return 2;
	}
	
	//Prepare initial message
	AM_Message *initMessage = calloc(1, sizeof(AM_Message));
	initMessage->type = htonl(AM_INIT);
	initMessage->init.nAvatars = htonl(nAvatars);
	initMessage->init.Difficulty = htonl(difficulty);
	
	//Send initiial message
	if (write(comm_sock, initMessage, sizeof(AM_Message)) <= 0){
		printf("Error sending message.\n");
		return 3;
	}

	// Read response from server
	AM_Message *response = calloc(1, sizeof(AM_Message));
	if (read(comm_sock, response, sizeof(AM_Message)) <= 0){
		printf("Error reading message.\n");
		return 4;
	}

	// Check Message type
	if (IS_AM_ERROR(response->type)){
		printf("Error Initializing.\n");
		return 5;
	}	

	int mazeWidth = ntohl(response->init_ok.MazeWidth);
	int mazeHeight = ntohl(response->init_ok.MazeHeight);
	mazePort = ntohl(response->init_ok.MazePort);
	
	//Prepare log file
	FILE *fp;
	char *user = getlogin();
	char *title = malloc(strlen(user) + 23);
	time_t mytime = time(NULL);
	struct tm *strtime;
	strtime = localtime(&mytime);
	sprintf(title, "logs/Amazing_%s_%d_%d.log", user, nAvatars, difficulty);
	fp = fopen(title, "w");
	fprintf(fp, "%s %d %s\n", user, ntohl(response->init_ok.MazePort), asctime(strtime));
	free(title);

	// Create thread for each avatar
	pthread_t threads[nAvatars];
	struct info *avatarGroup[nAvatars];
	int threadStatus;	
	for (int avatarID = 0; avatarID < nAvatars; avatarID++){
		
		struct info *avatarInfo = calloc(1, sizeof(struct info));
		
		// Store necesary information for later use
		avatarInfo->avatarID = avatarID;
		avatarInfo->mzPort = mazePort;
		avatarInfo->hostname = hostname;
		avatarInfo->mzResponse = calloc(1, sizeof(AM_Message));
	
		//Create thread
		threadStatus = pthread_create(&threads[avatarID],
				 NULL, initAvatar, avatarInfo);

		if (threadStatus != 0){
			printf("Error creating thread\n");
			return 6;
		}

		avatarGroup[avatarID] = avatarInfo;
		
		sleep(.2);
	}
	

	// Join the threads
	for (int j = 0; j < nAvatars; j++){
		pthread_join(threads[j], NULL);

	}

	// Quick clean up
	free(initMessage);
	free(response);
		

	// Basic loop for avatars to take turns 
	int i = 0;
	int currTurn = 0;
	int read_comm_sock = avatarGroup[0]->comm_sock;

	pthread_mutex_t turnMutex = PTHREAD_MUTEX_INITIALIZER;
	AM_Message *updatedResponse; // will hold the message from server
	updatedResponse = avatarGroup[0]->mzResponse;
	
	// Initialize struct and array needed for logic and drawing functions
	int leaders[nAvatars];
	cell_t test[mazeWidth][mazeHeight];
	maze_setup(mazeWidth, mazeHeight, nAvatars, test);

	for (int x = 0; x < nAvatars; x++){
		leaders[x] = 0;
	}

	//transfer data to GUI
	passData(mazeWidth, mazeHeight, nAvatars, argv[2]);

	//prepare gtk
	int dummyc=1;
	gtk_init(&dummyc, &argv);

	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), darea);

	//open connection the the gtk+ GUI
	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), test);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	initialize_window(window);

	//create .1 seond timout function to draw maze with data updates
	g_timeout_add(50, (GSourceFunc) time_handler, (gpointer) window);

	//show UI
	gtk_widget_show(window);

	while (1){

		//check for UI updates
		if(gtk_events_pending()){
			gtk_main_iteration();
		}
		
		//holds the message that will be sent to server 
		AM_Message *makeMove = calloc(1, sizeof(AM_Message));
		
		struct info *currAvatar = avatarGroup[currTurn];

		// Check whose turn is it
		if (currAvatar->avatarID == ntohl(updatedResponse->avatar_turn.TurnId)){
			pthread_mutex_lock(&turnMutex);
			fprintf(fp, "Avatar %d", currAvatar->avatarID);
			
			// Save the current's avatar position for later comparison			
			Avatar old;
			old.fd = currAvatar->avatarID;
			old.pos.x = ntohl(updatedResponse->avatar_turn.Pos[currAvatar->avatarID].x);
			old.pos.y = ntohl(updatedResponse->avatar_turn.Pos[currAvatar->avatarID].y);

			// Algorithm determines which direction the avatar will go
			int direction = alg(makeMove, mazeWidth, mazeHeight, test, old, nAvatars, 
				leaders, currAvatar->lastDir);

			fprintf(fp, ":(%d,%d) ", old.pos.x, old.pos.y);
			switch(direction){
				case 1:
					fprintf(fp, "Moving north.\n");
					break;
				case 2:
					fprintf(fp, "Moving east.\n");
					break;
				case 3:
					fprintf(fp, "Moving south.\n");
					break;
				case 4:
					fprintf(fp, "Moving west.\n");
					break;
				case 8:
					fprintf(fp, "Sleeping.\n");
					break;
			}

			// Tell server where avatar is moving
			if (write(currAvatar->comm_sock, makeMove, sizeof(AM_Message)) <= 0){
               			printf("Error sending message.\n");
                		return 7;
		        }
	
			if (updatedResponse != NULL){
				free(updatedResponse);
			}
			updatedResponse = calloc(1, sizeof(AM_Message));
		        
			// Obtain response from server
			if (read(read_comm_sock, updatedResponse, sizeof(AM_Message)) <= 0){
                		printf("Error reading message.\n");
                		return 7;
			}
		
			// Check if server returned an error message
			if (IS_AM_ERROR(updatedResponse->type)){
				printf("Server has encountered an error.\n");
				break;
			}
			
			// If maze was solved
			if (ntohl(updatedResponse->type) == AM_MAZE_SOLVED ){
	
				free(makeMove);
				turns = ntohl(updatedResponse->maze_solved.nMoves);
				fprintf(fp, "AM_MAZE_SOLVED %d %d %d %d", 
					ntohl(updatedResponse->maze_solved.nAvatars), 
					ntohl(updatedResponse->maze_solved.Difficulty),
					turns,
					ntohl(updatedResponse->maze_solved.Hash));

				/*** show success message***/
				gtk_main();

				//destory cairo surface
				cairo_surface_destroy(pic);

				break;
			}
			sleep(0);
			currTurn = ntohl(updatedResponse->avatar_turn.TurnId);

			// Save the avatar's new position for comparison
			Avatar new;
			new.pos.x = ntohl(updatedResponse->avatar_turn.Pos[currAvatar->avatarID].x);
			new.pos.y = ntohl(updatedResponse->avatar_turn.Pos[currAvatar->avatarID].y);
			new.fd = currAvatar->avatarID;

			for(int i = 0; i < nAvatars; i++){
				avPos[i].x = ntohl(updatedResponse->avatar_turn.Pos[i].x);
				avPos[i].y = ntohl(updatedResponse->avatar_turn.Pos[i].y);
				fprintf(fp, "%d:(%d,%d) ", i, avPos[i].x, avPos[i].y);

				//draw the maze with new avatar/maze information on terminal
				drawMaze(mazeWidth, mazeHeight, test, nAvatars, avPos);
			}

			fprintf(fp, "\n");
			int moved = move(mazeWidth, mazeHeight, test, old, new, direction);

			if(moved){
				currAvatar->lastDir = moved;
			}
			pthread_mutex_unlock(&turnMutex);
		}
		
       		if (makeMove != NULL){ 
			free(makeMove);
		}
		i++;
		
		
	}

	// Quick clean up	
	for (int i = 0; i < nAvatars; i++){
		
		//Free any stored message received from server
		if (avatarGroup[i]->mzResponse != NULL && i !=0){
			free(avatarGroup[i]->mzResponse);
		}
		
		//Free the avatars
		free(avatarGroup[i]);
	}
	close(comm_sock);
	
	if (updatedResponse != NULL){

		free(updatedResponse);
	}
	
  	maze_delete(mazeWidth, mazeHeight, test);
	fclose(fp);
	printf("Success\n");
	return 0;
	
	
}

/* initAvatar is used when creating a thread. It creates a socket, 
 * initializes the maze server, and then connects the socket to the
 * server. An avatar then sends a message to the server with its turn ID, 
 * notifying it that is ready. Any necessary information obtained within this 
 * function that may be used later is stored in the avatar's struct "info".
 */
void *initAvatar (void *data)
{
	
	struct info *aInfo = data;
	struct sockaddr_in mzServer;
    struct hostent *hostp;
	int mz_comm_sock;
	
	aInfo->lastDir = 1;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);

	// Create Socket
    mz_comm_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (mz_comm_sock < 0){
            printf("Error Opening Socket\n");
            return NULL;
    }

    // Initialize the server address
	mzServer.sin_family = AF_INET;
    mzServer.sin_port = htons(aInfo->mzPort);
    hostp = gethostbyname(aInfo->hostname);
  	if (hostp == NULL){
                printf("Host not found\n");
                return NULL;
        }
    memcpy(&mzServer.sin_addr, hostp->h_addr_list[0], hostp->h_length);
        
	//Connect socket to server
    if (connect(mz_comm_sock, (struct sockaddr *) &mzServer, sizeof(mzServer)) < 0){
            printf("Error connecting\n");
            return NULL;
    }

	//Prepare avatar_ready message
	AM_Message *avatarReady = calloc(1, sizeof(AM_Message));
	avatarReady->type = htonl(AM_AVATAR_READY);
	avatarReady->avatar_ready.AvatarId = htonl(aInfo->avatarID);

	//Send Message
	if (write(mz_comm_sock, avatarReady, sizeof(AM_Message)) <= 0){
		printf("Error sending message.\n");
		return NULL;
	}

	//Obtain message from maze server
	if (read(mz_comm_sock, aInfo->mzResponse, sizeof(AM_Message)) <= 0){
		printf("Error reading message.\n");
		return NULL;
	}
	aInfo->comm_sock = mz_comm_sock;
	
	pthread_mutex_unlock(&mutex);
	free(avatarReady);
	
	return NULL;
}

//function to handle draw calls
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
	gpointer user_data)
{ 

	doDrawing(cr, user_data);
	return FALSE;
}

//function to handle draw calls
static void doDrawing(cairo_t *cr, gpointer user_data)
{ 

  //cast map data from gpointer
  cell_t (*map)[mazeH] = (cell_t(*)[mazeH]) user_data;

  //updating the maze
  realtimedraw(cr, mazeW, mazeH, map, nAvatars, avPos, turns);
}

//draw realtime
void realtimedraw(cairo_t *cr, int x, int y, cell_t map[x][y], 
	int nAvatars, XYPos avPos[10], int turns){
	
	//draw maze background then the maze
	initGTKMaze(cr, x, y, map);
	drawMazeGTK(cr, x, y, map, nAvatars, avPos, turns);
  	
}


//callback function for updating gtk ui
static gboolean time_handler(GtkWidget *widget)
{	
    gtk_widget_queue_draw(widget);
    return TRUE;

}
