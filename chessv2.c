#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#define ROWS 8
#define COLS 8
void printBoard(char board[ROWS][COLS]);
void initializeBoard(char board[ROWS][COLS]);
void Turn(char board[ROWS][COLS], char *color);
int pawnMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc);
int isWhite(char spot);
int isBlack(char spot);
int rookMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc);
int bishopMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc);
int knightMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc);
int queenMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc);
int kingMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc);
void movePiece (char board[ROWS][COLS], int row, int col, int rowLoc, int colLoc, int castling);
int checkKing(char board[ROWS][COLS], char *color);
void attemptPromote(char board[ROWS][COLS], int row, int col, int rowLoc, int colLoc);


typedef struct {
    int kingMoved;
    int leftRookMoved;
    int rightRookMoved;
} Castling;

Castling whiteCastling = {0, 0, 0};
Castling blackCastling = {0, 0, 0};

int main()
{
    char board[ROWS][COLS];
    printf("Welcome to chess in C! To move simply enter the location \nof the piece you want to move and then where you want it to move to. To castle move your rook ontop of your king and it will castle automatically.\n");
    initializeBoard(board);

    Turn(board, "White"); // always starts with white
    return 0;

}

void printBoard(char board[ROWS][COLS]) // prints the board
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // for changing color and background
    SetConsoleTextAttribute(hConsole, 15);
    SetConsoleOutputCP(CP_UTF8); // for printing unicode pieces
    printf("%35s", "A   B   C   D   E   F   G   H\n"); 
    printf("  +--------------------------------+\n");
    for(int i = ROWS - 1; i >= 0; i--) //  iterates through board array and prints each piece
    {
        printf("%d |", i + 1);
        char *piece;
        for (int j = 0; j < COLS; j++)
        {
            switch(board[i][j])
            {
                case 'r':
                    piece = "♖";
                    break;
                case 'R':
                    piece = "♜";
                    break;
                case 'n':
                    piece = "♘";
                    break;
                case 'N':
                    piece = "♞";
                    break;
                case 'b':
                    piece = "♗";
                    break;
                case 'B':
                    piece = "♝";
                    break;
                case 'k':
                    piece = "♔";
                    break;
                case 'K':
                    piece = "♚";
                    break;
                case 'q':
                    piece = "♕";
                    break;
                case 'Q':
                    piece = "♛";
                    break;
                case 'p':
                    piece = "♙";
                    break;
                case 'P':
                    piece = "♟︎";
                    break;
                case ' ':
                    piece = " ";
                    break;
                default:
                    break;                    

            }

            if (board[i][j] <= 90 && board[i][j] >= 65)
            {
                if ((j + i) % 2 == 0)
                {
                    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
                    SetConsoleTextAttribute(hConsole, 0 | 32); // color and background
                    printf(" %s  ", piece);
                }
                else
                {
                    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, 0 | 112);
                    printf(" %s  ", piece);
                }
            }
            else
            {
                if ((j + i) % 2 == 0)
                    {
                    SetConsoleTextAttribute(hConsole, 0 | 32);
                    printf(" %s  ", piece);
                    }
                else
                    {
                    SetConsoleTextAttribute(hConsole, 0 | 112);
                    printf(" %s  ", piece);
                    }
            }
        }
        SetConsoleTextAttribute(hConsole, 15);
        printf("|\n");
        
    }
    printf("  +--------------------------------+\n");

}

void initializeBoard(char board[ROWS][COLS]) // sets up board with initial positions
{
    for(int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            switch(i)
            {
                case 1:
                case 6:
                    board[i][j] = 'p';
                    break;
                case 0:
                case 7:
                    switch(j)
                    {
                        case 0:
                        case 7:
                            board[i][j] = 'r';
                            break;
                        case 1:
                        case 6:
                            board[i][j] = 'n';
                            break;
                        case 2:
                        case 5:
                            board[i][j] = 'b';
                            break;
                        case 3:
                            board[i][j] = 'q';
                            break;
                        case 4:
                            board[i][j] = 'k';
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                board[i][j] = ' ';
            }
            if(i == 6 || i == 7)
            {
                board[i][j] = board[i][j] - 32;
            }
        }
    }
}

