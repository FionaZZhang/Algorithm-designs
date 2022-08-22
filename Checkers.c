/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [Junfei Zhang 1255069]
  Dated:     [30 Sep 2021]

*/
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

/* Constants used in this program */
#define ASCII_ONE           49
#define ASCII_START         65
#define WHITE_WIN           -1
#define BLACK_WIN           1
#define INITIAL             0
#define FST_DPT             0
#define SND_DPT             1
#define TRD_DPT             2
#define YES                 1
#define NO                  0

/* Printing format */
#define FIRST_LINE_CHECKER  "A   B   C   D   E   F   G   H"
#define SUMMARY_LINE        "+---+---+---+---+---+---+---+---+"
#define DELIMITER           "====================================="
#define SEPARATION          "|"

/* Input format */
#define INI_COL             0
#define INI_ROW             1
#define FIN_COL             3
#define FIN_ROW             4
#define MOVE_DIRECTION      4
#define ACTION_COMMAND      'A'
#define PLAY_COMMAND        'P'

/* Defines regarding settings */
#define INITIAL_PIECES      BOARD_SIZE / 2 * ROWS_WITH_PIECES
#define ACTION_COUNT_START  1
#define PLAY_TIMES          10
#define USE_BLACK_GRID      YES
#define CSTART              1     // Column start
#define RSTART              1     // Row start
#define W_TOWER_ROW         7
#define B_TOWER_ROW         0
#define CAPTURE_STEP        2
#define NON_CAPTURE_STEP    1
#define FIRST_BLACK         YES

/* Orientation definition of board */
#define NORTH               -1     // Row direction of north
#define EAST                +1     // Column direction of east
#define SOUTH               +1     // Row direction of east
#define WEST                -1     // Column direction of east

/* Action array format */
#define ROW_FROM            0
#define COL_FROM            1
#define ROW_TO              2
#define COL_TO              3
#define NE                  0      // North East
#define SE                  1      // South East
#define SW                  2      // South West
#define NW                  3      // North West

/* Error codes */
#define SOURCE_OUTSIDE      -1
#define TARGET_OUTSIDE      -2
#define SOURCE_EMPTY        -3
#define TARGET_NOT_EMPTY    -4
#define HOLD_OPPO           -5


/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

/* Structure of a node board */
typedef struct {
    int action[MOVE_DIRECTION];
    board_t checker;
    int cost;
    int link;
}node_t;


/* function prototypes -------------------------------------------------------*/
/* Stage zero */
int initial_setup(board_t checker);
int stage_zero (board_t checker, int row1, int row2, int col1, int col2,
                int *action_count);

/* Stage one */
int get_node(board_t checker, node_t* tree, int row, int col, int black_action);
int generate_tree(board_t checker, node_t **total_tree, int* depth_count,
                  int black_action);
int stage_one(board_t checker, int* action_count, int black_action);

/* Major helper functions*/
int print_checker(board_t checker);
int board_cost(board_t checker);
char read_input(int* col1, int* row1, int* col2, int* row2);
int checker_move(board_t checker, int row1, int col1, int row2, int col2,
                 int black_action);
int all_legal_actions(board_t checker, int* action_arr, int row, int col,
                      int black_action);
int check_error(board_t checker, int col1, int row1, int col2, int row2,
               int black_action);
int check_legal(board_t checker, int row1, int col1, int row2, int col2,
                int black_action);
int print_error(int error_code, int legal_code);
int win_status (board_t checker);

/* Other helper functions */
int is_capture(int row1, int row2);
char colour(board_t checker, int row1, int col1);
int is_tower(board_t checker, int row, int col);
int black_turn(int action_count);
int is_odd(int num);
int letter_to_col(char letter);
char col_to_letter(int col);


