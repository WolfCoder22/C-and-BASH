/*
 * GUI.h - header file for maze functionality
 *    
 *
 *
 */

#ifndef __GUI_H
#define __GUIGUI_H

#include <cairo.h>
#include <gtk/gtk.h>

/*********** global types ***************/

/****** Global Vars ******/
int mazeH;
int mazeW;
int TotalAvs;
char *mazeLevel;


/*********** functions ***************/
void drawMaze(int x, int y, cell_t in[x][y], int avNum, XYPos avPos[10]);
char *cheackAVPos(int xCor, int yCor, int avNum, XYPos avPos[10]);
void passData(int x, int y, int avs, char *level);
void drawMazeGTK(cairo_t *cr, int x, int y, cell_t in[x][y], int avNum, XYPos avPos[10], uint32_t turns);
void initialize_window(GtkWidget* window);
void initGTKMaze(cairo_t *cr, int x, int y, cell_t map[x][y]);
void printSuccess(cairo_t *cr, int turns);

#endif // __GUI_H