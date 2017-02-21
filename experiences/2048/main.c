#include "engine2048.h"

#include <stdlib.h>
#include <stdio.h>


int main() {
	board_t board = board_init();

	while(1) {
		board_print(board);
		char c = getchar();
		switch(c) {
			case 'u':
				board_move(&board, MOVE_UP);
				break;
			case 'r':
				board_move(&board, MOVE_RIGHT);
				break;
			case 'd':
				board_move(&board, MOVE_DOWN);
				break;
			case 'l':
				board_move(&board, MOVE_DOWN);
				break;
		}


	}
}