/* main Function -------------------------------------------------------------*/
int
main(int argc, char *argv[]) {
    board_t checker;
    int action_count = ACTION_COUNT_START;
    int col1 = 0, row1 = 0, col2 = 0, row2 = 0, count = 0;
    char read;

    /* Stage Zero */
    initial_setup(checker);
    while ((read = read_input(&col1, &row1, &col2, &row2))) {
        if (read != (char) YES) {
            break;
        }
        stage_zero(checker, row1, row2, col1, col2, &action_count);
    }

    /* Stage One */
    if (read == ACTION_COMMAND) {
        stage_one(checker, &action_count, black_turn(action_count));
        return EXIT_SUCCESS;
    }

    /* Stage Two */
    if (read == PLAY_COMMAND) {
        for (count = 0; count < PLAY_TIMES; count++) {
            stage_one(checker, &action_count, black_turn(action_count));
        }
        return EXIT_SUCCESS;
    }
    
    return EXIT_SUCCESS;
}

/* Stage 0 Functions ---------------------------------------------------------*/

/* Put black and white pieces to required initialisation,
   print the starting checker.
*/
int
initial_setup(board_t checker) {
    int row, col;
    /* Initialize the board with 'empty' */
    for (row = 0; row < BOARD_SIZE; row++) {
        for (col = 0; col < BOARD_SIZE; col++) {
            checker[row][col] = CELL_EMPTY;
        }
    }

    /* Put black and white pieces to required cells */
    for (row = 0; row < BOARD_SIZE; row++) {
        for (col=0; col<BOARD_SIZE; col++) {
            if (is_odd(row + col) == USE_BLACK_GRID) {
                if (row < ROWS_WITH_PIECES) {
                    checker[row][col] = CELL_WPIECE;
                }
                else if (row >= BOARD_SIZE - ROWS_WITH_PIECES) {
                    checker[row][col] = CELL_BPIECE;
                }
            }
        }
    }
    
    /* Print the initial checker*/
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", INITIAL_PIECES);
    printf("#WHITE PIECES: %d\n", INITIAL_PIECES);
    print_checker(checker);
    return EXIT_SUCCESS;
}


/* Implement stage zero.
*/
int
stage_zero (board_t checker, int row1, int row2, int col1, int col2,
            int *action_count) {
    int error_code, legal_code, black_action = black_turn(*action_count);

    /* Check for error */
    error_code = check_error(checker, row1, col1, row2, col2, black_action);
    legal_code = check_legal(checker, row1, col1, row2, col2, black_action);
    if (print_error(error_code, legal_code) == NO) {
        exit(EXIT_SUCCESS);
    }
    
    /* Move the checker according to input */
    checker_move(checker, row1, col1, row2, col2, black_action);
    
    /* Print the new checker board */
    if (black_action) {
        printf(DELIMITER"\n");
        printf("BLACK ACTION #%d: %c%d-%c%d\n", *action_count,
               col_to_letter(col1) + 1, row1 + 1, col_to_letter(col2) + 1,
               row2+1);
        printf("BOARD COST: %d\n", board_cost(checker));
        print_checker(checker);
    }
    else {
        printf(DELIMITER"\n");
        printf("WHITE ACTION #%d: %c%d-%c%d\n", *action_count,
               col_to_letter(col1) + 1, row1 + 1, col_to_letter(col2) + 1,
               row2 + 1);
        printf("BOARD COST: %d\n", board_cost(checker));
        print_checker(checker);
    }
    (*action_count)++;
    return EXIT_SUCCESS;
}


/* Stage 1 Functions ---------------------------------------------------------*/

