#include <stdio.h>

/*

Written by Montek Singh
Copyright and all rights reserved by Montek Singh
Last Updated:  April 4, 2025

Permission granted to use this only for students, teaching/learning assistants
and instructors of the COMP 541 course at UNC Chapel Hill.
For any other use, contact Montek Singh first.

*/


/*

This is a C template for initial development
of your demo app for COMP541 find projects!v

You must compile and run this code in an ANSI
compatible terminal.  You can use the terminal app
in the course VM.  For macOS and Linux users, the
standard terminal/shell on your laptop is also ANSI
compatible.

Open a terminal and compile and run the code as follows:

	gcc code.c
	./a.out

*/



/* Specify the keys here that get_key() will look for,
returning 1 if the first key was found, 2, for the second key, etc.,
and returning 0 if none of these keys was found.
In the actual board-level implementation, you will define
scancodes instead of characters, and you can use specify 
key releases as well.
*/

int key_array[] = {'r','p','w','a','s','d','1','2'}; 	// define as many as you need

/* Specify the keys here that get_key2() will look for. */

int key_array2[] = {}; 	// define as many as you need


/* Let us define our sprites.  These will be text approximations
of the actual sprites used in the board implementation.
Here, each sprite is specified by:
	{ text character, foreground color, background color }

For example, specifying a sprite as
	{'.', white, red},
means it is drawn as a white dot over a red background.

Specify the number of sprites first (Nchars), and then the
attributes of each sprite.
*/

// type definition for emulating sprites (see below)
typedef struct {
	char char_to_display;
	int fg_color;
	int bg_color;
} sprite_attr;


#define Nchars 36
#define HORI_TRACK_1 5
#define HORI_TRACK_2 3
#define VERT_TRACK_1 6
#define VERT_TRACK_2 4
#define CURVE_DOT 26
#define CURVE_CENTER 27
#define CURVE_DOT_2 30
#define CURVE_CENTER_2 31

enum colors {black, red, green, yellow, blue, magenta, cyan, white};

sprite_attr sprite_attributes[Nchars] = {
	{'.', white, red},
	{'S',white,red},
	{'D',white,red},
	{'-',blue, yellow},
	{'|',blue,yellow},
	{'-', white, yellow},
	{'|',white,yellow},
	{'-',green,green},
	{'X',blue, green},
	{'-',red,yellow},
	{'|',red,yellow},
	{'.',black,black}, //don't need
	{'n',cyan,magenta},
	{'u',cyan,magenta},
	{'T',magenta,green},
	{'R',magenta,green},
	{'A',magenta,green},
	{'I',magenta,green},
	{'N',magenta,green},
	{'P',magenta,green},
	{'L',magenta,green},
	{'E',magenta,green},
	{'V',magenta,green},
	{'1',magenta,green},
	{'2',magenta,green},
	{'.', white, white}, //don't need
	{'.',white,yellow},
	{'*',white,yellow},
	{'.',red,yellow},
	{'*',red,yellow},
	{'.',blue,yellow},
	{'*',blue,yellow},
	{'2',magenta,green},
	{'P',magenta,green},
	{'S',magenta,green},
	{'O',magenta,green},
	
	//{'O', black, yellow},
	//{'\\', blue, white}
};


//===============================================================
// Here are the functions available for I/O.  These correspond
// one-to-one to functions available in MIPS assembly in the
// helper files provided.
//
// NOTE:  There is one function specific to the C implementation
// that is not needed in the assembly implementation:
//     void initialize_IO(char* smem_initfile);
//===============================================================

void my_pause(int N);  	// N is hundredths of a second

void putChar_atXY(int charcode, int col, int row);
	// puts a character at screen location (X, Y)

int getChar_atXY(int col, int row);
	// gets the character from screen location (X, Y)

int get_key();
	// if a key has been pressed and it matches one of the
	// characters specified in key_array[], return the
	// index of the key in that array (starting with 1),
	// else return 0 if no valid key was pressed.

int get_key2();
	// similar to get_key(), but looks for key in
	// key_array2[].

int pause_and_getkey(int N);
	// RECOMMENDED!
	// combines pause(N) with get_key() to produce a 
	// *responsive* version of reading keyboard input

void pause_and_getkey_2player(int N, int* key1, int* key2);
	// 2-player version of pause_and_getkey().

