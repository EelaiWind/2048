#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef GLOBAL
#define GLOBAL "Global.h"
#include GLOBAL
#endif

void InitGame(Board);
int FinishGame(Board);
int RandBlock(Board);
void PrintBoard(Board);
void KeyBoard(char,Board);
int MoveRight(Board, Board);
int MoveLeft(Board, Board);
int MoveDown(Board, Board);
int MoveUp(Board, Board);
void MoveBlock(Board,int,int,int,int);
void UpdateBoard(Board,Board);
/*
int main(){

    char c;
    int i;
    int gameBoard[4][4];

    InitGame(gameBoard);
    PrintBoard(gameBoard);

    while (1){
        c = getch();
        if (c == 'q')
            break;
        else if ( c == 'z'){
            for ( i = 0 ; i < 16 ; i ++)
                printf(" %d",gameBoard[i/4][i%4]);
        }
        else{
            KeyBoard(c, gameBoard);
        }
        printf("[%c]\n",c);
        Sleep(500);
        if ( FinishGame(gameBoard) == 1)
            break;
    }
    return 0;
}
*/
void InitGame(Board gameBoard){

    int i,j;

    srand(rand());
    for ( i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j< BOARD_WIDTH ; j++){
            gameBoard[i][j] = 0 ;
        }
    }
    /*gameBoard[0][0] = 3;
    gameBoard[0][1] = 1;
    gameBoard[0][2] = 1;
    gameBoard[0][3] = 1;

    gameBoard[1][0] = 8;
    gameBoard[1][1] = 8;
    gameBoard[1][2] = 9;
    gameBoard[1][3] = 10;

    gameBoard[2][0] = 3;
    gameBoard[2][1] = 4;
    gameBoard[2][2] = 3;
    gameBoard[2][3] = 4;

    gameBoard[3][0] = 1;
    gameBoard[3][1] = 2;
    gameBoard[3][2] = 6;
    gameBoard[3][3] = 1;*/
    RandBlock(gameBoard);
    RandBlock(gameBoard);

    return;
}

int FinishGame(Board gameBoard){

    int i,j;
    int max = 0;
    int full = 1, hasEmpty = 0;

    for ( i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            if (gameBoard[i][j] > max)
                max = gameBoard[i][j];

            if (gameBoard[i][j] == 0)
                hasEmpty = 1;

            if ( i > 0 && gameBoard[i - 1][j] == gameBoard[i][j]){
                full = 0;
            }
            if ( i < BOARD_HEIGHT -1 && gameBoard[i + 1][j] == gameBoard[i][j]){
                full = 0;
            }
            if ( j > 0 && gameBoard[i][j - 1] == gameBoard[i][j]){
                full = 0;
            }
            if ( j < BOARD_WIDTH -1 && gameBoard[i][j + 1] == gameBoard[i][j]){
                full = 0;
            }

        }
    }

    if ( max == 11){
        TotalScore += 1000000;
        if (PLAY_MODE == 1) printf("You Win!(%d) \n", max);
        return max;
    }
    else if ( hasEmpty == 0 && full == 1){
        if (PLAY_MODE == 1) printf("You lose!! (%d)\n", max);
        return max;
    }
    else
        return 0;

}

/*
    Generate a block, which may be level 1 or 2 ,
    on a random position on the board

    70% : level 1 block
    30% : level 2 block

    Return 1/0 means create successful/failed
*/
int RandBlock(Board gameBoard){

    int i;
    int empty[BOARD_HEIGHT * BOARD_WIDTH],count=0;
    int point;
    int level;

    for ( i = 0 ; i < 16 ; i++){
        if (gameBoard[i/BOARD_HEIGHT][i%BOARD_WIDTH] == 0)
            empty[count++] = i;
    }

    if ( count > 0){
        if (rand()%10 > 7){
            level = 2;
        }
        else{
            level = 1;
        }

        point = empty[rand()%count];
        gameBoard[point/BOARD_HEIGHT][point%BOARD_WIDTH] = level;
        TotalScore += 2*level;
        return 1;
    }
    else
        return 0;
}

void PrintBoard(Board gameBoard){

    int i,j;

    if (PLAY_MODE == 0)
        return;
    //system("pause");
    system("cls");
    printf("Total score = %d\n",TotalScore);
    printf("+--+--+--+--+\n");
    for ( i = 0 ; i < BOARD_HEIGHT ; i ++){
        for ( j = 0 ; j< BOARD_WIDTH ; j++){
            putchar('|');
            if ( gameBoard[i][j] == 0)
                printf("  ");
            else{
                printf("%2d",gameBoard[i][j]);
            }
        }
        putchar('|');
        putchar('\n');
        printf("+--+--+--+--+\n");
    }

}

