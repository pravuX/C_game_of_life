#include <stdio.h>
#include <unistd.h>
#define HEIGHT 21 // no of rows
#define WIDTH  21 // no of cols

int mod(int a, int b) {
  // calculates a mod b
  // a >= 0, a mod b = a % b
  // a < 0, a mod b = b + a % b
  int res = a % b;
  return res < 0 ? res + b : res;
}

// sleep for give no. of milliseconds
void msleep(int ms) {
  usleep(ms * 1000);
}

// each cell can either be dead(0) or alive(1)
typedef enum {
  dead,
  alive
} cell;

// an array of cells forms a board
typedef struct {
  cell grid[HEIGHT][WIDTH];
} board;

board current_board;
board next_board;

// initialize the board as a glider
void initialize_glider() {
  int row, col;
  for (row = 0; row < HEIGHT; row++)
    for (col = 0; col < WIDTH; col++) {
      if (row == 0 && col == 0 ||
          row == 1 && col == 1 ||
          row == 1 && col == 2 ||
          row == 2 && col == 0 ||
          row == 2 && col == 1) {
        current_board.grid[row][col] = alive;
      } else current_board.grid[row][col] = dead;
    }
}

// initialize the board as a pulsar
void initialize_pulsar() {
  int row, col;
  for (row = 0; row < HEIGHT; row++)
    for (col = 0; col < WIDTH; col++) {
      if (row == 10 && (col ==  5 || col ==  6 ||
                        col ==  7 || col ==  8 || col ==  9 ||
                        col == 11 || col == 12 || col == 13 ||
                        col == 14 || col == 15)) {
        current_board.grid[row][col] = alive;
      } else current_board.grid[row][col] = dead;
    }
}

// display the board
void render_board() {
  int row, col;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++)
      switch (current_board.grid[row][col]) {
        case dead:
          printf("░░");
          break;
        case alive:
          printf("██");
          break;
      }
    putchar('\n');
  }
}

// for a cell at (row, col) the subgrid from
// row-1 to row+1 and col-1 to col+1 represents
// it's neighborhood. add alive neighbors to the
// neighbor count.
int no_of_neighbors(int row, int col) {
  int row_offset, col_offset, n;
  n = 0; // initialize no of neighbors
  for (row_offset = -1; row_offset <= 1; row_offset++)
    for (col_offset = -1; col_offset <=1; col_offset++) {
      n += current_board.grid[mod((row + row_offset), HEIGHT)][mod((col + col_offset), WIDTH)];
    }
  // subtract the cell itsel from the neighbor count
  return n - current_board.grid[row][col];
}

// calculate the next generation
void next_generation() {
  int row, col, n;
  // copy the current board
  next_board = current_board;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      n = no_of_neighbors(row, col);
      switch (current_board.grid[row][col]) {
        case alive:
          // if a live cell has less than 2 or greater than 3 neighbors
          // it dies, else it remains alive in the next generation.
          if (n < 2 || n > 3)
            next_board.grid[row][col] = dead;
          break;
        case dead:
          // if a dead cell has exactly 3 neighbors it becomes alive in the
          // generation, else it remains dead.
          if (n == 3)
            next_board.grid[row][col] = alive;
          break;
      }
    }
  }
  current_board = next_board;
}

int main() {
  initialize_pulsar();
  while (1) {
    render_board();
    next_generation();
    msleep(250); // sleep for 250 milliseconds
    printf("\033[%dA\033[%dD", HEIGHT, WIDTH); // clear the screen
  }
  return 0;
}
