/*Functions.c - dumping ground for shared functions. Current contents:
 *  cell_t- basic struct of a cell of the maze
 *  
 *  maze_setup- takes a double array of cells and builds an empty maze
 *  maze_delete- takes a double array of cells and frees them. 
 *  algorithm- takes an AM_MESSAGE and avatar information, determines the 
 *    direction for the avatar to take, and sets the message settings.
 *  logic- does the determining for algorithm.
 *  passed- checks if a leader is the first leader in a given cell
 *  reassign- adds the current leader to a new followers trail. This ensures
 *    that every explored cell stays marked as being explored by a leader.
 *  move- takes two avatar positions and updates the maze struct based on
 *    the result of their movement.
 *  seal- automatically closes dead ends.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include "amazing.h"
#include <pthread.h>
#include "Functions.h"

/*********** global variables ***************/

/*********** local variables ***************/

/*********** local types ***************/

/*********** global types ***************/

/*********** local functions ***************/
static int passed(cell_t cell, int avID, int total, int leaders[total]);

static void reassign(int x, int y, cell_t map[x][y], int avID, int tID);

/*********** global functions ***************/

/*********** maze_setup() ***************/
/* Builds the empty maze and fills in the outside walls*/
void maze_setup(int x, int y, int avs, cell_t in[x][y]){
  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      in[i][j].avs = malloc(sizeof(int) * avs);
      for(int k = 0; k < avs; k++){
        in[i][j].avs[k] = 0;
      }
      for(int r = 0; r < 4; r++){
        in[i][j].walls[r] = 0;
      }
    }
  }
  //set north wall
  for(int n = 0; n < x; n++){
    in[n][0].walls[0]=1;
  }

  //set south wall
  for(int s = 0; s < x; s++){
    in[s][y-1].walls[2]=1;
  }

  //set east wall
  for(int e = 0; e < y; e++){
    in[x-1][e].walls[1]=1;
  }

 //set west wall
  for(int w = 0; w < y; w++){
    in[0][w].walls[3]=1;
  }
}

/*********** maze_delete() ***************/
/* Frees the allocated memory for avs*/
void maze_delete(int x, int y, cell_t in[x][y]){
  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      free(in[i][j].avs);
    }
  }
}

/*********** algorithm() ***************/
/* Sends the move message and returns the direction taken. */
int alg(AM_Message *initMessage, int x, int y, cell_t map[x][y], 
    Avatar cur, int total, int leaders[total], int last){
  int dir;
  /* We use different direction codes than amazing.h This converts them.*/
  switch(logic(x, y, map, total, leaders, cur, last)){
    case 1:
      dir = 1;
      break;
    case 2:
      dir = 3;
      break;
    case 3:
      dir = 2;
      break;
    case 4:
      dir = 0;
      break;
    case 8:
      dir = 8;
      break;
  }
  //Set the message paramaters
  initMessage->type = htonl(AM_AVATAR_MOVE);
  initMessage->avatar_move.AvatarId = htonl(cur.fd);
  initMessage->avatar_move.Direction = htonl(dir);
  //Send the direction to AMStartup to be used in move.
  return (logic(x, y, map, total, leaders, cur, last));
}

int logic(int x, int y, cell_t map[x][y], int total, int leaders[total], 
  Avatar cur, int last){
  int avID = cur.fd;            //current avatar
  int cx, cy;
  cx = cur.pos.x;               //current x position
  cy = cur.pos.y;               //current y position
  int pass = 0;                 //highest wall, used by followers
  int lpass = 3;                //lowest wall, used by followers
  //Set pass and lpass
  for(int i = 0; i < 4; i++){
    if(map[cx][cy].walls[i] > pass){
      pass = map[cx][cy].walls[i];
    }
    if(map[cx][cy].walls[i] < lpass){
      lpass = map[cx][cy].walls[i];
    }
  }
  //Check if you're a leader. If yes, see if you're on someone's trail. 
  if(leaders[avID] == 0){
    int onTrail = passed(map[cx][cy], avID, total, leaders);
    if(-1 == onTrail){
      int leads = 0;
      for(int i = 0; i < total; i++){
        if(!leaders[i]){
          leads++;
        }
      }
      //Check if this is the last leader left
      if(1 == leads){
        /*See if you're in a dead end. If yes, leave it- not doing so will
         * lead to an edge case where the leader stays and the followers don't*/
        if(1 == lpass){
          int walls = 0;
          int open = 0;
          for(int i = 0; i < 4; i++){
            if(map[cx][cy].walls[i] == 1){
              walls++;
            } else {
              open = i;
            }
          }
          if(3 == walls){
            return (open + 1);
          }
        }
        //Otherwise, sleep
        return 8;
      }
      /*If you aren't the last leader, look for unexplored walls. If there are
       * none, take the leftmost open path. If there are unexplored walls,
       * explore them in this order: North, then East, then South, then
       * West. */
      if(1 == lpass){
        switch(last){
          case 1:
            if(map[cx][cy].walls[3] > 1){
              return 4;
            } else if(map[cx][cy].walls[0] > 1){
              return 1;
            } else if(map[cx][cy].walls[1] > 1){
              return 2;
            } else if(map[cx][cy].walls[2] > 1){
              return 3;
            }
          case 2:
            if(map[cx][cy].walls[0] > 1){
              return 1;
            } else if(map[cx][cy].walls[1] > 1){
              return 2;
            } else if(map[cx][cy].walls[2] > 1){
              return 3;
            } else if(map[cx][cy].walls[3] > 1){
              return 4;
            }
          case 3:
            if(map[cx][cy].walls[1] > 1){
              return 2;
            } else if(map[cx][cy].walls[2] > 1){
              return 3;
            } else if(map[cx][cy].walls[3] > 1){
              return 4;
            } else if(map[cx][cy].walls[0] > 1){
              return 1;
            }
          case 4:
            if(map[cx][cy].walls[2] > 1){
              return 3;
            } else if(map[cx][cy].walls[3] > 1){
              return 4;
            } else if(map[cx][cy].walls[0] > 1){
              return 1;
            } else if(map[cx][cy].walls[1] > 1){
              return 2;
            }
        }
      } else {
        for(int i = 0; i < 4; i++){
          if(0 == map[cx][cy].walls[i]){
            return i+1;
          }
        }
      }
    /* If you have become a follower, extend your new leaders trail to cover
     * yours and then follow the leader. If you can't, wait.*/
    } else {
      leaders[avID] = 1;
      reassign(x, y, map, avID, onTrail);
      if(3 == pass){
        for(int i = 0; i < 4; i++){
          if(3 == map[cx][cy].walls[i]){
            return i+1;
          }
        }
      } else {
        return 8;
      }
    }
  /* If you're a follower, follow the leader. If you can't, wait. */
  } else {
    if(3 == pass){
      for(int i = 0; i < 4; i++){
        if(3 == map[cx][cy].walls[i]){
          return i+1;
        }
      }
    } else {
      return 8;
    }
  }
  //This should never get called, but just in case.
  return 8;
}