void KeyBoard(char key, Board gameBoard){

    Board nextBoard;

    if ( key =='A' || key == 'a'){
        MoveLeft(nextBoard, gameBoard);
        UpdateBoard(gameBoard,nextBoard);
    }
    else if ( key =='D' || key == 'd'){
        MoveRight(nextBoard, gameBoard);
        UpdateBoard(gameBoard,nextBoard);
    }
    else if ( key =='W' || key == 'w'){
        MoveUp(nextBoard, gameBoard);
        UpdateBoard(gameBoard,nextBoard);
    }
    else if ( key =='S' || key == 's'){
        MoveDown(nextBoard, gameBoard);
        UpdateBoard(gameBoard,nextBoard);
    }
}
/*
    The next stage of the GameBoard will be store in the nextBoard[][].
    Fuction return 0 if no changes happends.
*/
int MoveRight(Board nextBoard, Board nowBoard){

    int i,j;
    Board lock;
    int modified = 0;
    int endY;

    for ( i = 0 ; i < BOARD_HEIGHT ; i++)
        for ( j = 0 ; j < BOARD_WIDTH ;j++){
            lock[i][j] = 0 ;
            nextBoard[i][j] = nowBoard[i][j];
        }

    for ( i = 0 ; i < BOARD_HEIGHT ; i++){

        endY = -1;
        for ( j = BOARD_WIDTH - 1 ; j >= 0 ; j--){
            if (nextBoard[i][j] != 0){
                if (endY == -1){
                    endY = BOARD_WIDTH-1;
                    if ( j != BOARD_WIDTH - 1 ){
                        MoveBlock(nextBoard, i,j,i,BOARD_WIDTH-1);
                        modified = 1;
                    }
                }
                else if (nextBoard[i][j] == nextBoard[i][endY] && lock[i][endY] ==0){
                    MoveBlock(nextBoard, i,j,i,endY);
                    lock[i][endY] = 1;
                    modified = 1;
                }
                else{
                    if ( j != endY -1){
                        MoveBlock(nextBoard, i,j,i,endY-1);
                        modified = 1;
                    }
                    endY  = endY-1;

                }
            }
        }
    }
    return modified;
}

int MoveLeft(Board nextBoard, Board nowBoard){
    int i,j;
    int modified = 0;
    Board lock;
    int endY;

    for ( i = 0 ; i < BOARD_WIDTH ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ;j++){
            lock[i][j] = 0 ;
            nextBoard[i][j] = nowBoard[i][j];
        }
    }

    for ( i = 0 ; i < BOARD_HEIGHT ; i++){

        endY = -1;
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            if (nextBoard[i][j] != 0){
                if (endY == -1){
                    endY = 0;
                    if ( j != 0){
                        MoveBlock(nextBoard, i,j,i,0);
                        modified = 1;
                    }
                }
                else if (nextBoard[i][j] == nextBoard[i][endY] && lock[i][endY] ==0){
                    MoveBlock(nextBoard, i,j,i,endY);
                    lock[i][endY] = 1;
                    modified = 1;
                }
                else{
                    if ( j != endY+1){
                        MoveBlock(nextBoard, i,j,i,endY+1);
                        modified = 1;
                    }
                    endY  = endY+1;
                }
            }
        }
    }
    return modified;
}

int MoveUp(Board nextBoard, Board nowBoard){
    int i,j;
    int modified = 0;
    Board lock;
    int endX;

    for ( i = 0 ; i < BOARD_WIDTH ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ;j++){
            lock[i][j] = 0 ;
            nextBoard[i][j] = nowBoard[i][j];
        }
    }

    for ( i = 0 ; i < BOARD_WIDTH ; i++){

        endX = -1;
        for ( j = 0 ; j < BOARD_HEIGHT ; j++){
            if (nextBoard[j][i] != 0){
                if (endX == -1){
                    if ( j != 0){
                        MoveBlock(nextBoard, j,i,0,i);
                        modified = 1;
                    }
                    endX = 0;
                }
                else if (nextBoard[j][i] == nextBoard[endX][i] && lock[endX][i] ==0){
                    MoveBlock(nextBoard, j,i,endX,i);
                    lock[endX][i] = 1;
                    modified = 1;
                }
                else{
                    if ( j != endX +1){
                        MoveBlock(nextBoard, j,i,endX+1,i);
                        modified = 1;
                    }
                    endX  = endX+1;
                }
            }
        }
    }
    return modified;
}

int MoveDown(Board nextBoard, Board nowBoard){
    int i,j;
    int modified = 0;
    Board lock;
    int endX;

    for ( i = 0 ; i < BOARD_WIDTH ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ;j++){
            lock[i][j] = 0 ;
            nextBoard[i][j] = nowBoard[i][j];
        }
    }

    for ( i = 0 ; i < BOARD_WIDTH ; i++){

        endX = -1;
        for ( j = BOARD_HEIGHT - 1 ; j >= 0 ;j--){
            if (nextBoard[j][i] != 0){
                if (endX == -1){
                    if ( j != BOARD_HEIGHT - 1){
                        MoveBlock(nextBoard, j,i,BOARD_HEIGHT -1,i);
                        modified = 1;
                    }
                    endX = BOARD_HEIGHT -1;
                }
                else if (nextBoard[j][i] == nextBoard[endX][i] && lock[endX][i] == 0){
                    MoveBlock(nextBoard, j,i,endX,i);
                    lock[endX][i] = 1;
                    modified = 1;
                }
                else{
                    if ( j != endX-1){
                        MoveBlock(nextBoard, j,i,endX-1,i);
                        modified = 1;
                    }
                    endX  = endX-1;
                }
            }
        }
    }
    return modified;
}

void MoveBlock(Board board, int x1, int y1, int x2, int y2){

    if (x1 == x2 && y1 == y2)
        return;

    if ( board[x2][y2] == 0){
        board[x2][y2] = board[x1][y1];
        board[x1][y1] = 0;
    }
    else if ( board[x2][y2] == board[x1][y1]){
        /*Union two block and increase the level by one*/
        board[x2][y2] = board[x1][y1] + 1;
        board[x1][y1] = 0;
    }

}

/*
*   Update the game board and add a new random block
*/
void UpdateBoard(Board gameBoard,Board nextBoard){

    int i,j;
    int different = 0;
    for ( i = 0 ; i < BOARD_HEIGHT ; i ++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            if (gameBoard[i][j] != nextBoard[i][j])
                different = 1;
            gameBoard[i][j] = nextBoard[i][j];
        }
    }
    if (different){
        RandBlock(gameBoard);
        PrintBoard(gameBoard);
    }
}