int get_accel();
	// returns the accelerometer value:  accelX in bits [31:16], accelY in bits [15:0]
	// to emulate accelerometer, use the four arrow keys

int get_accelX();
	// returns X tilt value (increases back-to-front)

int get_accelY();
	// returns Y tilt value (increases right-to-left)
	

void background_color(enum colors c);
void put_sound(int period);
	// visually shows approximate sound tone generated
	// you will not hear a sound, but see the tone highlighted on a sound bar

void sound_off();
	// turns sound off

void put_leds(int pattern);
	// put_leds: set the LED lights to a specified pattern
	//   displays on row #31 (below the screen display)

void initialize_IO(char* smem_initfile);

int dfs();
int visit(int visited[30][40], int r, int c);
int is_valid(int visited[30][40], int from_r, int from_c, int to_r, int to_c);
void run_train();
void lvl2();

//===============================================================
// This is the code for your demo app!
//===============================================================

int horizontal_track = 1; //whether orientation of the straight track is horizontal
int straight_track=1; 	//whether currently selected track is straight or curvy
int curvy_r = 0;	//which orientation/rotation of curvy track is currently selected (to be placed down)	
int curvy_hovered=0;	//rotation/orientation of the curvy track being hovered ovder
 //ordered row,col UNLIKE the function is:
int destarr[4][2] = {
	{13,23},
	{24,23},
	{21,16},
	{23,23}
};
int startarr[4][2] = {
	{24,23},
	{11,23},
	{22,23},
	{19,16}
};

int dest[] = {13,23};
int start[] = {24,23};
int numrows = 30;
int numcols=40;
int path[900][2]; //storing the path for the train
int path_len=0;
int counter = 30;
int oldpath[900][2]; //path to check against when completing second half of a level
int oldpath_len=1;
int check_old=0;
int old_train_dest_u[2] = {50,0}; 
int old_train_dest_n[2] = {50,0};
int first_level_done=0;
int level_two_part_one_done=0;
int levels_done=0;
int second_oldest_path[900][2];


int s_char_placements[4][2] = {{0,0},
			{14,25},
			{23,25},{0,0}};
int d_char_placements[4][2] = {{0,0},
				{23,25},
				{20,14},
				{0,0} };
			
				

//initialize it with irrelevant out-of-bounds value as a 'flag'--so that the first half of lvl not affected
	

int add_track_straight(int x, int y){
	if(horizontal_track == 1){
	putChar_atXY(5,x,y);
	return 5;
	}
	else {putChar_atXY(6,x,y); return 6;}	
}

int abs(int x){
if(x<0){return (x * -1);}
return x;
}

int add_track_curvy(int x, int y){
int p1 = getChar_atXY(x+1,y);
int p2 = getChar_atXY(x,y+1);
int p3 = getChar_atXY(x-1,y);
int p4 = getChar_atXY(x,y-1);
switch (curvy_r) {
			case 0: 
				if(p1==7 && p2==7){
				if(p3!=26 && p4!=26){
				putChar_atXY(27,x,y);
				putChar_atXY(26, x+1,y);
				putChar_atXY(26,x,y+1);
				return 27; break;
				}}
				break;
			case 1:
				if(p3==7 && p2==7){
				if(p1!=26 && p4!=26){
				putChar_atXY(27,x,y);
				putChar_atXY(26,x-1,y);
				putChar_atXY(26, x,y+1);
				return 27; break;}}
				break;
			case 2:
				if(p3==7 && p4==7){
				if(p1!=26 && p2!=26){
				putChar_atXY(27,x,y);
				putChar_atXY(26,x-1,y);
				putChar_atXY(26,x,y-1);
				return 27;break;}}
				break;
			case 3:
				if(p1==7 && p4==7){
				if(p3!=26 && p2!=26){
				putChar_atXY(27,x,y);
				putChar_atXY(26,x+1,y);
				putChar_atXY(26,x,y-1);
				return 27; break;}}
				break;
			}


}
void remove_curvy_track(int col,int row){
	putChar_atXY(7,col,row);
	switch (curvy_hovered){
		case 0:
			putChar_atXY(7,col+1,row);
			putChar_atXY(7,col,row+1);
			break;
		case 1:
			putChar_atXY(7,col-1,row);
			putChar_atXY(7,col,row+1);
			break;
		case 2:
			putChar_atXY(7,col-1,row);
			putChar_atXY(7,col,row-1);
			break;
		case 3:
			putChar_atXY(7,col+1,row);
			putChar_atXY(7,col,row-1);
			break;
	}
}
int is_curvy_char(int ch) {
    return (ch == 26 || ch == 27 || ch==28);  // '.' or '*'
}
int dfs(){
 int visited[30][40] = {0}; //array keeping track of visited squares
 return visit(visited,start[0],start[1]);
}