void Turn(char board[ROWS][COLS], char *color) // runs a players turn of said color
{
    printBoard(board);
    char colorOffset = 0;
    int rowLoc; // row and char location
    int colLoc;
    int rowDes; // row and char destination
    int colDes;
    char temp;
    printf("%s to move, enter the location of the piece you would like to move: ", color);
    scanf(" %c%d", &temp, &rowLoc);
    temp = temp - 'A' + '0';
    colLoc = atoi(&temp); // WIP NEED TO SUB ASCII
    rowLoc--;
    
    printf("Enter the location of where you like to move this piece: "); 
    scanf(" %c%d", &temp, &rowDes); 
    temp = temp - 'A' + '0';
    colDes = atoi(&temp);
    rowDes--;



    switch(board[rowLoc][colLoc]) // simply checks the piece type attempting to move and does the respective move
    {
        case 'p':
        case 'P':
            if(pawnMove(board, color, rowDes, colDes, rowLoc, colLoc))
            {
                movePiece (board, rowDes, colDes, rowLoc, colLoc, 0);
            }
            else
            {
                printf("Invalid move.\n");
                Turn(board, color);
            }
            break;
        case 'r':
        case 'R':
            if(rookMove(board, color, rowDes, colDes, rowLoc, colLoc) == 1)
            {
                movePiece (board, rowDes, colDes, rowLoc, colLoc, 0);
            }
            else if(rookMove(board, color, rowDes, colDes, rowLoc, colLoc) == 2)
            {
                movePiece (board, rowDes, colDes, rowLoc, colLoc, 1);
            }
            else
            {
                printf("Invalid move.\n");
                Turn(board, color);
            }
            break;
        case 'b':
        case 'B':
            if(bishopMove(board, color, rowDes, colDes, rowLoc, colLoc))
            {
                movePiece (board, rowDes, colDes, rowLoc, colLoc, 0);
            }
            else
            {
                printf("Invalid move.\n");
                Turn(board, color);
            }
            break;
        case 'n':
        case 'N':
            if(knightMove(board, color, rowDes, colDes, rowLoc, colLoc))
            {
                movePiece (board, rowDes, colDes, rowLoc, colLoc, 0);
            }
            else
            {
                printf("Invalid move.\n");
                Turn(board, color);
            }
            break;
        case 'Q':
        case 'q':
            if(queenMove(board, color, rowDes, colDes, rowLoc, colLoc))
            {
                movePiece (board, rowDes, colDes, rowLoc, colLoc, 0);
            }
            else
            {
                printf("Invalid move.\n");
                Turn(board, color);
            }
            break;
        case 'K':
        case 'k':
            if(kingMove(board, color, rowDes, colDes, rowLoc, colLoc))
            {
                movePiece (board, rowDes, colDes, rowLoc, colLoc, 0);
            }
            else
            {
                printf("Invalid move.\n");
                Turn(board, color);
            }
            break;
        case ' ':
            printf("A piece must be selected.\n");
            Turn(board, color);
            break;
        default:
        break;
    }

}

int pawnMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc)  // returns 0 if cant move 1 if can
{
    if(strcmp(color, "White") == 0) // makes sure correct color piece selected
    {
        if(isBlack(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }

    if(strcmp(color, "Black") == 0)
    {
        if(isWhite(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }
    
    
    //invalid move if: ANY PIECE IS IN FRONT or if out of bounds

    if(row < 0 || row > 7 || col < 0 || col > 7)
    {
        return 0;
    }
    int colDiff = col - colLoc;
    int rowDiff = row - rowLoc;
    

    // if moves left/right over 1
    if(abs(colDiff)  > 1)
        return 0;



    // ALSO invalid move if not by 1 row and left/right 1 col if ' ' isnt there or ally piece isnt there
    if(strcmp(color, "White") == 0) 
    {
        if(isWhite(board[row][col])) // CHECKS IF WHITE PIECE THEN CHECKS IF IT HAS MOVED YET AND ALLOWS 2 MOVE OTHERWISE 1 MOVE
        {
            return 0;
        }
        if(rowLoc == 1)
        {
            if(rowDiff > 2 || rowDiff < 1)
            {
                return 0;
            }
        }
        else
        {
            if(rowDiff != 1)
            {
                return 0;    
            }
        }



        if(abs(colDiff) == 1) // diagonal white move
        {
            if(isBlack(board[row][col]) == 0)
            {
                return 0;
            }
            else if(board[row][col] == ' ' && isBlack(board[row - 1][col]))
            {
                board[row - 1][col] = ' ';
                attemptPromote(board, row, col, rowLoc, colLoc);
                return 1;
            }
        }


    }
    else
    {
        if(isBlack(board[row][col])) // Checks if black piece then if it has moved yet 
        {
            return 0;
        }
        if(rowLoc == 6)
        {
            if(row - rowLoc < -2 || row - rowLoc > -1)
            {
                return 0; 
            }
        }
        else
        {
            if(row - rowLoc != -1)
            {
                return 0;
            }
        }

        
        if(abs(colDiff) == 1) // diagonal move condition
        {
            if(isWhite(board[row][col]) == 0)
            {
                return 0;
            }
            else if(board[row][col] == ' ' && isWhite(board[row + 1][col]))
            {
                board[row + 1][col] = ' ';
                attemptPromote(board, row, col, rowLoc, colLoc);
                return 1;
            }
        }
    }


    attemptPromote(board, row, col, rowLoc, colLoc); // always sees if can promote at end of turn
    return 1;
} 


int rookMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc) // returns 0 if cant move 1 if can 2 if castle 
{


    



    // makes sure piece being moves is right color
    if(strcmp(color, "White") == 0)
    {
        if(isBlack(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }

    if(strcmp(color, "Black") == 0)
    {
        if(isWhite(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }




    // out of bounds
    if(row < 0 || row > 7 || col < 0 || col > 7)
    {
        return 0;
    }



    // castling conditions 
    if(whiteCastling.kingMoved == 0 && row == 0 && col == 4)
    {
        if((rowLoc == 0 && colLoc == 0 && whiteCastling.leftRookMoved == 0) || (rowLoc == 0 && colLoc == 7 && whiteCastling.rightRookMoved == 0))
        {
            int direction = (col - colLoc) / abs(col - colLoc);
            for(int i = colLoc + direction; i != 4; i+= direction)
            {
                if(board[rowLoc][i] != ' ' )
                {
                    return 0;
                }
            }
            board[0][4 - direction] = board[0][4];
            if(checkKing(board, "White"))
            {
                board[0][4 - direction] = ' ';
                return 0;
            }

            return 2;
        }
    }
    if(blackCastling.kingMoved == 0 && row == 7 && col == 4)
    {
        if((rowLoc == 7 && colLoc == 0 && blackCastling.leftRookMoved == 0) ||(rowLoc == 7 && colLoc == 7 && blackCastling.rightRookMoved == 0))
        {
            int direction = (col - colLoc) / abs(col - colLoc);
            for(int i = colLoc + direction; i != 4; i+= direction)
            {
                if(board[rowLoc][i] != ' ')
                {
                    return 0;
                }
            }
            board[7][4 - direction] = board[7][4];
            if(checkKing(board, "Black"))
            {
                board[7][4 - direction] = ' ';
                return 0;
            }

            return 2;
        }
    }


    // piece moving diagonally
    if((row != rowLoc) && (col != colLoc))
    {
        return 0;
    }
    
    //piece not moving
    if((row == rowLoc) && (col == colLoc))
    {
        return 0;
    }


    // vertical move
    if(col == colLoc)
    {
        int direction = (row - rowLoc) / abs(row - rowLoc);
        for(int i = rowLoc + direction; i != row ; i+= direction)
        {
            if(board[i][colLoc] != ' ')
            {
                return 0;
            }
        }

    }
    // horizontal move
    else if(row == rowLoc)
    {
        int direction = (col - colLoc) / abs(col - colLoc);
        for(int i = colLoc + direction; i != col; i+= direction)
        {
            if(board[rowLoc][i] != ' ')
            {
                return 0;
            }
        }

    }

    // makes sure capture piece right color
    if (strcmp(color, "White") == 0)
    {
        if(isWhite(board[row][col]))
        {
            return 0;
        }
    }
    else
    {
        if(isBlack(board[row][col]))
        {
            return 0;   
        }
    }

    return 1;

}

int bishopMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc)  // returns 0 if cant move 1 if can
{
        
        if(strcmp(color, "White") == 0) // makes sure correct color piece selected
        {
            if(isBlack(board[rowLoc][colLoc]))
            {
                return 0;
            }
        }

        if(strcmp(color, "Black") == 0)
        {
            if(isWhite(board[rowLoc][colLoc]))
            {
                return 0;
            }
        }


        // out of bounds
        if(row < 0 || row > 7 || col < 0 || col > 7)
        {
            return 0;
        }

        
        int rowDif = row - rowLoc;
        int colDif = col - colLoc;


        // not diagonal
        if (abs(rowDif) != abs(colDif))
        {
            return 0;
        }

        int rowStep = rowDif / abs(rowDif);
        int colStep = colDif / abs(colDif);
        int step = abs(rowDif);


        // diagonal movement
        for(int i = 1; i < step; i++) // steps in direction of rowDiff and colDiff
        {
            if(board[rowLoc + (i * rowStep)][colLoc + (i * colStep)] != ' ')
            {
                return 0;
            }
        }


        

        if (isWhite(board[rowLoc][colLoc])) // makes sure if capturing the piece is of opposite color
        {
            if(isWhite(board[row][col]))
            {
                return 0;
            }
        }
        else
        {
            if(isBlack(board[row][col]))
            {
                return 0;
            }
        }
        
        return 1;
        
}

int knightMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc) // returns 0 if cant move 1 if can
{
   
   
    if(strcmp(color, "White") == 0) // makes sure piece selected is right color
    {
        if(isBlack(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }

    if(strcmp(color, "Black") == 0)
    {
        if(isWhite(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }
   
   if(row < 0 || row > 7 || col < 0 || col > 7) // out of bounds
    {
        return 0;
    }
    
    if(!(abs(row - rowLoc) + abs(col - colLoc) == 3 && (abs(row - rowLoc) == 1 || abs(col - colLoc) == 1))) // checks if L shape 
    {
        return 0;
    }


    if (isWhite(board[rowLoc][colLoc])) // makes sure captured piece is of opposite color
    {
        if(isWhite(board[row][col]))
        {
            return 0;
        }
    }
    else
    {
        if(isBlack(board[row][col]))
        {
            return 0;
        }
    }

    return 1;
}

int queenMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc) // returns 0 if cant move 1 if can
{
    if(bishopMove(board, color, row, col, rowLoc, colLoc) || rookMove(board, color, row, col, rowLoc, colLoc)) // combination of bishop and rook move
    {
        return 1;
    }
    return 0;
}

int kingMove(char board[ROWS][COLS], char *color, int row, int col, int rowLoc, int colLoc) // returns 0 if cant move 1 if can
{
    
    if(strcmp(color, "White") == 0) // make sure piece selected is of right color
    {
        if(isBlack(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }

    if(strcmp(color, "Black") == 0)
    {
        if(isWhite(board[rowLoc][colLoc]))
        {
            return 0;
        }
    }
    
    
    if(row < 0 || row > 7 || col < 0 || col > 7) // out of bounds
    {
        return 0;
    }

    if(row == rowLoc && col == colLoc) // makes sure it moves
    {
        return 0;
    }

    int colDif = col - colLoc;
    int rowDif = row - rowLoc;

    if (abs(colDif) > 1 || abs(rowDif) > 1) // make sure movement doesnt exceed 1
    {
        return 0;
    }

    if (strcmp(color, "White") == 0) // makes sure piece being captured is opposite color
    {
        if(isWhite(board[row][col]))
        {
            return 0;
        }
    }
    else
    {
        if(isBlack(board[row][col]))
        {
            return 0;
        }
    }

    return 1;

}

int checkKing(char board[ROWS][COLS], char *color) //returns 1 if king is in threat 0 if not
{
    int i;
    int j;
    int kingRow = -1;
    int kingCol = -1;
    int kingThreat = 0;
    char kingChar;
    char *movingColor;
    if(strcmp(color, "White") == 0)
    {
        kingChar = 'k';
        movingColor = "Black";
    }
    else
    {
        kingChar = 'K';
        movingColor = "White";
    }
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if((board[i][j]) == kingChar ) // locates king location of row and col
            {
                kingRow = i;
                kingCol = j;
            }
            
        }
    }

    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            switch(board[i][j]) // checks every piece on board and attempts to move it to king - if it can sets kingthreat to 1
            { 
                case 'p':
                case 'P':
                
                    if(pawnMove(board, movingColor, kingRow, kingCol, i, j))
                    {
                        printf("Pawn - %s", movingColor);
                        kingThreat = 1;
                    }
                    break;
                case 'r':
                case 'R':
                    if(rookMove(board, movingColor, kingRow, kingCol, i, j))
                    {
                        printf("Rook- %s", movingColor);
                        kingThreat = 1;
                    }
                    break;
                case 'b':
                case 'B':
                    if(bishopMove(board, movingColor, kingRow, kingCol, i, j))
                    {
                        printf("Bishop");
                        kingThreat = 1;
                    }
                    break;
                case 'n':
                case 'N':
                    if(knightMove(board, movingColor, kingRow, kingCol, i, j))
                    {
                        printf("Knight");
                        kingThreat = 1;
                    }
                    break;
                case 'Q':
                case 'q':
                    if(queenMove(board, movingColor, kingRow, kingCol, i, j))
                    {
                        printf("Queen- %s", movingColor);
                        kingThreat = 1;
                    }
                    break;
                case 'K':
                case 'k':
                    if(kingMove(board, movingColor, kingRow, kingCol, i, j))
                    {
                        printf("King\n");
                        kingThreat = 1;
                    }
                    break;
                case ' ':
                    break;
                default:
                break;
            }
            
        }
    }
    return kingThreat; // returns kingthreat

}

void movePiece (char board[ROWS][COLS], int row, int col, int rowLoc, int colLoc, int castling) // moves pawn to row and col from rowLoc colLoc
{
    if(castling)
    {
        int direction = (col - colLoc) / abs(col - colLoc); // dir rook moving
        char temp = board[row][col]; // initial king
        char tempLoc = board[rowLoc][colLoc]; // initial rook
        board[row][col -  (2 *direction)] = temp;
        board[row][col - direction] = tempLoc;
        if (isWhite(board[row][col])) // checks to make sure king isnt moving to self check
        {
            if(checkKing(board, "White"))
            {
                board[row][col - (2* direction)] = ' ';
                board[row][col - direction] = ' ';
                printf("Invalid Move.\n");
                Turn(board, "White");
            }
            if(board[row][col - direction] == 'r')
            {
                if(colLoc == 0 && rowLoc == 0)
                {
                    whiteCastling.leftRookMoved = 1;
                }
                else if(colLoc == 7 && rowLoc == 0)
                {
                    whiteCastling.rightRookMoved = 1;
                }
            }
            whiteCastling.kingMoved = 1;
            board[row][col] = ' ';
            board[rowLoc][colLoc] = ' '; // removes old positions
            printf("Piece has been moved to: %c%d\n", (col + 'A'), row + 1);
            Turn(board, "Black");
        }
        else
        {
            if(checkKing(board, "Black")) // checks to make sure king isnt moving to self check
            {
                board[row][col - (2* direction)] = ' '; // restored old positions
                board[row][col - direction] = ' ';
                printf("Can't pass through a self-check!\n");
                Turn(board, "Black");
            }
            if(board[row][col - direction] == 'R')
            {
                if(colLoc == 0 && rowLoc == 7)
                {
                    blackCastling.leftRookMoved = 1;
                }
                else if(colLoc == 7 && rowLoc == 7)
                {
                    blackCastling.rightRookMoved = 1;
                }
            }
            blackCastling.kingMoved = 1; // removes old postions
            board[row][col] = ' ';
            board[rowLoc][colLoc] = ' ';
            printf("Piece has been moved to: %c%d\n", (col + 'A'), row + 1);
            Turn(board, "White");
            exit(0);
        }
    }
    switch (board[row][col]) // checks to see if a piece is being captured and prints text for such.
    {
        case 'p':
        case 'P':
            printf("Pawn has been captured\n");
            break;
        case 'r':
        case 'R':
            printf("Rook has been captured\n");
            break;
        case 'b':
        case 'B':
            printf("Bishop has been captured\n");
            break;
        case 'n':
        case 'N':
            printf("Knight has been captured\n");
            break;
        case 'Q':
        case 'q':
            printf("Queen has been captured\n");
            break;
        case 'K':
            printf("Black's King has been captured! White wins!");
            exit(0);
        case 'k':
            printf("White's King has been captured! Black wins!");
            exit(0);
        default:
        break;
    }
        char temp = board[row][col];
        board[row][col] = board[rowLoc][colLoc];
        board[rowLoc][colLoc] = ' ';
        if (isWhite(board[row][col]))
        {
            if(checkKing(board, "White"))
            {
                board[rowLoc][colLoc] = board[row][col]; // checks kind and if king is not safe restores positions
                board[row][col] = temp;
                printf("Can't self-check!\n");
                Turn(board, "White");
            }
            if(board[row][col] == 'r')
            {
                if(colLoc == 0 && rowLoc == 0) // castling variables
                {
                    whiteCastling.leftRookMoved = 1;
                }
                else if(colLoc == 7 && rowLoc == 0)// castling variables
                {
                    whiteCastling.rightRookMoved = 1;
                }
            }
            if(board[row][col] == 'K')// castling variables
            {
                whiteCastling.kingMoved = 1;
            }
            printf("Piece has been moved to: %c%d\n", (col + 'A'), row + 1);
            Turn(board, "Black");
        }
        else
        {
            if(checkKing(board, "Black"))// checks kind and if king is not safe restores positions
            {
                board[rowLoc][colLoc] = board[row][col];
                board[row][col] = temp;
                printf("Can't self-check!\n");
                Turn(board, "Black");
            }
            if(board[row][col] == 'R')
            {
                if(colLoc == 0 && rowLoc == 7)// castling variables
                {
                    blackCastling.leftRookMoved = 1;
                }
                else if(colLoc == 7 && rowLoc == 7)// castling variables
                {
                    blackCastling.rightRookMoved = 1;
                }
            }
            if(board[row][col] == 'K')// castling variables
            {
                blackCastling.kingMoved = 1;
            }
            printf("Piece has been moved to: %c%d\n", (col + 'A'), row + 1);
            Turn(board, "White");
        }

}


void attemptPromote(char board[ROWS][COLS], int row, int col, int rowLoc, int colLoc) //attemps to promote pawn
{
        char promotionPiece[10];
        if ((board[rowLoc][colLoc] =='P' && row == 0) || (board[rowLoc][colLoc] == 'p' && row == 7))
        {
            printf("What piece would you like to promote your pawn to?\nOptions: Knight, Rook, Bishop, or Queen: ");
            scanf("%10s", promotionPiece);
            switch(promotionPiece[0])
            {
                case 'K':
                case 'k':
                    board[rowLoc][colLoc] = board[rowLoc][colLoc] - 2;
                    break;
                case 'R':
                case 'r':
                    board[rowLoc][colLoc] = board[rowLoc][colLoc] + 2;
                case 'B':
                case 'b':
                    board[rowLoc][colLoc] = board[rowLoc][colLoc] - 14;
                case 'Q':
                case 'q':
                    board[rowLoc][colLoc] = board[rowLoc][colLoc] + 1;
                    break;
                default:
                    printf("Must choose valid piece");
                    attemptPromote(board, row, col, rowLoc, colLoc);
                    break;
            }

        }
}





int isWhite(char spot) // if spot is white returns 1 else 0
{
    if(spot >= 97 && spot <= 122)
        return 1;
    else
        return 0;
    
}

int isBlack(char spot) // if spot is black returns 1 else 0
{
    if(spot >= 65 && spot <= 90)
        return 1;
    else
        return 0;
    
}