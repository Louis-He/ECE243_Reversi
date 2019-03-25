#include <stdio.h>
#include <stdbool.h>
//print board
void printBoard(char board[][26], int n);

//checks whether the specified (row, col) lies within the board dimensions
bool isAvaliableMove(char board[][26], int n, char player);
void chessMove(char* gridPoint, char player);
void newChessMove(char board[][26], int n, int row, int col, char player);
void updateChessInDirection(char board[][26], int n, char row, char col, char player, int deltaRow, int deltaCol);
bool positionInBounds(int n, char row, char col);
bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol);
bool checkMove(char board[][26], int n, int row, int col, char player);
void printAvaliableMove(char board[][26], int n, char player);
void AIMove(char board[][26], int n, char color);
int getScore(char board[][26], int n, int row, int col, char color);
int getFinalScore(char board[][26], int n, char player);
int flipInDirection(char board[][26], int n, char row, char col, char color, int deltaRow, int deltaCol);

// basic helper functions
int letterToNumber(char letter);
char numberToLetter(int number);

int main(int argc, char **argv){
    char winner = 'U';
	int dimension, row, col;
    char userInput[2];
	char computer; 
    int rowenter, colenter;
    char movecolor = 'B';

	// intialize the board
	printf("Enter the board dimension: ");
	scanf("%d", &dimension);
	char board[26][26];
	for(row = 0; row < dimension; row ++){
		for(col = 0; col < dimension; col ++){
			if (row == dimension / 2 - 1){
				if (col == dimension / 2 - 1){
					board[row][col] = 'W';
				}else if (col == dimension / 2){
					board[row][col] = 'B';
				}else{
          board[row][col] = 'U';
        }
			}else if (row == dimension / 2){
				if (col == dimension / 2 - 1){
					board[row][col] = 'B';
				}else if (col == dimension / 2){
					board[row][col] = 'W';
				}else{
          board[row][col] = 'U';
        }
			}else{
				board[row][col] = 'U';
			}
		}
	}
    
    // request for the color of AI
    printf("Computer plays (B/W) : ");
	scanf(" %c", &computer);
    
    // print board before the game starts
    printBoard(board, dimension);
	
    // start game
    
    // Enter the loop. If no avalible move then exit
    while(winner == 'U' && (isAvaliableMove(board, dimension, 'W') || isAvaliableMove(board, dimension, 'B'))){
        
        // if avaliablemove for movecolor
        if (isAvaliableMove(board, dimension, movecolor)){
            if (computer == movecolor){
                AIMove(board, dimension, movecolor);
            }else{
                // Request enter a move
                printf("Enter move for colour %c (RowCol): ", movecolor);
                scanf("%s", userInput);
                // analyze the input of user
                rowenter = letterToNumber(userInput[0]);
                colenter = letterToNumber(userInput[1]);
                // legal than continue, else exit
                if(checkMove(board, dimension, rowenter, colenter, movecolor)){
                    newChessMove(board, dimension, rowenter, colenter, movecolor);
                }else{
                    printf("Invalid move.\n");
                    winner = computer;
                }
            }
            // print board if board changed
            if (winner == 'U'){
                printBoard(board, dimension);
            }
            
        } else {
            printf("%c player has no valid move.\n", movecolor);
        }
        
        // change movecolor every time
        if (movecolor == 'B'){
            movecolor = 'W';
        }else{
            movecolor = 'B';
        }
    }
    
    if (winner == 'U'){
        int score1 = getFinalScore(board, dimension, 'B');
        int score2 = getFinalScore(board, dimension, 'W');
        if (score1 > score2){
            printf("B player wins.\n");
        }else if (score1 == score2){
            printf("Draw!\n");
        }else if (score1 < score2){
            printf("W player wins.\n");
        }
    }else{
        printf("%c player wins.\n",winner);
    }
    
	return 0;
}

// AI: where to put the chess
void AIMove(char board[][26], int n, char color){
    char score[26][26];
    int row, col;
    int finalrow, finalcol;
    int maxscore = 0;
    
    // calculate score for every grid
    for (row = 0; row < n; row ++){
        for (col = 0; col < n; col ++){
            score[row][col] = getScore(board, n, row, col, color);
            if (score[row][col] > maxscore){
                maxscore = score[row][col];
                finalrow = row;
                finalcol = col;
            }
        }
    }
    // if Valid, then move
    if(checkMove(board, n, finalrow, finalcol, color)){
        printf("Computer places %c at %c%c.\n", color, numberToLetter(finalrow), numberToLetter(finalcol));
        newChessMove(board, n, finalrow, finalcol, color);
    }else{
        printf("%c player has no valid move.\n", color);
    }
}