int is_valid(int visited[30][40], int from_r, int from_c, int to_r, int to_c){ //r and c represent square we could possibly move to
	if((to_r>=0 && to_r<numrows) && (to_c>=0 && to_c<numcols)){
		if (!(visited[to_r][to_c])){
			if(to_r==dest[0] && to_c==dest[1]){return 1;}
			if(check_old){ //aka, if we DO have to care about validating against old path. only relevant for second level
				for(int i=0;i<oldpath_len;i++){
					if(oldpath[i][0]==to_r && oldpath[i][1]==to_c){ //found the track in the old path
						return 0;}}}
			int to_char = getChar_atXY(to_c,to_r); //square we cld move to
			int from_char = getChar_atXY(from_c, from_r); //square we r on
			int dr = to_r - from_r; //0
			int dc = to_c - from_c; //-1
			if (from_char == VERT_TRACK_1 || from_char == VERT_TRACK_2) {
				if ((dr == -1 || dr == 1) && dc == 0) {
					if (to_char == VERT_TRACK_1 || to_char == VERT_TRACK_2 || to_char == CURVE_DOT || to_char == CURVE_DOT_2){ return 1;}
				}
			}
			
			else if (from_char == HORI_TRACK_1 || from_char == HORI_TRACK_2) {
				if ((dc == -1 || dc == 1) && dr == 0) {
					if (to_char == HORI_TRACK_1 || to_char == HORI_TRACK_2 || to_char == CURVE_DOT || to_char==CURVE_DOT_2) {return 1;}
				}
			}
			
			else if (from_char == CURVE_DOT || from_char==CURVE_DOT_2) {
				
			// find the * next to it
				if (to_char == CURVE_CENTER || to_char==CURVE_CENTER_2) {
		
				// allow movement to center
					if ((abs(dr) + abs(dc)) == 1){ 
					return 1;}
				}
				else if ((abs(dr) + abs(dc)) == 1) {
				// check for a * adjacent to dot in one direction
					int star_r = from_r + dr; //22
					int star_c = from_c + dc; //21
					if (getChar_atXY(star_c, star_r) == CURVE_CENTER 
					|| getChar_atXY(star_c, star_r) == CURVE_CENTER_2) {
					// try to move to the far side of the star. IT CANT FIND STAR
						int next_r = from_r - dr;
						int next_c = from_c - dc;
						int next_char = getChar_atXY(next_c, next_r);
						if (dr != 0 && (next_char == VERT_TRACK_1 || next_char == VERT_TRACK_2)) return 1;
						if (dc != 0 && (next_char == HORI_TRACK_1 || next_char == HORI_TRACK_2)) return 1;
								} //ISSUE is with moving from the curve dot!
					else{
						int star_r = from_r-dr;
						int star_c = from_c-dc;
						if (getChar_atXY(star_c, star_r) == CURVE_CENTER 
						|| getChar_atXY(star_c, star_r) == CURVE_CENTER_2) {
							int next_r = from_r+dr;
							int next_c = from_c+dc;
							int next_char = getChar_atXY(next_c,next_r);
							if (dr != 0 && (next_char == VERT_TRACK_1 || next_char == VERT_TRACK_2)) {
								return 1;}
							if (dc != 0 && (next_char == HORI_TRACK_1 || next_char == HORI_TRACK_2)) {
								return 1;}
							}
					}
							}
				}
			else if (from_char == CURVE_CENTER || from_char==CURVE_CENTER_2) {
			// only allow movement into a dot
				if ((abs(dr) + abs(dc)) == 1 && (to_char == CURVE_DOT || to_char==CURVE_DOT_2)) return 1;
				}
			



			//if((horiz && curr==5) || (horiz && curr==3) || (!(horiz) && curr==6) || (!(horiz) && curr==4)){
			//'5' and '3' are horizontal tracks. '6' and '4' are vertical tracks. '26' is the dot and '27' is the *.
			//return 1;} //the 'horiz' variable ensures that if we are moving l/r it is a horizontal track, and a vertical track otherwise
		}}return 0;}
