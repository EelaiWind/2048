#include <stdio.h>
#include <stdarg.h>
#include<string.h>

#ifndef GLOBAL
#define GLOBAL "global.h"
#include GLOBAL
#endif

int CalculateScore(Board, int*,int, ...);

int MaxLevel(Board);
int Average(Board);
int MergeCount(Board);
int MergeDis(Board);
int MergeLevelUp(Board);
int EmptyCount(Board);
int LongestSequance(Board);
int MinEdge(Board);

/*int main(){

    int i,j;
    Board gameBoard= {{1,0,3,0},{0,3,0,5},{3,0,5,0},{0,5,0,7}};
    int weightArray[] = {1,1,1,1,1,1,1,1};
    int score;

    for (i = 0 ; i < BOARD_HEIGHT ; i++){
        for (j = 0 ; j < BOARD_WIDTH ; j++){
            printf(" %2d",gameBoard[i][j]);
        }
        putchar('\n');
    }

    score = Frame_CalculateScore(gameBoard, weightArray);

    printf("score = %d\n",score);
    return 0;
}*/

int Frame_CalculateScore(Board gameBoard, int* weightArray){

    return CalculateScore(gameBoard, weightArray,COUNT_FUNC, MaxLevel, Average, MergeCount, MergeDis,MergeLevelUp, EmptyCount,LongestSequance, MinEdge);
}

int CalculateScore(Board gameBoard, int* weightArray,int count_func, ...){

    int i;
    int score = 0;

    va_list funcList;
    va_start(funcList, count_func);

    for (i = 0 ; i < count_func ; i++){
        score += weightArray[i]*va_arg(funcList,ScoreFuncPtr)(gameBoard);
    }

    va_end(funcList);
    if (DEBUG_MODE == 1) printf("Score = %d\n",score);
    return score;
}

int MaxLevel(Board gameBoard){

    int i,j;
    int maxLevel = 0;

    for ( i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            if (gameBoard[i][j] > maxLevel )
                maxLevel = gameBoard[i][j];
        }
    }

    if (DEBUG_MODE == 1) printf("Max Level = %d\n", maxLevel);
    return maxLevel;
}

int Average(Board gameBoard){

    int i,j;
    int sum = 0;
    int count = 0;
    int avr;

    for ( i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            sum += gameBoard[i][j];
            if (gameBoard[i][j] > 0)
                count += 1;
        }
    }
    avr = sum/count;
    if (DEBUG_MODE == 1) printf("Level average = %d\n", avr);
    return avr;
}

int MergeCount(Board gameBoard){

    int i,j;
    int count_merge = 0;

    for (i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            if ( gameBoard[i][j] > 0){
                if ( i > 0 && gameBoard[i - 1][j] > 0 ){
                    count_merge += (gameBoard[i - 1][j] == gameBoard[i][j]);
                }
                if ( i < BOARD_HEIGHT -1 && gameBoard[i + 1][j] > 0){
                    count_merge += (gameBoard[i + 1][j] == gameBoard[i][j]) ;
                }
                if ( j > 0 && gameBoard[i][j - 1] > 0){
                    count_merge += (gameBoard[i][j - 1] == gameBoard[i][j]);
                }
                if ( j < BOARD_WIDTH -1 && gameBoard[i][j + 1] > 0){
                    count_merge += (gameBoard[i][j + 1] == gameBoard[i][j]);
                }
            }
        }
    }
    count_merge /= 2;
    if (DEBUG_MODE == 1) printf("There are %d chance to level up\n",count_merge);
    return count_merge;
}
/*
    相鄰2個block的level差距和
*/
int MergeDis(Board gameBoard){

    int i,j;
    int dis_merge = 0;
    int count = 0;

    for (i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            if ( gameBoard[i][j] > 0){
                count += 1;
                if ( i > 0 && gameBoard[i - 1][j] > 0 ){
                    dis_merge += abs(gameBoard[i - 1][j] - gameBoard[i][j]);
                }
                if ( i < BOARD_HEIGHT -1 && gameBoard[i + 1][j] > 0){
                    dis_merge += abs(gameBoard[i + 1][j] - gameBoard[i][j]) ;
                }
                if ( j > 0 && gameBoard[i][j - 1] > 0){
                    dis_merge += abs(gameBoard[i][j - 1] - gameBoard[i][j]);
                }
                if ( j < BOARD_WIDTH -1 && gameBoard[i][j + 1] > 0){
                    dis_merge += abs(gameBoard[i][j + 1] - gameBoard[i][j]);
                }
            }
        }
    }

    if (count > 0)
        dis_merge /= count;
    if (DEBUG_MODE == 1) printf("Distance between every block pair = %d\n",dis_merge);
    return dis_merge;
}

