/*
 * Eating Candy Game. A typical video game can remove the blocks and get
 * score. If you can have a high score, then you can put your name and score
 * on the TOP5 list.
 *
 * May 9th
 * Jason
 */
#include "game.h"

SDL_Color textColorWhite = {255, 255, 255};
SDL_Surface *screen = NULL;
SDL_Surface *congrat;
SDL_Surface *labelReset = NULL;
SDL_Surface *button = NULL;
SDL_Surface *top = NULL;
SDL_Surface *top0 = NULL;
SDL_Surface *top1 = NULL;
SDL_Surface *top2 = NULL;
SDL_Surface *top3 = NULL;
SDL_Surface *top4 = NULL;
SDL_Surface *labelStart = NULL;
SDL_Surface *labelResetInf = NULL;
SDL_Surface *labelStartInf = NULL;
SDL_Surface *restart = NULL;
SDL_Surface *button_save = NULL;
SDL_Surface *scores = NULL;
SDL_Surface *your_score = NULL;
SDL_Surface *lose_game = NULL;
SDL_Surface *name = NULL;
int score;
SDL_Surface *label_save = NULL;
int name_index;
char *str_score = NULL;
TTF_Font *font = NULL;
int num_block = 0;
int game_start = FALSE;
time_t last_time_add;
time_t start_time;
linked_list_t *row0 = NULL;
char *buffer = NULL;
linked_list_t *top_list;

int main(int argc, char **argv){
    int shutdown;
    int i,num,num2;
    //Event structure
    SDL_Event event;
    size_t fret;
    int ret;
    int duration = 5;
    FPSmanager framerate_manager;
    if((ret = initial_screen()) != 0)
	return ret;
    
    if ((ret = set_screen()) != 0)
	return ret;
    
    creat_list_of_list();
    fill_list_of_list();
    
    // open file
    FILE *fd = fopen("record.bin","r");
    if( NULL == fd ){
	fprintf(stderr,"Err: foppen");
	return -1;
    }

    fret = fread(&num,sizeof(int),1,fd);
    if(fret != 1){
	num = 0;
    }

    for( i = 0; i < num; i++){
	read_file(fd);
    }
    //close file
    fclose(fd);

    // set Framerate
    SDL_initFramerate( &framerate_manager );
    SDL_setFramerate( &framerate_manager, 24 );
    /*
     * Handle events
     */
    // While user hasn't shutdow
    shutdown = TRUE;
    while( TRUE == shutdown ){
	//update the score
	free(str_score);
	str_score = (char*)malloc(sizeof(char)*10);
	sprintf(str_score,"%d",score);
	SDL_FreeSurface(scores);
	scores =  TTF_RenderText_Solid(font, 
				 str_score,
				 textColorWhite);
	
	if(buffer == NULL){
	    buffer = (char*)malloc(sizeof(char)*11);
	    buffer[0] = '\0';
	}

	SDL_FreeSurface(name);
	name =  TTF_RenderText_Solid(font, 
				 buffer,
				 textColorWhite);

	if(game_start == START){
	    time_t c;
	    c = time(NULL);
	    int i =  c - last_time_add;
	    
	    if(i > duration){
		int check = add_new_row();
		if(check == FALSE){
		    game_start = TYPENAME;
		    
		}
		last_time_add = c;
	    }

	    if( c - start_time > 20){
		    start_time = c;
		    duration--;
		}
	    }

		repaint();

	// While there is an event to handle
	// Others:
	//  - SDL_waitEvent()
	//  - SDL_PeepEvents()
	while( SDL_PollEvent( &event )){
	    /*
	     * if the user had X'ed out the window
	     */	
	    if( event.type == SDL_QUIT){
		shutdown = FALSE;
	    }

	    else if( event.type == SDL_KEYDOWN){
		if(game_start == TYPENAME ){
		    print_key_info( &event.key );
		}
	    }
	    /*
	     * If mouse clicked (down)
	     */
	    else if( event.type == SDL_MOUSEBUTTONDOWN ){
		int x = event.button.x;
		int y = event.button.y;
		
		if( event.button.button == SDL_BUTTON_LEFT ){
		    
		    if( (250 <= x && x <= 356) && 
			(300 <= y && y <= 356) && 
			game_start == TYPENAME ){
			
			user_t *u = (user_t*)malloc(sizeof(user_t));
			u->score = score;
			u->name = strdup(buffer);
			linked_list_append(u, top_list);
			linked_list_sort(top_list,
					 cmp);
			
			if(top_list->length > 5){
			    linked_list_remove(5,top_list);
			}
			
			game_start = SHOWTOP;
		    }

		// reset the game when click the reset pic
		    else if( (440 <= x && x <= 613) && (100 <= y && y <= 169) ){
			game_start = BSTART;
			duration = 4;
			score = 0;
			reset_block();
			free(buffer);
			buffer = NULL;
			name_index = 0;
		    }
		    
		    if( game_start < TYPENAME){
			check_block(x,y);
		    }
		}
	    }
	}
    }

    // open record.txt
    fd = fopen("record.bin","w");
    if( NULL == fd ){
	fprintf(stderr,"Err: foppen");
	return -1;
    }
    
    num2 = top_list->length;
    fwrite(&num2, sizeof(int),1,fd);
    

    // put the newest top5 to the file
    for( i = 0; i < num2; i++ ){
	write_file(fd, i);
    }

    //flush and close the file fd
    fclose(fd);

    clean_mem();
    return 0;
}
