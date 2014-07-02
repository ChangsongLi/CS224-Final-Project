/*
 * implement methods from the support.h
 *
 * May 9th
 *
 * Jason
 */

#include "game.h"

int cmp(const void * p1, const void *p2) {
    user_t *left = (user_t*)p1;
    user_t *right = (user_t*)p2;

    if( left->score < right->score ) {
        return 1;
    }
    else if( left->score > right->score ) {
        return -1;
    }

    return 0;
}

void clean_mem(){
    int i,j;

    if( str_score != NULL ){
	free(str_score);
    }
    if(scores != NULL){
	SDL_FreeSurface(scores);
    }
    if(labelReset != NULL){
	SDL_FreeSurface(labelReset);
    }
    if(your_score != NULL){
	SDL_FreeSurface(your_score);
    }
    if(button != NULL){
	SDL_FreeSurface(button);
    }
    if(restart != NULL){
	SDL_FreeSurface(restart);
    }
    if(labelStart != NULL){
	SDL_FreeSurface(labelStart);
    }
    if(labelResetInf != NULL){
	SDL_FreeSurface(labelResetInf);
    }
    if(labelStartInf != NULL){
	SDL_FreeSurface(labelStartInf);
    }
    if(screen != NULL){
	SDL_FreeSurface(screen);
    }
    if(congrat != NULL){
	SDL_FreeSurface(congrat);
    }
    if(top != NULL){
	SDL_FreeSurface(top);
    }
    if(top0 != NULL){
	SDL_FreeSurface(top0);
    }
    if(top1 != NULL){
	SDL_FreeSurface(top1);
    }
    if(top2 != NULL){
	SDL_FreeSurface(top2);
    }
    if(top3 != NULL){
	SDL_FreeSurface(top3);
    }
    if(top4 != NULL){
	SDL_FreeSurface(top4);
    }
    if(label_save != NULL){
	SDL_FreeSurface(label_save);
    }
    if(button_save != NULL){
	SDL_FreeSurface(button_save);
    }
    if(name != NULL){
	SDL_FreeSurface(name);
    }

    if(buffer != NULL){
	free(buffer);
    }

    linked_list_t *tracer = row0;
    for(i = BLOCK_ROW; i > 0; i--){
	for(j = 0; j < i - 1; j++){
	    tracer = tracer->next;
	}
	user_t *u = linked_list_remove(0, tracer);
	while(u != NULL){
	    free(u);
	    u = linked_list_remove(0, tracer);
	}

	linked_list_destroy(tracer);
        tracer = row0;
    }

    tracer = top_list;
    user_t *u = linked_list_remove(0, tracer);
    while(u != NULL){
	free(u);
	u = linked_list_remove(0, tracer);
    }
    

    //SDL_FreeSurface();
    TTF_CloseFont( font );
    // Quit SDL
    TTF_Quit();
    
    SDL_Quit();

}

void write_file(FILE *fd, int i){
    int len,score;
    char *name;
    int j;
    user_t *u = linked_list_get(i,top_list);
    name = u->name;
    score = u->score;

    len = strlen(name);
    len++;
    fwrite(&len,sizeof(int),1,fd);
    
    for(j = 0; j < len; j++){
	fwrite(&(name[j]),sizeof(char),1,fd);
    }

    fwrite(&score,sizeof(int),1,fd);
    
}

void read_file(FILE *fd){
    int len,score;
    char *name;
    int i;
    user_t *u = (user_t*)malloc(sizeof(user_t));

    fread(&len,sizeof(int),1,fd);

    u->name = (char*)malloc(sizeof(char)*(len));
    name = u->name;
    for(i = 0; i < len; i++){
	fread(&(name[i]),sizeof(char),1,fd);
    }
  

    fread(&score,sizeof(int),1,fd);

    u->score = score;
    
    linked_list_append(u, top_list);
}

void* get_block(int row, int col){
    int i;
    block *b;
    linked_list_t *list = row0;

    //loop for the right list
    for(i = 0; i < row; i++){
	list = list->next;
    }

    //use the right list and col index to get the block
    b = linked_list_get(col, list);
    return b;
}

void print_key_info(SDL_KeyboardEvent *key){

    //handle the Backspace key delete 
    //the last char from the buffer string
    if(key->keysym.sym == SDLK_BACKSPACE){
	if(name_index != 0){
	    buffer[name_index-1] = '\0';
	    name_index--;
	}
    }

    //handle other printable key to add to the buffer str
    else if(key->keysym.unicode < 0x80 
       && key->keysym.unicode >0
       && name_index < 9){
	buffer[name_index] = (char)key->keysym.unicode;
	buffer[name_index+1] = '\0';
	name_index++;
    }

}