/* Checks if a leading avatar has passed through the cell it is given */
static int passed(cell_t cell, int avID, int total, int leaders[total]){
  for(int i = 0; i < total; i++){
    if(cell.avs[i] && !leaders[i] && (i != avID)){
      return i;
    }
  }
  return -1;
}

/*Adds the leader to the trail its follower took. This ensures that 
 * there is never a situation where A follows B, and C finds the start
 * of A's trail, but A is a follower, so C ignores it. */
static void reassign(int x, int y, cell_t map[x][y], int avID, int tID){
  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      if((map[i][j].avs[avID] > 0) && (map[i][j].avs[tID] == 0))
        map[i][j].avs[tID] = 1;
    }
  }
}

/*Checks if the avatar has moved, then adds walls as appropriate. */
int move(int x, int y, cell_t map[x][y], Avatar last, Avatar cur, int direction){

  int curX= cur.pos.x;
  int curY= cur.pos.y;
  int lastX= last.pos.x;
  int lastY= last.pos.y;
  int id = cur.fd;

  //check if after didn't move
  if((lastX == curX) && (lastY == curY)){
    
    //wall found

    //check north move attempt
    if(direction==1){

      //set walls
      map[curX][curY].walls[0]=1;
      map[curX][curY-1].walls[2]=1;
      
    }
    //check east move
    else if(direction==2){

      //set walls
      map[curX][curY].walls[1]=1;
      map[curX+1][curY].walls[3]=1;
    }

    //check south move
    else if(direction==3){

      //set walls
      map[curX][curY].walls[2]=1;
      map[curX][curY+1].walls[0]=1;
      
    }
    //check west move
    else if(direction==4){

      //set walls
      map[curX][curY].walls[3]=1;
      map[curX-1][curY].walls[1]=1;
      
    }
    return 0;
  }

  //avatar moved
  else{

    //check north move 
    if(direction==1){

      //set walls
      map[curX][curY].walls[2]=seal(x, y, map, lastX, lastY);
      map[lastX][lastY].walls[0]=3;

      //update the avs struct
      map[lastX][lastY].avs[id]++;
      return 1;
      
    }
    //check east move
    else if(direction==2){

      //set walls
      map[curX][curY].walls[3]=seal(x, y, map, lastX, lastY);
      map[lastX][lastY].walls[1]=3;

      //update the avs struct
      map[lastX][lastY].avs[id]++;
      return 2;
    }

    //check south move
    else if(direction==3){

      //set walls
      map[curX][curY].walls[0]=seal(x, y, map, lastX, lastY);
      map[lastX][lastY].walls[2]=3;

      //update the avs struct
      map[lastX][lastY].avs[id]++;
      return 3;
      
    }
    //check west move
    else if(direction==4){

      //set walls
      map[curX][curY].walls[1]=seal(x, y, map, lastX, lastY);
      map[lastX][lastY].walls[3]=3;

      //update the avs struct
      map[lastX][lastY].avs[id]++;
      return 4;
      
    }
  }
  return 0;
}

/*Returns 2 if the cell isn't a dead end, 1 if it is. Also used globally in the
 * GUI*/
int seal(int x, int y, cell_t map[x][y], int lx, int ly){
  int nWalls = 0;
  for(int i = 0; i < 4; i++){
    if(map[lx][ly].walls[i] == 1){
      nWalls++;
    }
  }
  if(nWalls == 3){
    return 1;
  }
  return 2;
}