/* Generate all nodes of next depth for the corresponding cell of
   the current board and store in an 1D array of structs. Return the number of
   nodes.
*/
int
get_node(board_t checker, node_t* tree, int row, int col, int black_action) {
    int count, i, action_arr[MOVE_DIRECTION] = {INITIAL}, row_copy, col_copy,
        newrow, newcol;
    board_t checker_copy;
    node_t node;
    node_t *rea_loc;
    
    /* Copy the checker */
    for (row_copy = 0; row_copy < BOARD_SIZE; row_copy++) {
        for (col_copy = 0; col_copy < BOARD_SIZE; col_copy++) {
            checker_copy[row_copy][col_copy] = checker[row_copy][col_copy];
        }
    }
    
    /* Store info in node */
    count = 0;
    if (((black_action && (colour(checker, row, col) == CELL_BPIECE))
        || (!black_action && (colour(checker, row, col) == CELL_WPIECE)))
        && (all_legal_actions(checker_copy, action_arr, row, col,
        black_action))) {
        rea_loc = (node_t*) realloc(tree, MOVE_DIRECTION * sizeof(node_t));
        assert(rea_loc != NULL);
        tree = rea_loc;
        for (i = 0; i < MOVE_DIRECTION; i++) {
            if (action_arr[i] != 0) {
                
                /* For each action, copy the starting checker board */
                for (row_copy = 0; row_copy < BOARD_SIZE; row_copy++) {
                    for (col_copy = 0; col_copy < BOARD_SIZE; col_copy++) {
                        checker_copy[row_copy][col_copy]
                            = checker[row_copy][col_copy];
                    }
                }
                
                /* Get new cell according to definition of action array */
                if (i < SW) {
                    newcol = col + action_arr[i];
                    if (i == NE) {
                        newrow = row-action_arr[i];
                    }
                    else {
                        newrow = row+action_arr[i];
                    }
                }
                else {
                    newcol = col - action_arr[i];
                    if (i == SW) {
                        newrow = row+action_arr[i];
                    }
                    else {
                        newrow = row-action_arr[i];
                    }
                }
                
                /* Store action in the node */
                node.action[ROW_FROM] = row;
                node.action[COL_FROM] = col;
                node.action[ROW_TO] = newrow;
                node.action[COL_TO] = newcol;


                /* Generate a new board according to the action */
                checker_move(checker_copy, row, col, newrow, newcol,
                             black_action);
                for (row_copy = 0; row_copy < BOARD_SIZE; row_copy++) {
                    for (col_copy = 0; col_copy < BOARD_SIZE; col_copy++) {
                        node.checker[row_copy][col_copy]
                            = checker_copy[row_copy][col_copy];
                    }
                }

                /* Get board cost of the new board */
                node.cost = board_cost(checker_copy);
                
                /* Store the node in the array */
                tree[count] = node;
                count++;
            }
        }
    }
    return count;
}


/* Generate a three-depth action tree for the current checker status
   in type of a 2D array of structs.
*/
int
generate_tree(board_t checker, node_t **total_tree, int* depth_count,
              int black_action) {
    int row, col, count, i, j, k, depth;
    board_t checker_copy;
    node_t node;
    node_t *rea_loc, *tree = (node_t*) malloc(sizeof(node_t));
    assert(tree != NULL);

    /* Generate first depth of the tree */
    depth_count[FST_DPT] = 0;
    for (row = 0; row < BOARD_SIZE; row++) {
        for (col = 0; col < BOARD_SIZE; col++) {
            count = get_node(checker, tree, row, col, black_action);
            if (count != 0) {
                i = depth_count[FST_DPT];
                rea_loc = (node_t*) realloc(total_tree[FST_DPT],
                                            (i + count) * sizeof(node_t));
                assert(rea_loc != NULL);
                total_tree[FST_DPT] = rea_loc;
                for (j = 0; j < count; j++) {
                    total_tree[FST_DPT][i+j] = tree[j];
                }
                depth_count[FST_DPT] += count;
            }
        }
    }
    
    /* There is a winner if there are no first depth nodes */
    if (depth_count[FST_DPT] == 0) {
        if (black_action) {
            printf("WHITE WIN!\n");
        } else {
            printf("BLACK WIN!\n");
        }
        free(tree);
        tree = NULL;
        for (i = 0; i < TREE_DEPTH; i++) {
            free(total_tree[i]);
            total_tree[i] = NULL;
        }
        free(total_tree);
        total_tree = NULL;
        exit(EXIT_SUCCESS);
    }
    
    /* Generate the rest depths of the tree */
    for (depth = SND_DPT; depth < TREE_DEPTH; depth++) {
        black_action = !black_action;
        depth_count[depth] = 0;
        for (i = 0; i < depth_count[depth - 1]; i++) {
            
            /* Copy the checker of the node board from last depth of tree */
            node = total_tree[depth - 1][i];
            for (row = 0; row < BOARD_SIZE; row++) {
                for (col = 0; col < BOARD_SIZE; col++) {
                    checker_copy[row][col] = node.checker[row][col];
                }
            }
            
            /* Store nodes in the tree, store index of last depth as link */
            for (row = 0; row < BOARD_SIZE; row++) {
                for (col = 0; col < BOARD_SIZE; col++) {
                    count = get_node(checker_copy, tree, row, col,
                                     black_action);
                    if (count != 0) {
                        k = depth_count[depth];
                        rea_loc = (node_t*) realloc(total_tree[depth],
                                                    (k + count)*sizeof(node_t));
                        assert(rea_loc != NULL);
                        total_tree[depth] = rea_loc;
                        for (j = 0; j < count; j++) {
                            total_tree[depth][k + j] = tree[j];
                            total_tree[depth][k + j].link = i;
                        }
                        depth_count[depth] += count;
                    }
                    else {
                        if (black_action) {
                            total_tree[depth - 1][i].cost = INT_MAX;
                        }
                        else {
                            total_tree[depth - 1][i].cost = INT_MIN;
                        }
                    }
                }
            }
        }
    }
    free(tree);
    tree = NULL;
    return EXIT_SUCCESS;
}

