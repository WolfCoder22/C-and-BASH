/* File: GUI.c
 *
 * Authors: Michael Ortiz, Galen Brown, and Alexander Wolf
 *
 * Description: GUI has helper functions to draw the maze 
 *  while new data is found. It can print it in ASC format 
 *  with drawmaze and it use the GTK library to have an
 *  aesthetic UI with pretty colors and shapes
 *
 * Summer 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include "amazing.h"
#include "Functions.h"
#include "GUI.h"

#include <time.h>
#include <string.h>


/***  Local Functions    ***/
double rgb(int i);
double randomRGB();
void drawWall(cairo_t *cr, double x1, double y1, double x2, double y2, double isBarrier);
void drawAvatar(cairo_t *cr, double xa1, double ya1, double length, char *av);
double *getRGBforAVid(int w);

/***   gtk local vars   ***/
int guiHEIGHT=750;
int guiWIDTH=1000;

int mazeStartx;
int mazeStarty=80;

int mazeEndx;
int mazeEndy=730;

double cellLength;
double strokeSize;

//pass needed GUI data from AMStartup
void passData(int x, int y, int avs, char *level){
  mazeW=x;
  mazeH=y;
  TotalAvs=avs;

  //is square maze of varying difficulties
  if(x==y){
    mazeStartx=175;
    cellLength= 650/(double) x;
  }
  //is difficulty 1 with height 9
  else if(x==6){

    cellLength= 650/(double) y;
    mazeStartx= 500-(3*cellLength);
  }

  //is difficulty 2 with hieght 10
  else{
    cellLength= 650/(double) y;
    mazeStartx= 500-(6*cellLength);
  }

  mazeEndx=guiWIDTH-mazeStartx;
  strokeSize=cellLength/7;
  cellLength=cellLength;

  mazeLevel=level;

}

//draw the maze with the current data 
void drawMaze(int x, int y, cell_t in[x][y], int avNum, XYPos avPos[10]){

  for(int a = 0; a < x; a++){
    for(int b = 0; b < y; b++){
      if(1 == seal(x, y, in, a, b)){
        for(int i = 0; i < 4; i++){
          if(1 != in[a][b].walls[i]){
            in[a][b].walls[i] = 3;
            switch(i){
              case 0:
                in[a][b-1].walls[2] = 1;
                break;
              case 1:
                in[a+1][b].walls[3] = 1;
                break;
              case 2:
                in[a][b+1].walls[0] = 1;
                break;
              case 3:
                in[a-1][b].walls[1] = 1;
                break;
            }
          }
        }
      }
    }
  }

  system("clear");
  printf("\n\n      ");

  //draw north wall
  for(int n=0; n<x; n++){  
    printf("____ ");    
  }

  //loop through every row in the maze
  for(int r=0; r<y; r++){
    if(r==0){
      printf("\n     ");
    }

    //loop through collums
    for(int w=0; w<x; w++){

      //get char for avatar display
      char *avStr=cheackAVPos(w, r, avNum, avPos);
      //printf("avstring check = %s\n", avStr);

      //check if west outer wall
      if(w==0){
        printf("| %s  ", avStr);
      }

      else{

        //check if there is a west wall draw first part
        if(in[w-1][r].walls[1]==1 && in[w][r].walls[3]==1){
          printf("| %s  ", avStr);
        }

        else{
          printf(" %s   ", avStr);
        }
       }

       //free avStr if not * or space
       if(strcmp(avStr, "*") && strcmp(avStr, " ")){
        free(avStr);
       }
    }

    //draw first part of east wall and new line
    printf("|\n     ");

    //loop through collums
    for(int e=0; e<x; e++){

      //print west wall
      if(e==0){

        printf("|");

        //draw south wall if there is one present
        if(in[e][r].walls[2]==1 && in[e][r+1].walls[0]==1){

              printf("____");
        }

        else{
          printf("    ");
        }
      }

      //check if west wall exists
      else if(in[e][r].walls[3]==1 && in[e-1][r].walls[1]==1) {

        //draw second half of west wall
        printf("|");

        //check if on last row
        if(r==y-1){
          printf("____");
        }

        //draw south wall if there is one present
        else if(in[e][r].walls[2]==1 && in[e][r+1].walls[0]==1){

              printf("____");
            
        }

        else{
          printf("    ");
        }
      }

      else{

        //check if on last row
        if(r==y-1){
          printf(" ____");
        }

        //draw south wall if there is one present
        else if(in[e][r].walls[2]==1 && in[e][r+1].walls[0]==1){

              printf(" ____");
            
        }
        else{
          printf("     ");
        }
      }
    }

    //draw second part of east wall and new line
      printf("|\n     ");
    }
}

