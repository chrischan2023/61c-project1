#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);
void getfile(FILE *fp, char *line, int *size);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
    return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
    state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
    // TODO: Implement this function.
    game_state_t *state = (game_state_t *) malloc(sizeof(game_state_t));
    state->x_size = 14;
    state->y_size = 10;
    state->num_snakes = 1;
    state->board = (char **) malloc(10 * sizeof(char *));
    for (int i = 0; i < state->y_size; i++) {
        state->board[i] = (char *) malloc((state->x_size + 1) * sizeof(char));
    }
    char *normal = "#            #";
    for (int i = 0; i < 10; i++) {
        if (i != 0 && i != 2 && i != 4 && i != 9) {
            strcpy(state->board[i], normal);
        }
    }
    strcpy(state->board[0], "##############");
    strcpy(state->board[2], "#        *   #");
    strcpy(state->board[4], "#   d>       #");
    strcpy(state->board[9], "##############");
    state->snakes = (snake_t *) malloc(sizeof(snake_t));
    state->snakes->tail_x = 4;
    state->snakes->tail_y = 4;
    state->snakes->head_x = 5;
    state->snakes->head_y = 4;
    state->snakes->live = true;
    return state;
}


/* Task 2 */
void free_state(game_state_t* state) {
    // TODO: Implement this function.
    free(state->snakes);
    state->snakes = NULL;
    int y_size = state->y_size;
    for (int i = 0; i < y_size; i++) {
        free(state->board[i]);
        state->board[i] = NULL;
    }
    free(state->board);
    state->board = NULL;
    free(state);
    state = NULL;
    return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
    // TODO: Implement this function.
    int y_size = state->y_size;
    for (int i = 0; i < y_size; i++) {
        fprintf(fp, "%s\n", state->board[i]);
    }
    return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
    FILE* f = fopen(filename, "w");
    print_board(state, f);
    fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
    // TODO: Implement this function.
    switch (c) {
        case 'w':
            return true;
        case 'a':
            return true;
        case 's':
            return true;
        case 'd':
            return true;
        default:
            return false;
    }
}

static bool is_snake(char c) {
    // TODO: Implement this function.
    if (is_tail(c)) {
        return true;
    } else {
        switch (c) {
            case '^':
                return true;
            case '<':
                return true;
            case '>':
                return true;
            case 'v':
                return true;
            case 'x':
                return true;
            default:
                return false;
        }
    }
}

static char body_to_tail(char c) {
    // TODO: Implement this function.
    switch (c) {
        case '^':
            return 'w';
        case '<':
            return 'a';
        case 'v':
            return 's';
        case '>':
            return 'd';
        default:
            exit(1);
    }
}

static int incr_x(char c) {
    // TODO: Implement this function.
   if (c == '>' || c == 'd') {
    return 1;
  } else if (c == '<' || c == 'a') {
    return -1;
  }
  
  return 0;
}

