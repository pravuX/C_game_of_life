#include <stdio.h>
#include <unistd.h>
#define HEIGHT 15 // no of rows
#define WIDTH  15 // no of cols

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
void initialize_board() {
  int i, j;
  for (i = 0; i < HEIGHT; i++)
    for (j = 0; j < WIDTH; j++) {
      if (i == 0 && j == 0 ||
          i == 1 && j == 1 ||
          i == 1 && j == 2 ||
          i == 2 && j == 0 ||
          i == 2 && j == 1) {
        current_board.grid[i][j] = alive;
      } else current_board.grid[i][j] = dead;
    }
}

// display the board
void render_board() {
  int i, j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++)
      switch (current_board.grid[i][j]) {
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
  // row offset, column offset and number of neighbors
  int or, oc, n;
  n = 0;
  for (or = -1; or <= 1; or++)
    for (oc = -1; oc <=1; oc++) {
      n += current_board.grid[mod((row + or), HEIGHT)][mod((col + oc), WIDTH)];
    }
  // subtract the cell itsel from the neighbor count
  return n - current_board.grid[row][col];
}

// calculate the next generation
void next_generation() {
  int i, j, n;
  // copy the current board
  next_board = current_board;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      n = no_of_neighbors(i, j);
      switch (current_board.grid[i][j]) {
        case alive:
          // if a live cell has less than 2 or greater than 3 neighbors
          // it dies, else it remains alive in the next generation.
          if (n < 2 || n > 3)
            next_board.grid[i][j] = dead;
          break;
        case dead:
          // if a dead cell has exactly 3 neighbors it becomes alive in the
          // generation, else it remains dead.
          if (n == 3)
            next_board.grid[i][j] = alive;
          break;
      }
    }
  }
  current_board = next_board;
}

int main() {
  initialize_board();
  while (1) {
    printf("\e[1;1H\e[2J"); // clear the screen
    render_board();
    next_generation();
    msleep(250); // sleep for 250 milliseconds
  }
  return 0;
}
