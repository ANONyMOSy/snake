// This program is a simple snake game in C that runs from your terminal.
// To run the game navigate to the snake directory wherever you downloaded it.
// Compile an a.out file using your C compiler (ex: cc snake.c -lncurses) make sure to link the curses library.
// Run the game using the command ./a.out and using the arrow keys to control.
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
                int x;
                int y;
} Vector2;

int currentScore = 0;

Vector2 body[255];

void endGame() {
	endwin();
	printf("Finished game with a score of: %d\n", currentScore);

	exit(0);
}	

int main() {
	// initialize game dimensions
	int windowWidth = 40;
	int windowHeight = 40;

	// initscr() initializes the screen
	initscr();

	// first gather player input using the functions of the curses library
	nodelay(stdscr, true); // forces game to keep running with no input
	curs_set(0); // hide the cursor
	keypad(stdscr, true); // start gathering input from the keypad
	
	// snake position
	Vector2 first = { 20, 10 }; // set the initial position of the head
	Vector2 facing = { 1, 0 };
	
	// fruit position (randomize)
	Vector2 fruit = { rand() % (windowWidth - 2) + 1, rand() % (windowHeight - 2) + 1 };
	
	// game loop
	while(true) {
		// ---------------- INPUT ---------------------------------------
		
		int update = wgetch(stdscr); // gets the value of the inputed key
		if (update == KEY_UP) {
			// first check if the current direction is the opposite of the desired direction, and prevent a full turn
			if (facing.y == -1) {
				continue;
			}
			facing.y = -1;
			facing.x = 0;
		} if (update == KEY_DOWN) {
			if (facing.y == -1) {
				continue;
			}
			facing.y = 1;
			facing.x = 0;
		} if (update == KEY_RIGHT) {
			if (facing.x == -1) {
				continue;
			}
			facing.x = 1;
			facing.y = 0;
		} if (update == KEY_LEFT) {
			if (facing.x == 1) {
				continue;
			}
			facing.x = -1;
			facing.y = 0;
		} if (update == '\e') {
			endGame();	
		}

		// --------------- UPDATE LOGIC ---------------------------------
			
		// update body
		for (int i = currentScore; i > 0; i--) {
			body[i] = body[i - 1];
		}
		// body should always start with the first
		body[0] = first;

		// use the facing variable to change the direction of the snake
		first.x += facing.x;
		first.y += facing.y;

		// check if the head has touched the border
		if (first.x <= 0 || first.x >= windowWidth || first.y <= 0 || first.y >= windowHeight) {
			endGame();
		}
		
		// check if the head has touched any part of the body
		for (int i = 0; i < currentScore; i++) {
			if(first.x == body[i].x && first.y == body[i].y) {
				endGame();
			}
		}
		
		// check if the head has contacted a fruit
		if(first.x == fruit.x && first.y == fruit.y) {
			currentScore += 1;
			
			// instead of spawning a new fruit, just relocate it
			fruit.x = rand() % windowWidth;
			fruit.y = rand() % windowHeight;
		}	
		// --------------- UPDATE SCREEN --------------------------------
		
		// clear last frame
		erase();

		// draw borders
		for (int i = 0; i < windowWidth * 2; i++) {
			mvaddch(0, i, '-'); // top border
			mvaddch(windowHeight, i, '-'); // bottom border
		}
		for (int i = 0; i < windowHeight; i++) {
			mvaddch(i, 0, '|'); // left border
			mvaddch(i, (windowWidth * 2), '|'); // right border
		}
		
		// draw fruit
		mvaddch(fruit.y, fruit.x * 2, '?');
		
		// draw player
		for (int i = 0; i < currentScore; i++) {
			mvaddch(body[i].y, body[i].x * 2, '~');
		}
		mvaddch(first.y, first.x * 2, '>'); // multiplies x by two to ensure direction in the x and y is the same speed

		// frame delay
		usleep(100000);			
	}
}