//draw maze backgound, outer walls, and information
void initGTKMaze(cairo_t *cr, int x, int y, cell_t map[x][y]){

  srand(time(NULL));

  //set background color
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_paint(cr);

  cairo_set_source_rgb(cr, rgb(93), rgb(82), rgb(248));
  cairo_select_font_face(cr, "Mono", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);

  //draw title
  cairo_set_font_size(cr, 27.0);
  cairo_move_to(cr, 400, 30);
  cairo_show_text(cr, "MaZe GRoUp #1");

  cairo_set_font_size(cr, 13.0);

  cairo_set_source_rgb(cr, rgb(240), rgb(100), rgb(230));
  cairo_move_to(cr, 410, 47);
  cairo_show_text(cr,"Difficulty:");

  cairo_set_source_rgb(cr, rgb(230), rgb(135), rgb(40));
  cairo_move_to(cr, 503, 47);
  cairo_show_text(cr, mazeLevel);

  cairo_set_source_rgb(cr, rgb(44), rgb(238), rgb(41));
  cairo_move_to(cr, 530, 47);
  cairo_show_text(cr,"Avatars:");

  char avsNum[2];
  sprintf(avsNum, "%d", TotalAvs);
  cairo_set_source_rgb(cr, rgb(245), rgb(220), rgb(90));
  cairo_move_to(cr, 595, 47);
  cairo_show_text(cr, avsNum);

  cairo_set_source_rgb(cr, rgb(84), rgb(198), rgb(76));
  cairo_move_to(cr, 420, 64);
  cairo_show_text(cr,"Width:");

  char width[2];
  sprintf(width, "%d", mazeW);
  cairo_set_source_rgb(cr, rgb(244), rgb(250), rgb(142));
  cairo_move_to(cr, 475, 64);
  cairo_show_text(cr, width);

  cairo_set_source_rgb(cr, rgb(250), rgb(137), rgb(242));
  cairo_move_to(cr, 520, 64);
  cairo_show_text(cr,"Height:");

  char height[2];
  sprintf(height, "%d", mazeH);
  cairo_set_source_rgb(cr, rgb(40), rgb(160), rgb(240));
  cairo_move_to(cr, 580, 64);
  cairo_show_text(cr, height);

  //draw coder names
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, 760, 15);
  cairo_set_font_size(cr, 10.0);
  cairo_show_text(cr,"Alex Wolf, Galen Brown, Michael Ortiz");

  cairo_set_source_rgb(cr, rgb(143), rgb(249), rgb(224));
  cairo_set_line_width(cr,strokeSize);
  
  //draw maze border
  cairo_rectangle(cr, mazeStartx, mazeStarty, mazeEndx-mazeStartx, mazeEndy-mazeStarty);
  cairo_stroke_preserve(cr);

}

//print success function
void printSuccess(cairo_t *cr, int turnsInt32){

  //convert turns to string
  char turns[11];
  snprintf(turns, sizeof turns, "%lu", (unsigned long) turnsInt32);

  cairo_set_source_rgb(cr, rgb(219), rgb(202), rgb(45));
  cairo_move_to(cr, 50, 250);
  cairo_set_font_size(cr, 200.0);
  cairo_show_text(cr,"SUCCESS!");

  cairo_set_source_rgb(cr, rgb(164), rgb(45), rgb(219));
  cairo_move_to(cr, 150, 350);
  cairo_set_font_size(cr, 80.0);
  cairo_show_text(cr,"Solved level");


  cairo_move_to(cr, 750, 350);
  cairo_show_text(cr,mazeLevel);

  cairo_set_source_rgb(cr, rgb(74), rgb(219), rgb(45));
  cairo_move_to(cr, 220, 450);
  cairo_show_text(cr, "in");

  cairo_set_source_rgb(cr, rgb(249), rgb(252), rgb(211));
  cairo_move_to(cr, 355, 455);
  cairo_show_text(cr,turns);

  cairo_set_source_rgb(cr, rgb(74), rgb(219), rgb(45));
  cairo_move_to(cr, 550, 450);
  cairo_show_text(cr, "turns");

}