/* Implement stage one */
int
stage_one(board_t checker, int* action_count, int black_action) {
    
    int i, j, match, depth, final_cost, final_index, row1, col1, row2, col2;
    node_t node, final_node;
    node_t **total_tree = (node_t**) malloc(TREE_DEPTH*sizeof(node_t*));
    assert(total_tree != NULL);
    int depth_count[TREE_DEPTH];
    
    /* Allocate memory for tree */
    for (depth = 0; depth < TREE_DEPTH; depth++) {
        total_tree[depth] = (node_t*) malloc(sizeof(node_t));
        assert(total_tree[depth] != NULL);
    }
    
    /* Generate the tree array */
    generate_tree(checker, total_tree, depth_count, black_action);

    /* Find required cost for depth one and two nodes according to
       depth three leaves; depth is traversed backwards */
    for (depth = SND_DPT; depth >= FST_DPT ; depth--) {
        for (i = 0; i < depth_count[depth]; i++) {
            match = 0;
            for (j = 0; j < depth_count[depth + 1]; j++) {
                node = total_tree[depth + 1][j];
                
                /* Find linking deeper node */
                if (node.link == i) {
                    if (match == 0) {
                        total_tree[depth][i].cost = node.cost;
                    }
                    
                    /* Minmax decision rule */
                    if (is_odd(depth)) {
                        if ((black_action && (node.cost >
                            total_tree[depth][i].cost)) || ((!black_action)
                            && (node.cost < total_tree[depth][i].cost))) {
                            total_tree[depth][i].cost = node.cost;
                        }
                    }
                    else {
                        if ((black_action && (node.cost <
                            total_tree[depth][i].cost)) || ((!black_action)
                            && (node.cost > total_tree[depth][i].cost))) {
                            total_tree[depth][i].cost = node.cost;
                        }
                    }
                    match++;
                }
            }
        }
    }
    
    /* Get final move according to cost of depth one nodes*/
    match = 0;
    final_index = 0;
    for (i = 0; i < depth_count[FST_DPT]; i++) {
        node = total_tree[FST_DPT][i];
        if (match == 0) {
            final_cost = node.cost;
            final_index = i;
        }
        
        /* Minmax decision rule*/
        if ((black_action && (node.cost>final_cost))
           || ((!black_action) && (node.cost < final_cost))) {
            final_cost = node.cost;
            final_index = i;
        }
        match++;
    }
    
    /* Get corresponding move */
    final_node = total_tree[FST_DPT][final_index];
    row1 = final_node.action[ROW_FROM];
    col1 = final_node.action[COL_FROM];
    row2 = final_node.action[ROW_TO];
    col2 = final_node.action[COL_TO];
    
    /* Update the checker board */
    checker_move(checker, row1, col1, row2, col2, black_action);

    /* Print corresponding action */
    if (black_action) {
        printf(DELIMITER"\n");
        printf("*** BLACK ACTION #%d: %c%d-%c%d\n", *action_count,
               col_to_letter(col1 + CSTART), row1 + RSTART,
               col_to_letter(col2 + CSTART), row2 + RSTART);
        printf("BOARD COST: %d\n", board_cost(checker));
        print_checker(checker);
    }
    else {
        printf(DELIMITER"\n");
        printf("*** WHITE ACTION #%d: %c%d-%c%d\n", *action_count,
               col_to_letter(col1 + CSTART), row1 + RSTART,
               col_to_letter(col2 + CSTART), row2 + RSTART);
        printf("BOARD COST: %d\n", board_cost(checker));
        print_checker(checker);
    }
    (*action_count)++;
    
    /* Free allocated memory */
    for (i = 0; i < TREE_DEPTH; i++) {
        free(total_tree[i]);
        total_tree[i] = NULL;
    }
    free(total_tree);
    total_tree = NULL;
    
    return EXIT_SUCCESS;
}


