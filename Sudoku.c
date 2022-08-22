/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: [Junfei Zhang (Fiona), 1255069]
   Dated:     [8 September 2021]

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3        /* sudoku dimension, size of each inner square */
#define NDIG (NDIM*NDIM)
            /* total number of values in each row */
#define NGRP 3        /* number of sets each cell is a member of */
#define NSET (NGRP*NDIG)
            /* total number of sets in the sudoku */
#define NCLL (NDIG*NDIG)
            /* total number of cells in the sudoku */

#define ERROR    (-1)    /* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being completely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM*NDIM] = {
    /* the first group of nine sets describe the sudoku's rows */
    {  0,  1,  2,  3,  4,  5,  6,  7,  8 },
    {  9, 10, 11, 12, 13, 14, 15, 16, 17 },
    { 18, 19, 20, 21, 22, 23, 24, 25, 26 },
    { 27, 28, 29, 30, 31, 32, 33, 34, 35 },
    { 36, 37, 38, 39, 40, 41, 42, 43, 44 },
    { 45, 46, 47, 48, 49, 50, 51, 52, 53 },
    { 54, 55, 56, 57, 58, 59, 60, 61, 62 },
    { 63, 64, 65, 66, 67, 68, 69, 70, 71 },
    { 72, 73, 74, 75, 76, 77, 78, 79, 80 },
    /* the second group of nine sets describes the sudoku's columns */
    {  0,  9, 18, 27, 36, 45, 54, 63, 72 },
    {  1, 10, 19, 28, 37, 46, 55, 64, 73 },
    {  2, 11, 20, 29, 38, 47, 56, 65, 74 },
    {  3, 12, 21, 30, 39, 48, 57, 66, 75 },
    {  4, 13, 22, 31, 40, 49, 58, 67, 76 },
    {  5, 14, 23, 32, 41, 50, 59, 68, 77 },
    {  6, 15, 24, 33, 42, 51, 60, 69, 78 },
    {  7, 16, 25, 34, 43, 52, 61, 70, 79 },
    {  8, 17, 26, 35, 44, 53, 62, 71, 80 },
    /* the last group of nine sets describes the inner squares */
    {  0,  1,  2,  9, 10, 11, 18, 19, 20 },
    {  3,  4,  5, 12, 13, 14, 21, 22, 23 },
    {  6,  7,  8, 15, 16, 17, 24, 25, 26 },
    { 27, 28, 29, 36, 37, 38, 45, 46, 47 },
    { 30, 31, 32, 39, 40, 41, 48, 49, 50 },
    { 33, 34, 35, 42, 43, 44, 51, 52, 53 },
    { 54, 55, 56, 63, 64, 65, 72, 73, 74 },
    { 57, 58, 59, 66, 67, 68, 75, 76, 77 },
    { 60, 61, 62, 69, 70, 71, 78, 79, 80 },
};


/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void
fill_c2s() {
    int s=0, d=0, c;
    for ( ; s<NSET; s++) {
        /* record the first set number each cell is part of */
        for (c=0; c<NDIM*NDIM; c++) {
            c2s[s2c[s][c]][d] = s;
        }
        if ((s+1)%(NGRP*NDIM) == 0) {
            d++;
        }
    }
#if 0
    /* this code available here if you want to see the array
       cs2[][] that gets created, just change that 0 two lines back
       to a 1 and recompile */
    for (c=0; c<NCLL; c++) {
        printf("cell %2d: sets ", c);
        for (s=0; s<NGRP; s++) {
            printf("%3d", c2s[c][s]);
        }
        printf("\n");
    }
    printf("\n");
#endif
    return;
}