//function to draw the current maze
void drawMazeGTK(cairo_t *cr, int x, int y, cell_t in[x][y], int avNum, XYPos avPos[10], uint32_t turns){

  //go through maze and take note of dead ends
  for(int a = 0; a < x; a++){
    for(int b = 0; b < y; b++){
      if(1 == seal(x, y, in, a, b)){
        for(int i = 0; i < 4; i++){
          if(1 != in[a][b].walls[i]){
            in[a][b].walls[i] = 3;
            switch(i){
              case 0:
                in[a][b-1].walls[2] = 1;
                break;
              case 1:
                in[a+1][b].walls[3] = 1;
                break;
              case 2:
                in[a][b+1].walls[0] = 1;
                break;
              case 3:
                in[a-1][b].walls[1] = 1;
                break;
            }
          }
        }
      }
    }
  }

  //loop through every row in the maze
  for(int r=0; r<y; r++){

    //loop through collums
    for(int w=0; w<x; w++){

      double avCellLength= 7*(cellLength-2*strokeSize)/10;

      //get cordiantes for drawing walls
      double xw1= mazeStartx+(w*cellLength);
      double yw1= mazeStarty+(r*cellLength);

      double xw2= xw1;
      double yw2= yw1+cellLength;

      double xs1= xw2;
      double ys1= yw2;

      double xs2= xs1+cellLength;
      double ys2= ys1;

      //get cordiantes for drawing avatar
      double xa1= xw1+7*strokeSize/4;
      double ya1= yw1+7*strokeSize/4;

      //check if there is a west wall; draw it if there is
      if(in[w-1][r].walls[1]==1 && in[w][r].walls[3]==1){
        drawWall(cr, xw1, yw1, xw2, yw2, 0);

      }
      //check if it is a west deadend block off and draw if there is
      else if((in[w-1][r].walls[1]==1 && in[w][r].walls[3]==3) ||
        (in[w-1][r].walls[1]==3 && in[w][r].walls[3]==1)){
          drawWall(cr, xw1, yw1, xw2, yw2, 1);
        
      }

      //check if there is a south wall draw it if there is
      if(in[w][r].walls[2]==1 && in[w][r+1].walls[0]==1
        && r!=y-1){
        drawWall(cr, xs1, ys1, xs2, ys2, 0);
      }

      //check if it is a south deadend block off and draw if there is
      else if(((in[w][r].walls[2]==1 && in[w][r+1].walls[0]==3) ||
        (in[w][r].walls[2]==3 && in[w][r+1].walls[0]==1))
        && r!=y-1){
          drawWall(cr, xs1, ys1, xs2, ys2, 1);
      }
     
      //draw avatar for display
      char *avStr=cheackAVPos(w, r, avNum, avPos);
      drawAvatar(cr, xa1, ya1, avCellLength, avStr);

      //free avStr if not * or space
      if(strcmp(avStr, "*") && strcmp(avStr, " ")){
        free(avStr);
      }
    }
  }

  //draw maze border
  cairo_set_source_rgb(cr, rgb(143), rgb(249), rgb(224));
  cairo_set_line_width(cr,strokeSize);
  cairo_rectangle(cr, mazeStartx, mazeStarty, mazeEndx-mazeStartx, mazeEndy-mazeStarty);
  cairo_stroke_preserve(cr);

  //check if is printing solution
  if(turns!=0){
    printSuccess(cr, turns);
  }
}

//draw a wall based off the given coordinates
void drawWall(cairo_t *cr, double x1, double y1, double x2, double y2, double isBarrier){

  double r;
  double g;
  double b;

  if(!isBarrier){
    r= rgb(143);
    g= rgb(249);
    b= rgb(224);
  }
  else{
    r= rgb(235);
    g= rgb(132);
    b= rgb(132);
  }

  cairo_set_source_rgb(cr, r, g, b);
  cairo_set_line_width(cr,strokeSize);
  
  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
  cairo_stroke(cr); 
}

