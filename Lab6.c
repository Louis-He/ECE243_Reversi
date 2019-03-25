#include <stdio.h>
#include <stdbool.h>
//print board
void printBoard(char board[][26], int n);

void chessMove(char* gridPoint, char player);
void newChessMove(char board[][26], int n, int row, int col, char player);
void updateChessInDirection(char board[][26], int n, char row, char col, char player, int deltaRow, int deltaCol);
bool positionInBounds(int n, char row, char col);
bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol);
bool checkMove(char board[][26], int n, int row, int col, char player);
void printAvaliableMove(char board[][26], int n, char player);

// basic helper functions
int letterToNumber(char letter);
char numberToLetter(int number);

// main function
int main(int argc, char **argv){
	int dimension, row, col;
	char configurationInput[3];
    char userInput[3];
	char player; int rowenter, colenter;

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
    printBoard(board, dimension);

	// set the configuration
    printf("Enter board configuration:\n");
	while(true){
			scanf("%s", &configurationInput);
			if(strncmp(configurationInput,"!!!",3) == 0){
				break;
			}else{
				player = configurationInput[0];
				rowenter = letterToNumber(configurationInput[1]);
				colenter = letterToNumber(configurationInput[2]);
				chessMove(&board[rowenter][colenter], player);
			}
	}
	printBoard(board, dimension);

	// print out avaliable moves for both players
	printAvaliableMove(board, dimension, 'W');
	printAvaliableMove(board, dimension, 'B');
    
    // Request enter a move
    printf("Enter a move:\n");
    scanf("%s", userInput);
    
    // override three variables to new userinput
    player = userInput[0];
    rowenter = letterToNumber(userInput[1]);
    colenter = letterToNumber(userInput[2]);
    
    if(checkMove(board, dimension, rowenter, colenter, player)){
        printf("Valid move.\n");
        newChessMove(board, dimension, rowenter, colenter, player);
    }else{
        printf("Invalid move.\n");
    }
    
    printBoard(board, dimension);
    //I love you
	return 0;
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
    if (checkLegalInDirection(board, n, row, col, player, deltaRow, deltaCol)){
        while (checkCol < n && checkRow < n && checkCol >= 0 && checkRow >= 0){
			// stop change color when done
            if(board[checkRow][checkCol] == 'U' || board[checkRow][checkCol] == player){
                changeColor = false;
            }
            
			// change color only when the move is valid
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

	// determine whether the first chess is opponent
	if((originalRow + deltaRow < n) && (originalRow + deltaRow >= 0) && (originalCol + deltaCol < n) 
        && (originalCol + deltaCol >= 0) && (board[originalRow + deltaRow][originalCol + deltaCol] == opponent)){
		opponentflag = true;
	}

	// continue detecting
	while (opponentflag && checkCol < n && checkRow < n && checkCol >= 0 && checkRow >= 0){
		// if detect space, then invalid
		if(board[checkRow][checkCol] == 'U'){
			return false;
		}
		// if detect color, then valid
        if(board[checkRow][checkCol] == color){
			return true;
		}
		checkRow += deltaRow;
		checkCol += deltaCol;
	}

	return false;
}

// return whether a move for player is avaliable
bool checkMove(char board[][26], int n, int row, int col, char player){
	bool avaliable = false;
	bool occupied = false;
	int rowCheckDir, colCheckDir;

	if(board[row][col] != 'U'){
		occupied = true;
	}

	// if not occupied, then start determine
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

// return all avaliable moves for player
void printAvaliableMove(char board[][26], int n, char player){
	int row, col;
	printf("Available moves for %c:\n", player);
	
	// check every legal move, if valid then print
	for (row = 0; row < n; row ++){
		for (col = 0; col < n; col ++){
			if (checkMove(board, n, row, col, player)){
				printf("%c%c\n",numberToLetter(row), numberToLetter(col));
			}
		}
	}
}

// return letter expression to number
int letterToNumber(char letter){
	return letter - 97;
}

// return number expression to letter
char numberToLetter(int number){
	return number + 97;
}