/* Major Helper Functions ----------------------------------------------------*/

/* Print the checker in required format.
*/
int
print_checker(board_t checker) {
    int row, row_num, col;
    printf("     "FIRST_LINE_CHECKER"\n");
    printf("   "SUMMARY_LINE"\n");
    for (row = 0; row < BOARD_SIZE; row++) {
        row_num = row + RSTART;
        printf(" %d ", row_num);
        for (col = 0; col < BOARD_SIZE; col++) {
            printf(SEPARATION" %c ", checker[row][col]);
        }
        printf(SEPARATION"\n");
        printf("   "SUMMARY_LINE"\n");
    }
    return EXIT_SUCCESS;
}


/* Compute and return the board cost of the current status of the checker.
*/
int
board_cost(board_t checker) {
    int row, col, w_num = 0, W_num = 0, b_num = 0, B_num = 0;
    char piece;
    if (win_status(checker) == NO) {
        for (row=0; row < BOARD_SIZE; row++) {
            for (col = 0; col < BOARD_SIZE; col++) {
                piece = checker[row][col];
                if (piece == CELL_WPIECE) {
                    w_num++;
                }
                else if (piece == CELL_WTOWER) {
                    W_num++;
                }
                else if (piece == CELL_BPIECE) {
                    b_num++;
                }
                else if (piece == CELL_BTOWER) {
                    B_num++;
                }
            }
        }
        return COST_PIECE * (b_num - w_num) + COST_TOWER * (B_num - W_num);
    }
    
    /* Change the cost to INT_MAX or INT_MIN if ther is a winner */
    else if (win_status(checker) == BLACK_WIN) {
        return INT_MAX;
    }
    else {
        return INT_MIN;
    }
}


/* Read the input action.
*/
char
read_input(int* col1, int* row1, int* col2, int* row2) {
    char command;
    char *move = (char*) malloc(6);
    assert(move != NULL);
    scanf("%s", move);
    
    /* Input is a command rather than a move */
    if (move[INI_ROW] == '\0') {
        command = move[INI_COL];
        free(move);
        move = NULL;
        return command;
    }
    
    /* Input is a move */
    else {
        *col1 = letter_to_col(move[INI_COL]) - CSTART;
        *row1 = (int) move[INI_ROW] - ASCII_ONE;
        *col2 = letter_to_col(move[FIN_COL]) - CSTART;
        *row2 = (int) move[FIN_ROW] - ASCII_ONE;
    }
    free(move);
    move = NULL;
    return (char) YES;
}


