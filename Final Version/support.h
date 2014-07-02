/*
 * support header for the game.c
 *
 * May 9th
 *
 * Jason
 */

#ifndef __SUPPORT_H_
#define __SUPPORT_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "linked_list.h"
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h" //rotate and zoom
#include "SDL/SDL_ttf.h" //Truetype fonts
#include "SDL/SDL_framerate.h" // Framerate manager

#define TRUE 1
#define FALSE 0

#define BSTART 0
#define START 1
#define TYPENAME 2
#define SHOWTOP 3

// Screen width/height
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BLOCK_ROW 24
#define BLOCK_COL 20
#define BLOCK_SIZE 20
// Bits per pixel
#define SCREEN_BPP 32

extern TTF_Font *font;
extern SDL_Color textColorwhite;
extern SDL_Surface *screen;
extern linked_list_t *list;
extern linked_list_t *top_list;
extern SDL_Surface *button_save;
extern SDL_Surface *labelReset;
extern SDL_Surface *labelStart;
extern SDL_Surface *labelResetInf;
extern SDL_Surface *labelStartInf;
extern linked_list_t *row0;
extern SDL_Surface *button;
extern SDL_Surface *top;
extern int score;
extern SDL_Surface *scores;
extern SDL_Surface *restart;
extern char *str_score;
extern SDL_Surface *your_score;
extern SDL_Surface *congrat;
extern int num_block;
extern time_t last_time_add;
extern time_t start_time;
extern int game_start;
extern SDL_Surface *lose_game;
extern SDL_Surface *name;
extern char *buffer;
extern int name_index;
extern SDL_Surface *label_save;
extern SDL_Surface *top0;
extern SDL_Surface *top1;
extern SDL_Surface *top2;
extern SDL_Surface *top3;
extern SDL_Surface *top4;

/*
 * Repaint the window
 */
void repaint();

/*
 * Clean the memory allocated
 */
void clean_mem();

/*
 * Compare 2 user struct's score.
 *
 * Parameters:
 *  p1 : a user struct.
 *  p2 : a user struct.
 *
 *  Returns:
 *   1: if p1's score is higher
 *  -1: if p2's score is higher
 *   0: their scores are same
 */
int cmp(const void * p1, const void *p2);

/*
 * Apply one surface (src) onto another (dest) at the 
 * position specified.
 *
 * Parameters:
 *  x : X position to place the surface
 *  y : Y position to place the surface
 *  src : The source surface (what to apply)
 *  dest : The destination surface (where to apply it)
 *
 *  Returns:
 *   nothing
 */
void apply_surface( int x, int y, SDL_Surface *scr, SDL_Surface *dest);

/*
 * Load an image onto a surface
 *
 * Parameters:
 *  filename: The filename of the image
 *  angle: Angle of rotation (0 = no rotation)
 *  zoom: Zoom factor (1 = no zoom)
 *
 * Returns:
 *  Pointer to a SDL_Surface containing the image, or NULL, or error.
 */
SDL_Surface *load_image( char *filename, double angle, double zoom );


/*
 * Get the str with name and score from the top_list at position i;
 *
 * Parameters:
 *  i: index for the top_list
 *
 * Returns:
 *   str with name and score from the top_list at position.
 */
char *get_str(int index);


/*
 * Creat the 5 labels with the top 5 user's name and score, and
 * then put them on the screen.
 *
 */ 
void print_top5();

/*
 * Creat the 23 list, and combain them together with row0.
 *
 */
void creat_list_of_list();

/*
 * fill the every list of blocks with color integer array,
 * and the integer array about x and integer array about y. 
 *
 */
void fill_list_of_list();

/*
 * To check the score whether can be put on the TOP5
 *
 * Parameters:
 *  score: the integer score to check
 *
 *  Returns:
 *   0: FALSE
 *   1: TRUE
 */
int check_top5(int score);

/*
 * create a list and add to the row0 list
 */
void add_list();

/*
 * Load an image onto a surface
 *
 * Parameters:
 *  filename: The filename of the image
 *  angle: Angle of rotation (0 = no rotation)
 *  zoom: Zoom factor (1 = no zoom)
 *
 * Returns:
 *  Pointer to a SDL_Surface containing the image, or NULL, or error.
 */
SDL_Surface * load_image( char *filename,double angle, double zoom );

/*
 * initial the SDL and FRONT and framerate and screen.
 *
 *
 * Returns:
 *  -1 : error about init SDL
 *  -4 : error about EnableUNICODE
 *  -3 : error about TTF
 *  -2 : error about the screen
 *  -5 : can't open TTF
 *   0 : successful.
 *
 */
int initial_screen();

/*
 * reset all blocks list. Top 6 rows with black colors, other with random color
 *
 */
void reset_block();

/*
 * check which block's been clicked, then change the block's 
 * color corespondingly.
 *
 * Parameter:
 * x : collom index
 * y : row index
 *
 */
int check_block( int x, int y );

/*
 * set up label and pictures for the screen;
 *
 * Return:
 * -1: allocate error
 *  0: successful.
 */
int set_screen();

/*
 * recursion method to check block color, if they are same, then
 * change it to black, and also send the blocks around this
 * block to this method.
 *
 * Parameter:
 * row: block row index
 * col: block collom index
 * color: the color we want to check
 * 
 * Return:
 * Nothing
 */
void set_black(int row, int col, int *color);

/*
 * get the block by the row index and collom index.
 *
 * Parameter:
 * row: block row index
 * col: block collom index
 *
 * Return:
 * the block pointer.
 */
void* get_block(int row, int col);

/*
 * check the block is black or not.
 *
 * Parameter:
 * row: block row index
 * col: block collom index
 *
 * Return:
 *  0: is not black
 *  1: is black
 */
int is_black(int row, int col);

/*
 * check the block ,and make them fall to the right spot.
 */
void block_falling();

/*
 * add a new row to the screen.
 *
 * Return:
 *  0: not add successfully
 *  1: add successfully
 *
 */
int add_new_row();


/*
 * read the record.bin file
 *
 * Parameter:
 * fd: the file of record.bin
 */
void read_file(FILE *fd);

/*
 * write to the record.bin file, with TOP5 user's name and score
 *
 * Parameter:
 * fd: the file of record.bin
 * i: rank of user from top5 list
 */
void write_file(FILE *fd, int i);

/*
 * catch the key information from the keyboard to modify the buffer for
 * user type their name when they are on top5.
 *
 * Parameter:
 * key: keyboard event.
 */
void print_key_info(SDL_KeyboardEvent *key);
#endif /* __SUPPORT_H_ */
