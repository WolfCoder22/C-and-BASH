/*
 * Functions.h - header file for maze functionality
 *    
 *
 *
 */

#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

/*********** global types ***************/
typedef struct cell{
  int walls[4];
  int *avs;
} cell_t;

/*********** functions ***************/

void maze_setup(int x, int y, int avs, cell_t in[x][y]);

void maze_delete(int x, int y, cell_t in[x][y]);

int alg(AM_Message *initMessage, int x, int y, cell_t map[x][y], 
    Avatar cur, int total, int leaders[total], int last);

int logic(int x, int y, cell_t map[x][y], int total, int leaders[total], Avatar cur, int last);

int move(int x, int y, cell_t map[x][y], Avatar last, Avatar cur, int direction);

int seal(int x, int y, cell_t map[x][y], int lx, int ly);

#endif // __FUNCTIONS_H