// AI: get score for a single grid
int getScore(char board[][26], int n, int row, int col, char color){
    int score = 0;
    
	bool occupied = false;
	int rowCheckDir, colCheckDir;
    //printf("check: row=%d, col=%d, grid=%c\n",row, col, board[row][col]);
	if(board[row][col] != 'U'){
		occupied = true;
        score = -1;
	}

    // if the grid is unoccupied, cal the socre
	if (!occupied){
		for(rowCheckDir = -1; rowCheckDir <= 1; rowCheckDir++){
			for(colCheckDir = -1; colCheckDir <= 1; colCheckDir++){
				if (!(rowCheckDir==0 && colCheckDir==0) && checkLegalInDirection(board, n, numberToLetter(row), numberToLetter(col), color, rowCheckDir, colCheckDir)){
					score += flipInDirection(board, n, numberToLetter(row), numberToLetter(col), color, rowCheckDir, colCheckDir);
				}
			}
		}
	}
    
    return score;
}

// analyze the board, return the number of chesses on board of player
int getFinalScore(char board[][26], int n, char player){
    int score = 0;
    int row, col;
    
    // increment score when detect player's chess
    for (row = 0; row < n; row ++){
        for (col = 0; col < n; col ++){
            if (board[row][col] == player){
                score++;
            }
        }
    }
    
    return score;
}

// print the board
void printBoard(char board[][26], int n){
	int row = 0; int col = 0;
	for(row = -1; row < n; row ++){
		for(col = -2; col < n; col ++){
			// if the first row or first colomn
			if(row == -1){
				if(col != -1 && col != -2){
					printf("%c", col + 97);
				}else{
					printf(" ");
				}
			}else if (col == -2){
				printf("%c", row + 97);
			}else if (col == -1){
				printf(" ");
			}else{
				printf("%c", board[row][col]);
			}
			// determine whether to start a new line
			if (col == n - 1) {
				printf("\n");
			}
		}
	}
}

// row and col starts from 0
bool positionInBounds(int n, char row, char col){
	if (row >= n || col >= n){
		return false;
	}else{
		return true;
	}
}

// Move single chess
void chessMove(char* gridPoint, char player){
	*gridPoint = player;
}

// If valid, the outcome of new chess
void newChessMove(char board[][26], int n, int row, int col, char player){
    int rowCheckDir, colCheckDir;
    
    // in case of invalid move, check first
    if (checkMove(board, n, row, col, player)){
        //################# UPDATE IN EIGHT DIRECTIONS####################//
        for(rowCheckDir = -1; rowCheckDir <= 1; rowCheckDir++){
			for(colCheckDir = -1; colCheckDir <= 1; colCheckDir++){
				if (!(rowCheckDir==0 && colCheckDir==0)){
					updateChessInDirection(board, n, numberToLetter(row), numberToLetter(col), player, rowCheckDir, colCheckDir);
				}
			}
		}
        chessMove(&board[row][col], player);
    }
}

// change the color of in one direction
void updateChessInDirection(char board[][26], int n, char row, char col, char player, int deltaRow, int deltaCol){
    
    int originalRow = letterToNumber(row);
	int originalCol = letterToNumber(col);
    int checkCol = originalCol + deltaCol;
	int checkRow = originalRow + deltaRow;
    bool changeColor = true;
    char opponent;

	// determine the color of opponent
	if (player == 'W'){
		opponent = 'B';
	}else if (player == 'B'){
		opponent = 'W';
	}
    
    // If valid then change the color
    // 
    //printf("checkrow=%d checkcol=%d\n", row, col);
    if (checkLegalInDirection(board, n, row, col, player, deltaRow, deltaCol)){
        while (checkCol < n && checkRow < n && checkCol >= 0 && checkRow >= 0){
            //printf("checkrow=%d checkcol=%d colour=%c\n", checkRow, checkCol, board[checkRow][checkCol]);
            if(board[checkRow][checkCol] == 'U' || board[checkRow][checkCol] == player){
                changeColor = false;
            }
            
            if (changeColor == true) {
                chessMove(&board[checkRow][checkCol], player);
            }
            checkRow += deltaRow;
            checkCol += deltaCol;
        }
    }  
}