/* Alternate the board according to a specific action.
*/
int
checker_move(board_t checker, int row1, int col1, int row2, int col2,
             int black_action) {
    
    /* Return 0 if the move is illegal */
    if (!((check_error(checker, row1, col1, row2, col2, black_action)==YES)
        && check_legal(checker, row1, col1, row2, col2, black_action))) {
        return NO;
    }
    
    /* Move the checker */
    checker[row2][col2] = checker[row1][col1];
    checker[row1][col1] = CELL_EMPTY;
    if (is_capture(row1, row2)) {
        checker[(row1+row2) / CAPTURE_STEP][(col1+col2)
                                            / CAPTURE_STEP] = CELL_EMPTY;
    }

    if (black_action && (row2 == B_TOWER_ROW)) {
        checker[row2][col2] = CELL_BTOWER;
    }
    
    else if ((!black_action) && (row2 == W_TOWER_ROW)) {
        checker[row2][col2] = CELL_WTOWER;
    }
    return YES;

}


/* Generate all legal actions of a piece.
*/
int
all_legal_actions(board_t checker, int* action_arr, int row, int col,
                  int black_action) {
    int action_num, rowplus, colplus, row_count, col_count,
        new_row, new_col, can_move = NO;

    /* Check for each possible move; if it is legal, update the action array.
    */
    action_num = 0;
    colplus = EAST;
    for (col_count = 0; col_count < CAPTURE_STEP; col_count++) {
        
        if (col_count == 0) {
            rowplus = NORTH;
        }
        else {
            rowplus = SOUTH;
        }
        for (row_count = 0; row_count < CAPTURE_STEP; row_count++) {
            new_row = row + rowplus;
            new_col = col + colplus;
            
            /* Try if the non-capture move in the direction is legal */
            if ((check_error(checker, row, col, new_row, new_col, black_action))
                ==YES) {
                action_arr[action_num] = NON_CAPTURE_STEP;
                can_move = YES;
            }
            
            /* Try if the capture move in the direction is legal */
            else {
                new_row += rowplus;
                new_col += colplus;
                if ((check_error(checker, row, col, new_row, new_col,
                     black_action) == YES)
                    && (((black_action && (colour(checker, row + rowplus,
                          col+colplus) == CELL_WPIECE)))
                        || (!black_action && (colour(checker, row+rowplus,
                            col + colplus) == CELL_BPIECE)))) {
                    action_arr[action_num] = CAPTURE_STEP;
                    can_move = YES;
                }
            }
            rowplus *= -1; // Reverse the row direction
            action_num++;
        }
        colplus*=-1; // Reverse the column direction
    }
    
    /* Non-tower pieces cannot mobe backwards */
    if (!is_tower(checker, row, col)) {
        if (black_action) {
            action_arr[SE] = 0;
            action_arr[SW] = 0;
        }
        else if (!black_action){
            action_arr[NE] = 0;
            action_arr[NW] = 0;
        }
    }
    return can_move;
}


/* Check if an action has error, return type of error.
*/
int
check_error(board_t checker, int row1, int col1, int row2, int col2,
           int black_action) {
    
    /* Source cell is outside of the board */
    if ((col1 < 0) || (col1 > BOARD_SIZE - CSTART) || (row1 < 0)
        || (row1 > BOARD_SIZE - RSTART) ) {
        return SOURCE_OUTSIDE;
    }
    
    /* Target cell is outside of the board */
    if ((col2 < 0) || (col2 > BOARD_SIZE - CSTART) || (row2 < 0)
        || (row2 > BOARD_SIZE - RSTART) ) {
        return TARGET_OUTSIDE;
    }
    
    /* Source cell is empty */
    if (checker[row1][col1] == CELL_EMPTY) {
        return SOURCE_EMPTY;
    }
    
    /* Target cell is not empty */
    if (checker[row2][col2] != CELL_EMPTY) {
        return TARGET_NOT_EMPTY;
    }
    
    /* Source cell holds opponent’s piece/tower */
    if (((black_action==YES) && (colour(checker, row1, col1) != CELL_BPIECE))
        || ((black_action==NO) && (colour(checker, row1, col1) != CELL_WPIECE))
       ){
        return HOLD_OPPO;
    }
    
    return YES;
}