int visit(int visited[30][40], int r, int c){
	if(r==dest[0] && c==dest[1]){
	path[path_len][0]=r;
	path[path_len][1]=c;
	path_len++;
	return 1;}
	visited[r][c] = 1;
	path[path_len][0]=r;
	path[path_len][1]=c;
	path_len++;
	int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
	for(int i=0; i<4;i++){
		int new_row = r+directions[i][0];
		int new_col = c + directions[i][1];
		if (is_valid(visited, r, c, new_row, new_col)){
			//if(levels_done == 3){
			//putChar_atXY(11, c-15,r);}
			if(visit(visited, new_row, new_col)){
				return 1;}}
	}
	
	path_len--;
	return 0;	
	}

void delay_roughly_ms(int ms) {
    for (int m = 0; m < ms; m++) {
        for (int i = 0; i < 10000; i++);  // Tune this number!
    }
}

//void run_infinite_train();
	
void run_train(){ //chars 13 and 14 are the n and u respectively
levels_done++;
put_sound(2);
if(old_train_dest_u[0] !=50){ //aka second half of lvl1 complete
	first_level_done=1;
	putChar_atXY(6, old_train_dest_u[1],old_train_dest_u[0]);
	putChar_atXY(6,old_train_dest_n[1],old_train_dest_n[0]);}

int charsonpath[path_len];
for (int i=0;i<path_len;i++){
charsonpath[i] = getChar_atXY(path[i][1],path[i][0]); //save all curr chars so we can replace them while train moves
}
putChar_atXY(13, path[0][1], path[0][0]);
putChar_atXY(12,path[1][1],path[1][0]);

int nextold = charsonpath[0];
int i=1;
int counter=0;
while(i < path_len-1){
if (counter == 100000000){
	putChar_atXY(nextold,path[i-1][1],path[i-1][0]);
	nextold = charsonpath[i];
	putChar_atXY(13, path[i][1], path[i][0]);
	putChar_atXY(12,path[i+1][1],path[i+1][0]);
	counter = 0;
	i++;
	}
	else{
	counter++;}
}
	
putChar_atXY(12, path[path_len-1][1], path[path_len-1][0]-1);
putChar_atXY(13, path[path_len-1][1], path[path_len-1][0]);
putChar_atXY(6,path[path_len-2][1],path[path_len-2][0]);
old_train_dest_n[0]=path[path_len-1][0]-1;
old_train_dest_n[1]=path[path_len-1][1];
old_train_dest_u[0] = path[path_len-1][0];
old_train_dest_u[1] = path[path_len-1][1];
/*if(old_train_dest_u[0] !=50){
	putChar_atXY(11,old_train_dest_u[1],old_train_dest_u[0]);
		putChar_atXY(11,old_train_dest_n[1],old_train_dest_n[0]);}*/
//old start is where the u is, aka char 13. new start is that but row=row-2
//FIRST: add the track behind the train rn to the set of 'path' aka tracks that we can't backtrack on
//then alter the algorithm to be a normal dfs EXCEPT, is_valid has to include checking to make sure the track is not a part of
//the old path

sound_off();
int new_s_r = s_char_placements[levels_done][0];
int new_s_c = s_char_placements[levels_done][1];
int new_d_r = d_char_placements[levels_done][0];
int new_d_c = d_char_placements[levels_done][1];

putChar_atXY(1,new_s_c,new_s_r); //change start char
putChar_atXY(2,new_d_c,new_d_r); //change dest char

check_old = !check_old;
putChar_atXY(check_old, 0, 11);

if(levels_done == 2){
	lvl2();
}
if(levels_done==3){check_old=0;}


int temp[2];
for(int i=0;i<2;i++){
start[i] = startarr[levels_done][i];
dest[i] = destarr[levels_done][i];
}
//putChar_atXY(11,start[1],start[0]);
//putChar_atXY(14,dest[1],dest[0]);
//putChar_atXY(check_old,0,20);

if(levels_done==2){
	for(int i=0;i<oldpath_len;i++){
	second_oldest_path[i][0] = oldpath[i][0];
	second_oldest_path[i][1] = oldpath[i][1];
	}}
oldpath[path_len][0] = path[path_len-1][0]+1;
oldpath[path_len][1] = path[path_len-1][1]; //add that last track that isn't aprt of the track, to it, so that user can't use it on
						// next part of level
oldpath_len = path_len+1;


for(int i=0;i<path_len;i++){
	if(!((path[i][0] == dest[0]) && (path[i][1] == dest[1]))){
	oldpath[i][0] = path[i][0];
	oldpath[i][1] = path[i][1];}
	else{continue;}
}
path_len=0; //now we can reset pathlen to 0
}