// return whether a legal move
bool checkLegalInDirection(char board[][26], int n, char row, char col, char color, int deltaRow, int deltaCol){
	// convert letter expression to number expression
	int originalRow = letterToNumber(row);
	int originalCol = letterToNumber(col);
    int number = 0;
    // printf("row=%d, col=%d, deltaRow=%d, deltaCol=%d\n", originalRow, originalCol, deltaRow, deltaCol);

	// the first encountered grid is opponent's chess or not
	bool opponentflag = false;

	int checkCol = originalCol + deltaCol;
	int checkRow = originalRow + deltaRow;
	char opponent;

	// determine the color of opponent
	if (color == 'W'){
		opponent = 'B';
	}else if (color == 'B'){
		opponent = 'W';
	}

	if((originalRow + deltaRow < n) && (originalRow + deltaRow >= 0) && (originalCol + deltaCol < n) 
        && (originalCol + deltaCol >= 0) && (board[originalRow + deltaRow][originalCol + deltaCol] == opponent)){
		opponentflag = true;
	}

	while (opponentflag && checkCol < n && checkRow < n && checkCol >= 0 && checkRow >= 0){
        //printf("check: row=%d, col=%d, grid=%c\n",checkRow, checkCol, board[checkRow][checkCol]);
		if(board[checkRow][checkCol] == 'U'){
			return false;
		}
        if(board[checkRow][checkCol] == color){
			return true;
		}
		checkRow += deltaRow;
		checkCol += deltaCol;
        number++;
	}

	return false;
}

// return whether a move for player is avaliable
bool checkMove(char board[][26], int n, int row, int col, char player){
	bool avaliable = false;
	bool occupied = false;
	int rowCheckDir, colCheckDir;
    //printf("check: row=%d, col=%d, grid=%c\n",row, col, board[row][col]);
	if(board[row][col] != 'U'){
		occupied = true;
	}

	if (!occupied){
		for(rowCheckDir = -1; rowCheckDir <= 1; rowCheckDir++){
			for(colCheckDir = -1; colCheckDir <= 1; colCheckDir++){
				if ( !avaliable && !(rowCheckDir==0 && colCheckDir==0) && checkLegalInDirection(board, n, numberToLetter(row), numberToLetter(col), player, rowCheckDir, colCheckDir)){
					avaliable = true;
				}
			}
		}
	}

	return avaliable;
}

// print all avaliable moves for player
void printAvaliableMove(char board[][26], int n, char player){
	int row, col;
	printf("Available moves for %c:\n", player);
	for (row = 0; row < n; row ++){
		for (col = 0; col < n; col ++){
			if (checkMove(board, n, row, col, player)){
				printf("%c%c\n",numberToLetter(row), numberToLetter(col));
			}
		}
	}
}

// return whether avaliable moves for player
bool isAvaliableMove(char board[][26], int n, char player){
    int row, col;
	for (row = 0; row < n; row ++){
		for (col = 0; col < n; col ++){
			if (checkMove(board, n, row, col, player)){
				return true;
			}
		}
	}
    return false;
}

// return letter expression to number
int letterToNumber(char letter){
	return letter - 97;
}

// return number expression to letter
char numberToLetter(int number){
	return number + 97;
}

// return the chess can be fliped in one direction
int flipInDirection(char board[][26], int n, char row, char col, char color, int deltaRow, int deltaCol){
	// convert letter expression to number expression
	int originalRow = letterToNumber(row);
	int originalCol = letterToNumber(col);
    int number = 0;
    // printf("row=%d, col=%d, deltaRow=%d, deltaCol=%d\n", originalRow, originalCol, deltaRow, deltaCol);

	// the first encountered grid is opponent's chess or not
	bool opponentflag = false;

	int checkCol = originalCol + deltaCol;
	int checkRow = originalRow + deltaRow;
	char opponent;

	// determine the color of opponent
	if (color == 'W'){
		opponent = 'B';
	} else if (color == 'B'){
		opponent = 'W';
	}

    // if in bound, and the first chess is opponent, then continue 
	if((originalRow + deltaRow < n) && (originalRow + deltaRow >= 0) && (originalCol + deltaCol < n) 
        && (originalCol + deltaCol >= 0) && (board[originalRow + deltaRow][originalCol + deltaCol] == opponent)){
		opponentflag = true;
	}

    // increment number, until detect player
	while (opponentflag && checkCol < n && checkRow < n && checkCol >= 0 && checkRow >= 0){
		if(board[checkRow][checkCol] == 'U'){
			return 0;
		}
        if(board[checkRow][checkCol] == color){
			return number;
		}else{
            number++;
        }
		checkRow += deltaRow;
		checkCol += deltaCol;
        
	}

	return number;
}