/* Check if an action is legal.
*/
int
check_legal(board_t checker, int row1, int col1, int row2, int col2,
            int black_action) {
    int count, action_arr[MOVE_DIRECTION] = {INITIAL};
    all_legal_actions(checker, action_arr, row1, col1, black_action);
    for (count = 0; count < MOVE_DIRECTION; count++) {
        if (((row1+action_arr[count]==row2)&&(col1+action_arr[count]==col2))
           || ((row1-action_arr[count]==row2)&&(col1-action_arr[count]==col2))
           || ((row1+action_arr[count]==row2)&&(col1-action_arr[count]==col2))
           || ((row1-action_arr[count]==row2)&&(col1+action_arr[count]==col2))
           ) {
            return YES;
        }
    }
    return NO;
    
}


/* Print error message.
*/
int
print_error(int error_code, int legal_code) {
    if (error_code == SOURCE_OUTSIDE) {
        printf("ERROR: Source cell is outside of the board.\n");
        return NO;
    }
    if (error_code == TARGET_OUTSIDE) {
        printf("ERROR: Target cell is outside of the board.\n");
        return NO;
    }
    if (error_code == SOURCE_EMPTY) {
        printf("ERROR: Source cell is empty.\n");
        return NO;
    }
    if (error_code == TARGET_NOT_EMPTY) {
        printf("ERROR: Target cell is not empty.\n");
        return NO;
    }
    if (error_code == HOLD_OPPO) {
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        return NO;
    }
    if (legal_code == NO) {
        printf("ERROR: Illegal action.\n");
        return NO;
    }
    return YES;
}

/* Check if there is a winer of the current board.
*/
int
win_status (board_t checker) {
    int row, col, black_action = YES;
    int action_arr[MOVE_DIRECTION];
    int white_win = YES, black_win = YES;
    for (row = 0; row < BOARD_SIZE; row++) {
        for (col = 0; col < BOARD_SIZE; col++) {
            if (all_legal_actions(checker, action_arr, row, col, black_action)
                && (colour(checker, row, col) == CELL_BPIECE)) {
                white_win = NO;
            }
            else if (all_legal_actions(checker, action_arr, row, col,
                    !black_action) && (colour(checker, row, col)
                    == CELL_WPIECE)) {
                black_win = NO;
            }
        }
    }
    if ((!white_win) && (!black_win)) {
        return NO;
    }
    if (white_win == YES) {
        return WHITE_WIN;
    }
    else {
        return BLACK_WIN;
    }
}


/* Other Helper Functions ----------------------------------------------------*/

/* Check if a legal action is a capture move.
*/
int
is_capture(int row1, int row2) {
    if (abs(row1 - row2) != NON_CAPTURE_STEP) {
        return YES;
    }
    return NO;
}


/* Check if the colour of the given cell contains a black or white piece,
   or is empty cell. Return the corresponding symbol.
*/
char
colour(board_t checker, int row, int col) {
    if (checker[row][col] == CELL_BPIECE || checker[row][col] == CELL_BTOWER) {
        return CELL_BPIECE;
    }
    else if (checker[row][col] == CELL_WPIECE
            || checker[row][col] == CELL_WTOWER) {
        return CELL_WPIECE;
    }
    else {
        return CELL_EMPTY;
    }
}


/* Check if a cell is a tower.
*/
int
is_tower(board_t checker, int row, int col) {
    if ((checker[row][col] == CELL_WTOWER)
        || (checker[row][col] == CELL_BTOWER)) {
        return YES;
    }
    return NO;
}


/* Check if the current action is black's action.
*/
int
black_turn(int action_count) {
    if (!is_odd(action_count)) {
        return !FIRST_BLACK;
    }
    return FIRST_BLACK;
}


/* Check if the given number is odd
*/
int
is_odd(int num) {
    if (num % 2 == 0) {
        return NO;
    }
    return YES;
}


/* Convert the letters regarding to checker board to column numbers.
*/
int
letter_to_col(char letter) {
    int col;
    col = (int) letter + CSTART - ASCII_START;
    return col;
}


/* Convert the column numbers to column letters.
*/
char
col_to_letter(int col) {
    char letter;
    letter = (char) (col - CSTART + ASCII_START);
    return letter;
}

/* Algorithms are fun! */
/* THE END -------------------------------------------------------------------*/