void lvl2(){
	putChar_atXY(0,13,20);
	putChar_atXY(0,15,20);
	putChar_atXY(4, 16, 23);
	putChar_atXY(0,13,21);
	putChar_atXY(0,14,21);
	putChar_atXY(0,15,21);
	putChar_atXY(4, 16, 22);
	putChar_atXY(0,25,14);
	putChar_atXY(4, 16, 21);
	putChar_atXY( 24, 6,2);
	}


int check_accel(int accelx, int accely){
//return 0 for no change, 1 for up, 2 for left, 3 for down, 4 for right
//if(!flag){return flag;}
//put the logic of moving the character into here instead of returning out
if(accelx > 286 ){return 1;}
if(accelx < 226){return 3;}
if(accely > 286){return 2;}
if(accely < 226){return 4;}
}

int st_track_row = 23;
int st_track_col = 24;
int end_track_row = 23;
int end_track_col=13;

int main() {

	initialize_IO("smem.mem");
	int check_old = 0;
	int row = 15; 
	int col = 20;
	int key1 = 0; 
	int key1new;
	putChar_atXY(8,col,row);
	int nextold=7;
	int accelx, accely;
	int xflag=1;
	int yflag = 1;
	while(1){

	key1 = pause_and_getkey(1);
	accelx = get_accelX();
	accely = get_accelY();
	//int move=0;
	//move = check_accel(accelx,accely);
	
	/*if(accelx > 286  || accelx<226){}
	if(accelx > 286 ){return 1;}
	if(accelx < 226){return 3;}
	if(accely > 286){return 2;}
	if(accely < 226){return 4;}*/
	
	/*if(move != 0){ //if an accelerometer change occured
		//if(flag==0){flag=1;}else{flag=0;}
		int oldrow = row;
		int oldcol=col;
		switch(move){
			case 1: row--; if(row<0) row=0; break; //w
			case 2: col--; if(col<0) col=0; break; //a
			case 3: row++; if(row>29) row=29; break;//s
			case 4: col++; if(col>39) col=39; break;//d
		} //at this point, row and col represent the square user is attempting to move to
		int prospchar = getChar_atXY(col,row);
		if(prospchar > 4){ //ensure not trying to move onto a piece of the background stations/tracks
			putChar_atXY(nextold,oldcol,oldrow); //replace current x with what used to be there
			nextold = prospchar;		     //record the old char so we can replace it
			if(prospchar == 5){putChar_atXY(9,col,row);}
			if(prospchar==6){putChar_atXY(10,col,row);}	     
			if(prospchar == 7){ //if green square, we can move the X
				putChar_atXY(8,col,row);	     //put the X in the desired position
				}
			}
			else{ //if we tried to move onto a bg char, revert the row/column updates
				row = oldrow;
				col = oldcol;
			}
	}*/
	if(key1 !=0){
		//we only want to display the 'x' on green squares. else, update row and column and replace
				//the X w what used to be there, but don't put any "X" char
			//unless its blue tracks or the stations. then, don't put any char, and don't update row and column.	
			
		int oldrow = row;
		int oldcol=col;

		if(key1 == 1){ //rotate key being pressed
			if(straight_track){
			if(horizontal_track == 1){horizontal_track = 0;} else{horizontal_track=1;}} //keeping track of orientaiton of track
			else{
			if(curvy_r == 3){curvy_r = 0;} //set curvy track to one of four orientations
			else{curvy_r++;}
			}
			}
		if(key1==7){straight_track=1;}
		if(key1 == 8){ //'2', switch to vertical
			straight_track = 0;
			}
		int cur = getChar_atXY(col,row); //check the char at the current spot. should be a red track ?
		if((cur>4) && (cur!=12) && (cur!=13) && cur!=26){ //make sure not drawing on top of stations or train

			if(key1 == 2){ ////if 'p' key being pressed 
				//printf("cur is %d/n",cur);
				if(nextold == 27 || cur==27){
				remove_curvy_track(col,row);
				}
				else if(cur==5 || cur==6 || nextold==5 || nextold==6) { //if white track square, aka a track we r hovering over
					putChar_atXY(7,col,row); //remove the track
					nextold=7;
					}
				else{
				if(straight_track){nextold = add_track_straight(col,row);}
				else{nextold = add_track_curvy(col,row);}}
				int win = dfs();
				putChar_atXY(win,0,0);
				if(win){
				
				run_train();}
				/*if(cur!=3 && cur!=4){
					if(cur!=1 && cur!=2){ //if no train track alr there, add one
						 nextold = add_track_straight(col,row);}
					else{putChar_atXY(7,col,row);
					     nextold = 7;
					} //else, remove one
				}*/
				//when i click p on a red track, it turns white. it is placing 6 on top of 10
			
			}}
	if(key1>=2 && key1<=6){ //w,a,s,d
		switch (key1) {
			case 3: row--; if(row<0) row=0; break; //w
			case 4: col--; if(col<0) col=0; break; //a
			case 5: row++; if(row>29) row=29; break;//s
			case 6: col++; if(col>39) col=39; break;//d
			}
			//at this point, row and col represent the square user is attempting to move to:
			int prospchar= getChar_atXY(col,row);
			
			if((prospchar > 4) && (prospchar != 30) && (prospchar !=31)){ //ensure not trying to move onto a piece of the background stations/tracks
				putChar_atXY(nextold,oldcol,oldrow); //replace current x with what used to be there
				if(nextold==27 ){
				putChar_atXY(27,oldcol,oldrow);
				
					//use nextold array instead of singular nextold
				}
				
				nextold = prospchar;		     //record old char that x will replace, so we can replace it next time
				if(prospchar == 5){putChar_atXY(9,col,row);} //if hovering over a track
				if(prospchar==6){putChar_atXY(10,col,row);}  // ^
			
				if(is_curvy_char(prospchar)){
					int found_center=0;
					int cx,cy;
					if (getChar_atXY(col, row) == 27 && 
					getChar_atXY(col+1, row) == 26  //orientation 1
					&& getChar_atXY(col, row+1) == 26) {
        					cx = col; cy = row; curvy_r = 0; found_center = 1;}
        				else if (getChar_atXY(col, row) == 27 && 
        				getChar_atXY(col-1, row) == 26 
        				&& getChar_atXY(col, row+1) == 26) {
        					 cx = col; cy = row; curvy_r = 1; found_center = 1;}
        				else if (getChar_atXY(col, row) == 27 &&
             				getChar_atXY(col-1, row) == 26 &&
             				getChar_atXY(col, row-1) == 26) {
       				 		cx = col; cy = row; curvy_r = 2; found_center = 1;}
       				 	 else if (getChar_atXY(col, row) == 27 &&
       				 	 getChar_atXY(col,row-1)==26 && 
       				 	 getChar_atXY(col+1,row)==26){
        					cx = col; cy = row; curvy_r = 3; found_center = 1;}
        					
        				
        				if(found_center){
        					putChar_atXY(29,cx,cy);
        					curvy_hovered=curvy_r;
                						}
				}
				
				
				
				
				if(prospchar == 7   || prospchar==8){ //if green bg square, we can move the X
				putChar_atXY(8,col,row);		//put the X in the desired position
				if(prospchar==8){nextold = 7;}	    //if we were in the corner and tried to move off grid, once we move 
								    //back inside, put a bg char in our place
				}
			}
			else{ //if we tried to move onto a station/bluetrack char, revert the row/column updates
				row = oldrow;
				col = oldcol;
			}
			}
			
		
		}
	
	
	/*
	int row = getrow();
	int col = getcol();
	//putChar_atXY(3,accely,accelx);
	//int track = pause_and_getkey(2);
	if(!(row>=24 && row<=26 && ((col<=14 && col>=13) || (col<=24 && col>=23)))){ //make sure not drawing on top of stations
		int cur = getChar_atXY(row,col); //check the char at the current spot
			if(rotate == 2){ ////if 'p' key being pressed 
			if(cur!=3 && cur!=4){
			if(cur!=1 && cur!=2){ //if no train track alr there, add one
			add_track_straight(row,col);}
			else{putChar_atXY(6,row,col);} //else, remove one
				}
			}}*/
		my_pause(10);
	}
	
	
	return 0;
	

	// YOUR CODE GOES BELOW
	// ...

}


// The file below has the implementation of all of the helper functions
#include "procs.c"