static int incr_y(char c) {
    // TODO: Implement this function.
    if (c == 'v' || c == 's') {
     return 1;
  } else if (c == '^' || c == 'w') {
    return -1;
  }

  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
    // TODO: Implement this function.
    snake_t *snake = (state->snakes + snum);
    int x = snake->head_x; 
    int y = snake->head_y; 
    char c = get_board_at(state, x, y);
    switch (c) {
        case '<':
            return get_board_at(state, x - 1, y);
        case '>':
            return get_board_at(state, x + 1, y);
        case '^':
            return get_board_at(state, x, y - 1);
        case 'v':
            return get_board_at(state, x, y + 1);
        default:
            exit(1);
    }
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
    // TODO: Implement this function.
    snake_t *snake = state->snakes + snum;
    char c = next_square(state, snum);
    char head = get_board_at(state, snake->head_x, snake->head_y);
    if (is_snake(c)) {
        set_board_at(state, snake->head_x, snake->head_y, 'x');
        snake->live = false;
        return;
    }
    switch (c) {
        case '#':
            set_board_at(state, snake->head_x, snake->head_y, 'x');
            snake->live = false;
            break;
        default:
        // update the snake head
            snake->head_x = snake->head_x + incr_x(head);
            snake->head_y = snake->head_y + incr_y(head);  
        // update the board
            set_board_at(state, snake->head_x, snake->head_y, head);
    }
    return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
    // TODO: Implement this function.
    snake_t *snake = state->snakes + snum;
    char tail = get_board_at(state, snake->tail_x, snake->tail_y);
    set_board_at(state, snake->tail_x, snake->tail_y, ' ');
    // update the snake tail
    snake->tail_x += incr_x(tail);
    snake->tail_y += incr_y(tail); 
   // update the board
    char body = get_board_at(state, snake->tail_x, snake->tail_y);
    set_board_at(state, snake->tail_x, snake->tail_y, body_to_tail(body));
    return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
    // TODO: Implement this function.
    int snum = 0;
    snake_t *snake = state->snakes;
    while (snum < state->num_snakes) {
        char c = next_square(state, snum);
        //if the next item is fruit, only update head
        if (c == '*') {
            update_head(state, snum);
        // generate a new fruit
            add_food(state);
        }
        else {
            update_head(state, snum);
            char s = get_board_at(state, snake->head_x, snake->head_y);
            //if this snake is dead, then there is no need to update tail
            if (s != 'x') {
                update_tail(state, snum);
            }
        }
        snum++;
        snake = (state->snakes + snum);
    }
    return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
    // TODO: Implement this function.
    FILE *fp = fopen(filename, "r");
    char line[1024];
    int size[2];
    getfile(fp, line, size);
    game_state_t *state = (game_state_t *) malloc(sizeof(game_state_t));
    state->x_size = size[0];
    state->y_size = size[1];
    state->board = (char **) malloc(state->y_size *sizeof(char *));
    for (int i = 0; i < state->y_size; i++) {
        state->board[i] = malloc((state->x_size + 1) * sizeof (char));
    }
    fclose(fp);
    fp = fopen(filename, "r");
    int i = 0;
    while (fgets(line, 1024, fp) != NULL) {
        strncpy(state->board[i], line, state->x_size);
        i++;
    }
    fclose(fp);
    return state;
}

void getfile(FILE *fp, char *line, int *size) {
    int c;
    int length = 0;
    int countlines = 0;
    for (int i = 0; (c = getc(fp)) != '\n'; i++) {
        length++;
    }
    countlines++;
    while ((c = getc(fp)) != EOF) {
        if (c == '\n') {
            countlines++;
        }
    }
    size[0] = length;
    size[1] = countlines;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
    // TODO: Implement this function.
    snake_t *snake = state->snakes + snum;
    int x = snake->tail_x, y = snake->tail_y;
    int prevx, prevy;
    char c = get_board_at(state, snake->tail_x, snake->tail_y);
    while (is_snake(c)) {
        prevx = x;
        prevy = y;
        x += incr_x(c);
        y += incr_y(c);
        //snake is dead
        if (x == prevx && y == prevy){
            break;
        }
        c = get_board_at(state, x, y);
    }
    snake->head_x = prevx;
    snake->head_y = prevy;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
    // TODO: Implement this function.
    state->num_snakes = 0;
    // count the number of snakes
    for (int i = 0; i < state->y_size; i++) {
        for (int j = 0; j < state->x_size; j++) {
            if (is_tail(get_board_at(state, j, i))) {
                state->num_snakes++;
            }
        }
    }
    // create the snakes
    state->snakes = (snake_t *) malloc(state->num_snakes * sizeof(snake_t));
    int snum = 0;
    for (int i = 0; i < state->y_size; i++) {
        for (int j = 0; j < state->x_size; j++) {
            if (is_tail(get_board_at(state, j, i))) {
                snake_t *snake = state->snakes + snum;
                snake->tail_x = j;
                snake->tail_y = i;
                find_head(state, snum);
            }
        }
    }
    return state;
}