int EmptyCount(Board gameBoard){

    int i,j;
    int count_empty = 0;

    for ( i = 0 ; i < BOARD_HEIGHT ; i++){
        for (j = 0; j < BOARD_WIDTH ; j++){
            if ( gameBoard[i][j] == 0)
                count_empty += 1;
        }
    }

    if (DEBUG_MODE == 1) printf("There are %d empty block\n", count_empty);
    return count_empty;
}

int LongestSequance(Board gameBoard){

    int longestSeq = 0 ,tmp_longest;
    int i,j,lv;
    int m,n;
    int level;
    int size[12];
    int position[12][16];
    Board sequance;

    memset(size,0,sizeof(size));

    for ( i = 0; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            level = gameBoard[i][j];
            if ( level == 0)
                sequance[i][j] = 0;
            else
                sequance[i][j] = 1;

            if ( level > 1)
                position[level][ size[level]++ ] = i * BOARD_HEIGHT + j;
        }
    }

    for ( lv = 2; lv <= 11 ; lv++){
        for (j = 0 ; j < size[lv] ; j++){

            m = position[lv][j] / BOARD_HEIGHT;
            n = position[lv][j] % BOARD_HEIGHT;

            tmp_longest = 0;
            if ( m > 0 && gameBoard[m-1][n] == lv-1)
                if ( sequance[m-1][n] > tmp_longest)
                    tmp_longest = sequance[m-1][n];
            if ( m < BOARD_HEIGHT - 1 && gameBoard[m+1][n] == lv-1)
                if ( sequance[m+1][n] > tmp_longest)
                    tmp_longest = sequance[m+1][n];
            if ( n > 0 && gameBoard[m][n-1] == lv-1)
                if ( sequance[m][n-1] > tmp_longest)
                    tmp_longest = sequance[m][n-1];
            if ( n < BOARD_WIDTH - 1 && gameBoard[m][n+1] == lv-1)
                if ( sequance[m][n+1] > tmp_longest)
                    tmp_longest = sequance[m][n+1];
            sequance[m][n] = tmp_longest+1;
            if (tmp_longest+1 > longestSeq)
                longestSeq = tmp_longest+1;
        }
    }

    if (DEBUG_MODE == 1) printf("Longest Sequance = %d\n",longestSeq);
    return longestSeq;
}

/*
    計算相鄰的block是否level相差1
*/
int MergeLevelUp(Board gameBoard){

    int i,j;
    int count_levelUp = 0;

    for (i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++){
            if ( gameBoard[i][j] > 0){
                if ( i > 0 && gameBoard[i - 1][j] > 0 ){
                    count_levelUp += (gameBoard[i - 1][j] + 1 == gameBoard[i][j]);
                }
                if ( i < BOARD_HEIGHT -1 && gameBoard[i + 1][j] > 0){
                    count_levelUp += (gameBoard[i + 1][j] + 1 == gameBoard[i][j]) ;
                }
                if ( j > 0 && gameBoard[i][j - 1] > 0){
                    count_levelUp += (gameBoard[i][j - 1] + 1 == gameBoard[i][j]);
                }
                if ( j < BOARD_WIDTH -1 && gameBoard[i][j + 1] > 0){
                    count_levelUp +=  (gameBoard[i][j + 1] + 1 == gameBoard[i][j]);
                }
            }
        }
    }
    if (DEBUG_MODE == 1) printf("Merge LevelUp = %d\n",count_levelUp);
    return count_levelUp;
}


/*
    邊緣的數字越小越好
    平均值
*/
int MinEdge(Board gameBoard){

    int i,j;
    int valid;
    int edgeSum = 0;
    int count = 0;

    for ( i = 0 ; i < BOARD_HEIGHT ; i++){
        for ( j = 0 ; j < BOARD_WIDTH ; j++ ){
            if ( gameBoard[i][j] == 0)
                continue;

            valid = 0;
            if ( i > 0 && gameBoard[i - 1][j] == 0 ){
                valid =1;
            }
            else if ( i < BOARD_HEIGHT -1 && gameBoard[i + 1][j] == 0){
                valid =1;
            }
            else if ( j > 0 && gameBoard[i][j - 1] == 0){
                valid =1;
            }
            else if ( j < BOARD_WIDTH -1 && gameBoard[i][j + 1] == 0){
                valid =1;
            }

            if (valid == 1){
                edgeSum += gameBoard[i][j] ;
                count += 1;
            }
        }
    }
    if (count > 0)
        edgeSum /= count;
    if (DEBUG_MODE == 1) printf("Average Edge Sum = %d\n", edgeSum);
    return edgeSum;
}

