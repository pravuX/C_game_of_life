#include <stdio.h>
#include <unistd.h>
#define HEIGHT 21 // no of rows
#define WIDTH  21 // no of cols

int mod(int a, int b) {
  // Calculates a mod b.
  // a >= 0, a mod b = a % b
  // a < 0, a mod b = b + a % b
  int res = a % b;
  return res < 0 ? res + b : res;
}

// Sleep for given no. of milliseconds.
void msleep(int ms) {
  usleep(ms * 1000);
}

// Each cell can either be dead(0) or alive(1).
typedef enum {
  dead,
  alive
} cell;

// An array of cells forms a board.
typedef struct {
  cell grid[HEIGHT][WIDTH];
} board;

board current_board;
board next_board;

// Initialize the board as a glider.
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

// Initialize the board as a pulsar.
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

void initialize_acorn() {
  int row, col;
  for (row = 0; row < HEIGHT; row++)
    for (col = 0; col < WIDTH; col++) {
      if (row ==  9 && (col == 7 || col == 8 || col == 11 || col == 12 || col == 13) ||
          row == 10 && col == 10 ||
          row == 11 && col ==  8) {
        current_board.grid[row][col] = alive;
      } else current_board.grid[row][col] = dead;
    }
}

// Display the board.
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

// For a cell at (row, col) the subgrid from
// row-1 to row+1 and col-1 to col+1 represents
// it's neighborhood. Add alive neighbors to the
// neighbor count.
int no_of_neighbors(int row, int col) {
  int row_offset, col_offset, n, row_pos, col_pos;
  n = 0; // Initialize the neighbor count.
  for (row_offset = -1; row_offset <= 1; row_offset++)
    for (col_offset = -1; col_offset <=1; col_offset++) {
      // for grid that wraps around
      row_pos = mod((row + row_offset), HEIGHT);
      col_pos = mod((col + col_offset), WIDTH);
      n += current_board.grid[row_pos][col_pos];

      // for grid that does not wrap around
      // do not index locations which are out of bounds
      /* row_pos = row + row_offset;
      col_pos = col + col_offset;
      if (row_pos >= 0 && row_pos < HEIGHT &&
          col_pos >= 0 && col_pos < WIDTH)
        n += current_board.grid[row_pos][col_pos]; */
  }
  // Subtract the cell itself from the neighbor count.
  return n - current_board.grid[row][col];
}

// Calculate the next generation.
void next_generation() {
  int row, col, n;
  next_board = current_board; // Copy the current board.

  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      n = no_of_neighbors(row, col);
      switch (current_board.grid[row][col]) {
        case alive:
          // If a live cell has less than 2 or greater than 3 neighbors
          // it dies, else it remains alive in the next generation.
          if (n < 2 || n > 3)
            next_board.grid[row][col] = dead;
          break;
        case dead:
          // If a dead cell has exactly 3 neighbors it becomes alive in the
          // next generation, else it remains dead.
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
    msleep(250); // Sleep for 250 milliseconds.
    printf("\033[%dA\033[%dD", HEIGHT, WIDTH); // Clear the screen.
  }
  return 0;
}
