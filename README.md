# Algorithm-designs

This repository contains two algorithmic designs for two games: sudoku and checkers.

Game Description:

Sudoku:

The goal of sudoku is to complete a square grid of 9×9 = 81 cells using only the numbers from 1 to 9, in a way that has exactly one instance of each digit appearing in each column, in each row, and in each 3 × 3 sub-square. 
This task is to develop a sudoku assistant, to help people solve their sudoku puzzles. It uses a one-dimensional C array to store a linearized version
of the sudoku matrix, and will write functions that carry out useful operations, including checking for numbering violations, identifying any “it must be that value” cells, and perhaps even solving some easy puzzles through to completion. Start by copying the skeleton program and sample data file that The University of Melbourne provides, and compile the program via gcc. The skeleton file provides an empty main program that includes a very important authorship declaration. The skeleton program also includes two very useful pre-filled “constant” matrices. 

Stage 1 – Reading and Printing:
The input to the program will always be a set of 81 one-digit numbers, laid out as a sudoku grid, and with “0” used to represent empty cells. The input is read using a sequence of 81 calls (in a loop, of course) to scanf("%d", &val), to avoid the need to deal with blank and newline characters.

Stage 2 - Grid Checking:
The program checks the input grid for mistakes, to ensure that none of the 27 sets in the supplied puzzle have duplicate elements. Report any errors according to their set number, from 0 to 26, and also by the set type (“row”, “col”, or “sqr”) and the ordinal (counting from one) number of that set.

Stage 3 – Implementing Strategy One:
In Strategy One, each cell c should be considered in turn. The three sets that cell is a member of (found using array c2s[c]) should be examined, accumulating label counts from the (up to) 24 labeled cells in those three sets (found using array s2c[c]). Each of those labels is a number between 1 and 9. Hence, if there is only one of those nine possible numbers that does not yet occur anywhere in c’s three sets, then it must be the label for cell c.


Checkers:

Checkers, or draughts, is a strategy board game played by two players. There are many variants of checkers. For a guide to checker’s families and rules, see https://www.fmjd.org/downloads/Checkers_families_ and_rules.pdf. This task is to implement a program that reads, prints, and plays our variant of the game.

Setup. An 8x8 chessboard with 12 black and 12 white pieces.
Gameplay. Each player plays all pieces of the same color. Black open the game by making a move, then white make a move, and then players alternate their turns. In a single turn, the player either makes a move or capture. For example, the arrow in Figure 1a denotes an opening move of the black piece from cell G6 to cell F5.
Moves. A piece may move to an empty cell diagonally forward (toward the opponent; north for black and south for white) one square. 
Towers. When a piece reaches the furthest row (the top row for black and the bottom row for white), it becomes a tower (a pile of pieces). The only move of the white piece at cell D7 in Figure 1b promotes it to the tower. A tower may move to an empty cell diagonally, both forward and backward, one square. The arrows in Figure 1c show all the legal moves of black and white towers.
Captures. To capture a piece or a tower of the opponent, a piece or a tower of the player jumps over it and lands in a straight diagonal line on the other side. This landing cell must be empty. When a piece or tower is captured, it is removed from the board. Only one piece or tower may be captured in a single jump, and, in our variant of the game, only one jump is allowed in a single turn. Hence, if another capture is available after the first jump, it cannot be taken in this turn. Also, in our variant of the game, if a player can make a move or a capture, they may decide which of the two to complete. A piece always jumps forward (toward the opponent), while a tower can jump forward and backward. The arrows in Figure 1d show all the legal captures for both players.
Game end. A player wins the game if it is the opponent’s turn and they cannot take action, move or capture, either because no their pieces and towers are left on the board or because no legal move or capture is possible.

Stage 0 – Reading, Analyzing, and Printing Input Data: 
The first version of the program should read input and print the initial setup and all legal actions.

Stage 1 – Compute and Print Next Action:
If the ‘A’ command follows the input actions, the program should compute and print the information about the next Action of the player with the turn. All of the Stage 0 output should be retained. To compute the next action, the program should implement the minimax decision rule for the tree depth of three. 

First, the tree of all reachable board configurations starting from the current configuration (level 0) and of the requested depth is constructed. Second, the cost of all leaf boards is computed. Third, for each possible action of the player, we check all possible actions of the opponent, and choose the next action of the player to be the first action on the path from the root of the tree toward a leaf node for which the player maximizes their gain while the opponent (considered rational) aims to minimize the player’s gain. If the next action does not exist and, thus, cannot be computed, the player loses, and the corresponding message is printed on a new line (“BLACK WIN!” or “WHITE WIN!”).

Stage 2 – Machines Game:
If the ‘P’ command follows the input actions, the program should Play ten next actions or all actions until the end of the game, whatever comes first.