void set_black(int row, int col, int *color){
    if(row < 0|| row > BLOCK_ROW - 1 || col < 0 || col > BLOCK_COL - 1)
	return;

    // change the colorl if the color is same
    block *b = get_block(row,col);
    if((color[0] == (b->color)[0]) && 
       (color[1] == (b->color)[1]) &&
       (color[2] == (b->color)[2])){
	(b->color)[0] = 0;
	(b->color)[1] = 0;
	(b->color)[2] = 0;
	num_block++;

	// keeping check blocks around this block.
	set_black(row + 1, col,color);
	set_black(row -1, col,color);
	set_black(row , col + 1,color);
	set_black(row , col - 1,color);
    }
}

void repaint(){
    int i,j;

    //repaint screen
    SDL_FillRect(screen, NULL, 
		 SDL_MapRGB(screen->format, 0, 0, 0));

    //repaint picture and label 
    apply_surface( 440, 340, labelReset, screen );
    apply_surface( 440, 370, labelResetInf, screen );
    apply_surface( 440, 400, labelStart, screen );
    apply_surface( 440, 430, labelStartInf, screen );
    apply_surface( 440, 100, button, screen);
    apply_surface( 440, 120, restart, screen );
    apply_surface( 520, 50, scores, screen );
    apply_surface( 440, 30, your_score, screen );

    //if game is over print the losing label
    if(game_start == TYPENAME){
	apply_surface( 100, 150, lose_game, screen );
	int ret = check_top5(score);
	if( ret == TRUE){
	    apply_surface( 100, 200, congrat,screen);
	    apply_surface( 250, 300, button_save,screen);
	    apply_surface( 260, 300, label_save,screen);
	    if(buffer != NULL)
		apply_surface( 100, 300, name, screen );
	}
	else{
	    game_start = SHOWTOP;
	}
    }
    else if(game_start == SHOWTOP){
	print_top5();
    }
    else{
	linked_list_t *tail = row0;
	block *data = NULL; 
	for(i  = 0; i < BLOCK_ROW; i++){
	    for(j = 0; j < BLOCK_COL; j++){
		data = linked_list_get(j,tail);
		short *x = data->x;
		short *y = data->y;
		filledPolygonRGBA(screen,
				  x , y,
				  4, // length of the array
				  (data->color)[0], 
				  (data->color)[1], 
				  (data->color)[2], 255);
	    }
	    tail = tail->next;
	}
    }
    
    // update screen
    SDL_Flip( screen );

}

void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dest){
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, NULL, dest, &offset);
}

void creat_list_of_list(){
    int i;
    row0 = linked_list_create();
    row0->next = NULL;
    for(i = 0; i < BLOCK_ROW-1; i++){
	add_list();
    }
}

void fill_list_of_list(){
    int i,j;
    linked_list_t *tracer = row0;
    for(i = 0; i < BLOCK_ROW; i++){
	for(j = 0; j < BLOCK_COL; j++){

	    // creat a block with properiate location short arr
	    block *blk = (block*)malloc(sizeof(block));
	    short *x = (short*)malloc(sizeof(short)*4);
	    short *y = (short*)malloc(sizeof(short)*4);
	    x[0] = BLOCK_SIZE * j;
	    x[1] = x[0] + BLOCK_SIZE;
	    x[2] = x[1];
	    x[3] = x[0];
	    y[0] = BLOCK_SIZE * i;
	    y[1] = y[0];
	    y[2] = y[1] + BLOCK_SIZE;
	    y[3] = y[2];
	    blk->x = x;
	    blk->y = y;
	    
	    // fill the color arr with random color
	    int *color = (int*)malloc(sizeof(int)*3);
	    if(i < 6){
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
	    }
	    else{
		int ran = random()%4;
		if(ran == 0){
		    color[0] = 169;
		    color[1] = 65;
		    color[2] = 201;
		}
		else if(ran == 1){
		    color[0] = 99;
		    color[1] = 148;
		    color[2] = 220;
		}
		else if(ran == 2){
		    color[0] = 136;
		    color[1] = 232;
		    color[2] = 112;
		}
		else if(ran == 3){
		    color[0] = 255;
		    color[1] = 255;
		    color[2] = 127;
		}
	    }
	    blk->color = color;
	    
	    // append the block to the list
	    linked_list_append(blk, tracer);
	}
	tracer = tracer->next;
	
    }

}

void add_list(){
    // create a list and add to the row0 list
   linked_list_t *list = linked_list_create();
   list->next = NULL;
   linked_list_t *tracer = row0;
   while((tracer->next) != NULL){
       tracer = tracer->next;
   }
   tracer->next = list;
}