/* find the row number a cell is in, counting from 1
*/
int
rownum(int c) {
    return 1 + (c/(NDIM*NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int
colnum(int c) {
    return 1 + (c%(NDIM*NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int
sqrnum(int c) {
    return 1 + 3*(c/NSET) + (c/NDIM)%NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration
*/


/****************************defines*****************************/

#define NSUBSET 9    /* number of rows, column, squares */
#define LASTROW 8    /* last set number for rows */
#define LASTCOL 17    /* last set number for columns */
#define LASTSQR 26    /* last set number for squares */
#define NCLL_IN_SET ((NCLL * NGRP) / NSET)    /* number of cells in a set */
#define SUMMARYLINE "------+-------+------\n"
#define SEPARATION " | "
#define UNKNOWN 0
#define UNKN_SYMB "."
#define PRINT_SUCCESSFUL() printf("ta daa!!!\n")
#define PRINT_STRG_ONE() printf("strategy one\n")
#define YES 1    /* flag for 'yes' */
#define NO 0    /* flag for 'no' */
#define C2S_ROW 0    /* thses three are the order of c2s */
#define C2S_COL 1
#define C2S_SQR 2

/***********************function prototypes***********************/

void read_sudoku(int sudoku[], int dimension);
void print_sudoku(int sudoku[], int dimension);
int grid_check(int sudoku[]);
int strategy_one(int sudoku[]);
int updatable(int sudoku[], int must_num[]);
int check_set_vio(int sudoku[], int set_num);

/**************************main function**************************/

/* main program controls all the action
*/

int
main(int argc, char *argv[]) {
    int sudoku[NCLL] = {0};
    
    /* fill the c2s 2D array */
    fill_c2s();
    
    /* stage one: read and print the sudoku */
    read_sudoku(sudoku, NCLL);
    printf("\n");
    print_sudoku(sudoku, NCLL);
    printf("\n");
    
    /* stage two: check whether sudoku has violations; print the violations */
    if (grid_check(sudoku)) {
        
        /* do stage three if there is no violations: solve the sudoku */
        strategy_one(sudoku);
    }
    return 0;
}

/***********************stage one helper functions***********************/

/* read the sudoku into an 1D array, alternate the sudoku
*/

void
read_sudoku(int sudoku[], int dimension) {
    int cell_num, val;
    
    /* store sudoku into an 1D array */
    for (cell_num = 0; (cell_num < dimension)
        && (scanf("%d", &val) == 1); cell_num++) {
        sudoku[cell_num] = val;
    }
}


/* count and return the number of unknowns in the sudoku
*/

int
unknown(int sudoku[], int dimension) {
    int cell_num, num_unknown = 0;
    for (cell_num = 0; cell_num < dimension; cell_num++) {
        if (sudoku[cell_num] == UNKNOWN){
            num_unknown++;
        }
    }
    return num_unknown;
}


/* print the sudoku in required format, and print the number of unknowns
*/

void
print_sudoku(int sudoku[], int dimension) {
    int cell_num;
    for (cell_num = 0; cell_num < dimension; cell_num++) {
        
        /* print numbers */
        if (sudoku[cell_num] == UNKNOWN) {
            printf(UNKN_SYMB);
        }
        else {
            printf("%d", sudoku[cell_num]);
        }
        
        /* print format frame */
        if (colnum(cell_num) % NDIG != 0) {
            if ((colnum(cell_num) % NDIM == 0)) {
                printf(SEPARATION);
            }
            else {
                printf(" ");
            }
        }
        else {
            printf("\n");
        }
        if ((rownum(cell_num) % NDIM) == 0
            && (colnum(cell_num) == NSUBSET)
            && (rownum(cell_num) != NSUBSET)) {
            printf(SUMMARYLINE);
        }
    }
    
    /* print unknown message*/
    printf("\n");
    printf("%2d cells are unknown\n", unknown(sudoku, NCLL));
}

/***********************stage two helper functions***********************/

/* check for violations of sudoku, print the number of violations,
   return YES (1) if there's no violation and NO (0) if there are violations
*/

int
grid_check(int sudoku[]) {
    int set_num, vio_in_set, diff_sets = 0, violations = 0;
    
    /* check for violations in each set */
    for (set_num = 0; set_num < NSET; set_num++) {
        vio_in_set = check_set_vio(sudoku, set_num);
        if (vio_in_set != 0) {
            diff_sets++;
            violations += vio_in_set;
        }
    }
    
    /* print number of sets that have violations and total violations */
    if (violations) {
        printf("\n");
        printf("%d different sets have violations\n", diff_sets);
        printf("%d violations in total\n", violations);
        
        
    /* return a flag of whether the sudoku have violations*/
        return NO;
    }
    return YES;
}


/* check for violations in a set, print the violations,
   and return number of violations
*/

int
check_set_vio(int sudoku[], int set_num) {
    int cell_num, num, repeat, violations;
    violations = 0;
    
    /* check for violations of each number */
    for (num = 1; num <= 9; num++) {
        repeat = 0;
        for (cell_num = 0; cell_num < NCLL_IN_SET; cell_num++) {
            if (num == sudoku[s2c[set_num][cell_num]]) {
                repeat++;
            }
        }
        if (repeat > 1) {
            violations++;

            /* print violations */
            if (set_num <= LASTROW) {
                printf("set %2d (row %d): %d instances of %d\n", set_num,
                        rownum(s2c[set_num][0]), repeat, num);
            }
            else if ((LASTROW < set_num) && (set_num <= LASTCOL)) {
                printf("set %2d (col %d): %d instances of %d\n", set_num,
                        colnum(s2c[set_num][0]), repeat, num);
            }
            else {
                printf("set %2d (sqr %d): %d instances of %d\n", set_num,
                        sqrnum(s2c[set_num][0]), repeat, num);
            }
        }
    }
    return violations;
}

/***********************stage three helper functions***********************/

/* strategy one function: solving the sudoku by filling in the cells that have
   only one valid input possible; apply this strategy until no cells can be
   filled in
*/

int
strategy_one(int sudoku[]) {
    int cell_num;
    
    /* create an array for storing transformations of sudoku */
    int transformation[NCLL] = {0};
    
    /* print the sudoku and return if it is not updatable */
    if (!updatable(sudoku, transformation)) {
        print_sudoku(sudoku, NCLL);
        
        /* print TaDaa if sudoku is complete */
        if (unknown(sudoku, NCLL) == 0) {
            printf("\n");
            PRINT_SUCCESSFUL();
        }
        return 0;
    }
    
    /* if updatable, update sudoku according to transformation array */
    PRINT_STRG_ONE();
    for (cell_num = 0; cell_num < NCLL; cell_num++) {
        if (transformation[cell_num]) {
            sudoku[cell_num] = transformation[cell_num];
            printf("row %d col %d must be %d\n", rownum(cell_num),
                    colnum(cell_num), transformation[cell_num]);
        }
    }
    printf("\n");
    return strategy_one(sudoku);
}


/* check whether the sudoku is updatable, update the array of transformation
*/

int
updatable(int sudoku[], int transformation[]) {
    int cell_num, cll_in_set, num,
        row_set, col_set, sqr_set, row_cell, col_cell, sqr_cell,
        possible, have_same, must_num, updatable = NO;
    
    /* get the transformation array */
    for (cell_num = 0; cell_num < NCLL; cell_num++) {
            possible = 0;
            
            /* locate unknown cells in sudoku and sets containing it */
            if (sudoku[cell_num] == UNKNOWN) {
                row_set = c2s[cell_num][C2S_ROW];
                col_set = c2s[cell_num][C2S_COL];
                sqr_set = c2s[cell_num][C2S_SQR];
                                                                                    
                /* try each number, see if it is a possible input to the cell */
                for (num = 1; num <= 9; num++) {
                    have_same = NO;
                    for (cll_in_set = 0; cll_in_set < NCLL_IN_SET;
                        cll_in_set++) {
                        row_cell = s2c[row_set][cll_in_set];
                        col_cell = s2c[col_set][cll_in_set];
                        sqr_cell = s2c[sqr_set][cll_in_set];
                        if ((sudoku[row_cell] == num)
                            || (sudoku[col_cell] == num)
                            || (sudoku[sqr_cell] == num)) {
                            have_same = YES;
                            break;
                        }
                    }
                    if (!have_same) {
                        possible++;
                        must_num = num;
                        if (possible > 1) {
                            break;
                        }
                    }
                }
                if (possible == 1) {
                    transformation[cell_num] = must_num;
                    updatable = YES;
                }
            }
        }
        
    /* return flag telling if the sudoku is updatable */
    return updatable;
}

/* ALGORITHMS ARE FUN!!!!!!! I LOVE ALGORITHM !!!!!*/
/****************************************************************/
