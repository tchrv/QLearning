#ifndef __BOARD_H
#define __BOARD_H

#if defined(__cplusplus)
extern "C" {
#endif

#define COLSIZE 4
#define SIZE (COLSIZE * COLSIZE)

#define MOVE_UP 0
#define MOVE_RIGHT 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3

typedef struct board_t {
    /* stores the numbers on the board. 0 means empty. */
    int cells[SIZE];
    int score;
    /* bitmap of the cells that's occupied. E.g. 0xFFFF means every square of a 4x4 board is occupied. */
    int occupied_cells;
} board_t;

board_t board_init();
void board_print(board_t);
void board_move(board_t*, int);
void board_setup_bitfields(board_t*);
int board_value_at(board_t*, int x, int y);
void board_set_value_at(board_t*, int x, int y, int value);

#if defined(__cplusplus)
}
#endif

#endif