SDL_Surface *load_image( char *filename, double angle, double zoom ){
    SDL_Surface *optimized_image = NULL;
    SDL_Surface *loaded_image = NULL;
    SDL_Surface *scaled_image = NULL;

    loaded_image = IMG_Load( filename );
    if( NULL != loaded_image ){
	scaled_image = rotozoomSurface( loaded_image, angle,
					zoom, SMOOTHING_ON );
	optimized_image = SDL_DisplayFormat( scaled_image );

	SDL_FreeSurface( loaded_image );
	SDL_FreeSurface( scaled_image );
    }

    return optimized_image;
}

int initial_screen(){
    int ret;
    srandom(time(NULL));
    /*
     * Initialize the SDL
     */
    ret = SDL_Init( SDL_INIT_EVERYTHING );
    if( -1 == ret ){
	return -1;
    }

    ret = SDL_EnableUNICODE(1);
    if(-1 == ret){
	return -4;
    }
    score = 0;
    // Initialize SDL_ttf
    ret = TTF_Init();
    if( -1 == ret ){
	return -3;
    }

    
    // Setup the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH,
			       SCREEN_HEIGHT,
			       SCREEN_BPP,
			       SDL_SWSURFACE );
    

    if( NULL == screen ){
	return -2;
    }

    // Set title
    SDL_WM_SetCaption( "Eating candy game", NULL );
    
    // Open the font we want to use
    font = TTF_OpenFont("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf", 20);

    if( NULL == font ){
	fprintf(stderr, "Failed to open font\n");
	return -5;
    }
    return 0;
}

//TODO
void print_top5(){
    int length = top_list->length;
    SDL_Color textColorWhite = {255, 255, 255};
    if(length > 0){
	char *top0_str = get_str(0);
	if( top0 != NULL ){
	    SDL_FreeSurface(top0);
	}
	top0 = TTF_RenderText_Solid(font, 
				 top0_str,
				 textColorWhite);
	apply_surface( 150, 150, top0, screen );
    }
    if(length > 1){
	char *top1_str = get_str(1);
	if( top1 != NULL ){
	    SDL_FreeSurface(top1);
	}
	top1 = TTF_RenderText_Solid(font, 
				 top1_str,
				 textColorWhite);
	apply_surface( 150, 200, top1, screen );
    }
    if(length > 2){
	char *top2_str = get_str(2);
	if( top2 != NULL ){
	    SDL_FreeSurface(top2);
	}
	top2 = TTF_RenderText_Solid(font, 
				 top2_str,
				 textColorWhite);
	apply_surface( 150, 250, top2, screen );
    }
    if(length > 3){
	char *top3_str = get_str(3);
	if( top3 != NULL ){
	    SDL_FreeSurface(top3);
	}
	top3 = TTF_RenderText_Solid(font, 
				 top3_str,
				 textColorWhite);
	apply_surface( 150, 300, top3, screen );
    }
    if(length > 4){
	char *top4_str = get_str(4);
	if( top4 != NULL ){
	    SDL_FreeSurface(top4);
	}
	top4 = TTF_RenderText_Solid(font, 
				 top4_str,
				 textColorWhite);
	apply_surface( 150, 350, top4, screen );
    }

    if(top != NULL){
	SDL_FreeSurface(top);
    }
    top = TTF_RenderText_Solid(font, 
				 "TOP 5",
				 textColorWhite);
    apply_surface( 150, 80, top, screen );
}


char *get_str(int index){
    user_t *u = linked_list_get( index,top_list);
    char *arr = (char*)malloc(sizeof(char)*30);
    char *name = u->name;
    int score = u->score;

    sprintf(arr,"%d: %-9s %d",index+1,name,score);
    return arr;
}

int check_top5(int score){
    int size = top_list->length;
    if(size < 5){
	return TRUE;
    }
    else{
	user_t *u = linked_list_get( 4, top_list);
	if( (u->score) > score ){
	    return FALSE;
	}
	return TRUE;
    }
}