//draw an avatar in specific cell
void drawAvatar(cairo_t *cr, double xa1, double ya1, double length, char *av){

  double r;
  double g;
  double b;


  //no avatar on cell return
  if(!strcmp(av, " ")){
    r=0;
    g=0;
    b=0;

    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, xa1, ya1, length, length);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
  }

  //two or more avatars on same cell
  //make unique shape
  else if(!strcmp(av, "*")){

    r=randomRGB();
    g=randomRGB();
    b=randomRGB();

    //get values for drawing shape
    double x1= xa1;
    double y1= ya1+(length/3);

    double x2= xa1+(length/3);
    double y2= ya1;

    double l1w= length;
    double l1h= length/3;

    double l2h= length;
    double l2w= length/3;

    //draw shape
    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, x1, y1, l1w, l1h);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, x2, y2, l2w, l2h);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
  }

  //only one avatar was found on cell
  //print it with special individual color
  else{

    int w = atoi(av);
    double *rgbs=getRGBforAVid(w);

    r=rgbs[0];
    g=rgbs[1];
    b=rgbs[2];

    cairo_set_source_rgb(cr, r, g, b);
    cairo_rectangle(cr, xa1, ya1, length, length);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);

    free(rgbs);
  }
}

//get rgb values for a specific avatar
double *getRGBforAVid(int w){

  double *rbgs= malloc(sizeof(double)*3);

  if(w==0){
    rbgs[0]= rgb(255);
    rbgs[1]= rgb(255);
    rbgs[2]= rgb(255);
  }
  else if(w==1){
    rbgs[0]= rgb(234);
    rbgs[1]= rgb(171);
    rbgs[2]= rgb(76);
  }
  else if(w==2){
    rbgs[0]= rgb(76);
    rbgs[1]= rgb(234);
    rbgs[2]= rgb(92);
  }
  else if(w==3){
    rbgs[0]= rgb(131);
    rbgs[1]= rgb(76);
    rbgs[2]= rgb(234);
  }
  else if(w==4){
    rbgs[0]= rgb(240);
    rbgs[1]= rgb(100);
    rbgs[2]= rgb(230);
  }
  else if(w==5){
    rbgs[0]= rgb(93);
    rbgs[1]= rgb(82);
    rbgs[2]= rgb(248);
  }
  else if(w==6){
    rbgs[0]= rgb(85);
    rbgs[1]= rgb(233);
    rbgs[2]= rgb(236);
  }
  else if(w==7){
    rbgs[0]= rgb(124);
    rbgs[1]= rgb(118);
    rbgs[2]= rgb(76);
  }
  else if(w==8){
    rbgs[0]= rgb(255);
    rbgs[1]= rgb(230);
    rbgs[2]= rgb(5);
  }
  else if(w==9){
    rbgs[0]= rgb(244);
    rbgs[1]= rgb(250);
    rbgs[2]= rgb(142);
  }

  return rbgs;

}

//init gtk window
void initialize_window(GtkWidget* window){
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_title (GTK_WINDOW (window), "CS 50 Final Project");
    gtk_window_set_default_size(GTK_WINDOW(window), guiWIDTH, guiHEIGHT);
    gtk_widget_show_all(window);
}

/*function to check if an avatar is currently on a cell
   -returns the avatar number (char *) 0-9
   -if two avats in same cell return *
**/
char *cheackAVPos(int xCor, int yCor, int avNum, XYPos avPos[10]){

  int avReturn=11;

  //loop through all av positions
  for(int i=0; i<avNum; i++){

    int x= avPos[i].x;
    int y= avPos[i].y;

    //check if avatar is o tile
    if(xCor==x && yCor==y){

      //check if avatar was already found on that position
      if(avReturn!=11){
       avReturn=10;
      }
      else{
       avReturn=i;
      }
    }
  }

  //return space if no avatra on cell
  if(avReturn==11){
    return " ";
  }
  //two avatars where found on cell
  else if(avReturn==10){
    return"*";
  }
  //return the char of the avatar number
  else{

    char *str= malloc(sizeof(char *));
    sprintf(str, "%d", avReturn);
    return str;
  }
}

//get rgb value as in 0-1 float
double rgb(int i){
  return i/(double)255;
}

//returns a random rdg number
double randomRGB(){
  double r = rand()/(double) RAND_MAX;

  if(r<.03){
    return .03;
  }
  else{
    return r;
  }
}