int set_screen(){
    SDL_Color textColorWhite = {255, 255, 255};
    str_score = (char*)malloc(sizeof(char)*10);
    top_list = linked_list_create();
    name_index = 0;
    if(str_score == NULL)
	return -1;

    buffer = (char*)malloc(sizeof(char)*10);
    if(str_score == NULL)
	return -1;

    buffer[0] = '\0';
    name =  TTF_RenderText_Solid(font, 
				 buffer,
				 textColorWhite);
    sprintf(str_score,"%d",score);
    scores =  TTF_RenderText_Solid(font, 
				 str_score,
				 textColorWhite);
    //load image
    button = load_image("p/cat.jpg",0,1);
    button_save = load_image("p/d.jpg",0,1);

    // creat label
    congrat = TTF_RenderText_Solid(font, "You are in top 5!Input your name!",textColorWhite);
    restart = TTF_RenderText_Solid(font, "Reset",textColorWhite);
    lose_game = TTF_RenderText_Solid(font, 
				 "YOU LOSE!!!",
				 textColorWhite);
    label_save = TTF_RenderText_Solid(font, 
				 "SAVE",
				 textColorWhite);
    labelReset = TTF_RenderText_Solid(font, 
				 "Reset:",
				 textColorWhite);
    labelResetInf = TTF_RenderText_Solid(font, 
				 "reset the game. ",
				 textColorWhite);
    labelStart = TTF_RenderText_Solid(font, 
				 "Start:",
				 textColorWhite);
    labelStartInf = TTF_RenderText_Solid(font, 
				 "Click blocks to start.",
				 textColorWhite);
    your_score = TTF_RenderText_Solid(font, 
				 "Score:",
				 textColorWhite);
    return 0;
}
int check_block( int x, int y ){
    if( x > 400){
	
    }
    else{
	//start calculate the time when game begins
	if(game_start == FALSE){
	   last_time_add = time(NULL);
	   start_time = time(NULL);
	}

	//get the block is clicked
	game_start = TRUE;
	block *b;
	int row = y / 20;
	int col = x / 20;
	int color[3];
	b = get_block(row,col);
	color[0] = (b->color)[0];
	color[1] = (b->color)[1];
	color[2] = (b->color)[2];
	if(is_black(row,col) == TRUE){

	}else{
	    set_black(row,col,color);
	    score = score + num_block * num_block;
	    num_block = 0;
	    block_falling();
	}
    }
    return 0;
}

int is_black(int row, int col){
    int color[3];
    block *b = get_block(row,col);
    color[0] = (b->color)[0];
    color[1] = (b->color)[1];
    color[2] = (b->color)[2];
    if(color[0] == 0 && color[1] == 0 && color[2] == 0){
	return TRUE;
    }

    return FALSE;
}

void block_falling(){
    int col,row,k;
    for( col = 0; col < BLOCK_COL; col++ ){
	for( row = BLOCK_ROW-1; row >= 0; row-- ){
	    if(is_black(row,col) == TRUE){
		for(k = row - 1; k >= 0; k--){
		    if(is_black(k,col) == FALSE){
			block *nb = get_block(k,col);
			block *b = get_block(row,col);
			(b->color)[0] = (nb->color)[0];
			(b->color)[1] = (nb->color)[1];
			(b->color)[2] = (nb->color)[2];
			(nb->color)[0] = 0;
			(nb->color)[1] = 0;
			(nb->color)[2] = 0;
			k = -1;
		    }
		    if(k == 0){
			row = -1;
		    }
		}
	    }
	}
    }
}

int add_new_row(){
    int col,row;
    for(col = 0; col < BLOCK_COL; col++){
	if(is_black(0,col) == FALSE ){
	    return FALSE;
	}
    }

    for(col = 0; col < BLOCK_COL; col++){
	for(row = BLOCK_ROW-1; row >= 0; row--){
	    if(is_black(row,col) == TRUE){
		block *b = get_block(row,col);
		            int ran = random()%4;
		if(ran == 0){
		    (b->color)[0] = 169;
		    (b->color)[1] = 65;
		    (b->color)[2] = 201;
		}
		else if(ran == 1){
		    (b->color)[0] = 99;
		    (b->color)[1] = 148;
		    (b->color)[2] = 220;
		}
		else if(ran == 2){
		    (b->color)[0] = 136;
		    (b->color)[1] = 232;
		    (b->color)[2] = 112;
		}
		else if(ran == 3){
		    (b->color)[0] = 255;
		    (b->color)[1] = 255;
		    (b->color)[2] = 127;
		}
		row = -1;
	    }
	}
    }
    return TRUE;
}

void reset_block(){
    int col,row;
    for(row = 0; row < BLOCK_ROW; row++){
	for(col = 0; col < BLOCK_COL; col++){
	    block *b = get_block(row,col);
	    if(row < 6){
		(b->color)[0] = 0;
		(b->color)[1] = 0;
		(b->color)[2] = 0;
	    }
	    else{
		int ran = random()%4;
		if(ran == 0){
		    (b->color)[0] = 169;
		    (b->color)[1] = 65;
		    (b->color)[2] = 201;
		}
		else if(ran == 1){
		    (b->color)[0] = 99;
		    (b->color)[1] = 148;
		    (b->color)[2] = 220;
		}
		else if(ran == 2){
		    (b->color)[0] = 136;
		    (b->color)[1] = 232;
		    (b->color)[2] = 112;
		}
		else if(ran == 3){
		    (b->color)[0] = 255;
		    (b->color)[1] = 255;
		    (b->color)[2] = 127;
		}
	    }
	}
    }

}
