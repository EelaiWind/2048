#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef GLOBAL
#define GLOBAL "global.h"
#include GLOBAL
#endif
#include "Evalue.c"
#include "GameBoard_windows.c"

#define GROUP_SIZE 100
#define CROSSOVER_RATE 50
#define MUTATION_RATE 10

void NextStep(Board ,Board , int*, int,...);
int PlayGame();
double Evalue(int *);
void GeneticAlgorithm();
void InitGroup(int [][COUNT_FUNC]);
void Eliminate(int [][COUNT_FUNC]);
void Crossover(int [][COUNT_FUNC]);
void Mutation (int [][COUNT_FUNC]);
void PrintGene(int [][COUNT_FUNC]);

int main(int argc, char *argv[]){

    int test[] = {-1,-1,7,-3,-2,9,4,-7};
    //GeneticAlgorithm();
    PlayGame(test);
    return 0;
}

int PlayGame(int *Parameter){

    Board gameBoard,nextBoard;
    int i;

    InitGame(gameBoard);
    PrintBoard(gameBoard);

    /*for (i = 0 ; i < COUNT_FUNC ;i++)
        printf(" %3d", Parameter[i]);
    putchar('\n');*/
    maxLevel = 1;
    while (1){
        NextStep(nextBoard,gameBoard, Parameter, COUNT_STEP, MoveLeft, MoveRight, MoveUp, MoveDown);
        UpdateBoard(gameBoard,nextBoard);
        if ( FinishGame(gameBoard) == 1){
            if (PLAY_MODE == 1)
                system("PAUSE");
            break;
        }
    }

    return maxLevel;
}

void GeneticAlgorithm(){

    int maxIter = 2147483640;
    int i,j;
    int group[GROUP_SIZE][COUNT_FUNC];
    double sum;

    /*初始族群*/
    InitGroup(group);

    for ( i = 0 ; i < maxIter ; i++){
        Eliminate(group);
        Crossover(group);
        Mutation(group);

        sum = 0;
        for ( j = 0 ; j < GROUP_SIZE ; j++){
            sum += Evalue( group[j]);
        }
        printf("Average Level(%d) = %.2lf\n", i,sum/GROUP_SIZE);
        if ( sum >= 10.5 * GROUP_SIZE)
            break;
    }

    printf("==Result==\n");
    PrintGene(group);

}

void PrintGene(int group[][COUNT_FUNC]){
    int i,j;

    for ( i = 0 ; i < GROUP_SIZE ; i++){
        printf("Parameter %d :",i);
        for (j = 0 ; j < COUNT_FUNC ; j++){
            printf(" %3d", group[i][j]);
        }
        putchar('\n');
    }
}

void InitGroup(int group[][COUNT_FUNC]){

    int i,j;

    srand(time(NULL));
    for ( i = 0 ; i < GROUP_SIZE ; i++){
        for ( j = 0 ; j < COUNT_FUNC ; j++){
            group[i][j] = rand()%21 -10;
        }
    }
    printf("Init...\n");
    PrintGene(group);
}

/*基因有一定的機率突變，突變範圍是 -50% ~ 50%*/
void Mutation(int group[][COUNT_FUNC]){

    int i,j;
    int tmp;
    srand(time(NULL));

    for ( i = 0 ; i < GROUP_SIZE ; i++){
        for ( j = 0 ; j < COUNT_FUNC ; j++){
            if ( (rand()%100 + 1) <= MUTATION_RATE ){
                /*tmp = abs(group[i][j]);
                tmp += 10;
                if (tmp == 0)
                    group[i][j] = rand()%21 - 10;
                else
                    group[i][j] += (rand()%tmp) -  (tmp/2);*/
                    group[i][j] = rand()%3100 - 1500 ;
            }
        }
    }
    if (GA_DEBUG_MODE == 1){
        printf("==Mutation==\n");
        PrintGene(group);
    }
}

void Crossover(int group[][COUNT_FUNC]){

    int i,j;
    int copy[GROUP_SIZE][COUNT_FUNC];
    int index[2][GROUP_SIZE/2];
    int countA,countB;
    int crossoverPoint;

    for (i = 0 ; i < GROUP_SIZE ; i++){
        for ( j = 0 ; j < COUNT_FUNC ; j++){
            copy[i][j] = group[i][j];
        }
    }

    countA = 0;
    countB = 0;

    srand(time(NULL));

    for ( i = 0 ; i < GROUP_SIZE ; i++){
        if ( (rand()%2 == 0 && countA < GROUP_SIZE/2) || countB >= GROUP_SIZE/2 )
            index[0][countA++] = i;
        else
            index[1][countB++] = i;
    }

    if ( countA != countB)
        printf("Partition Error\n");

    for ( i = 0 ; i < GROUP_SIZE/2 ; i++){
        if ( (rand()%100 + 1) <= CROSSOVER_RATE ){
            crossoverPoint = rand()%(COUNT_FUNC-2) + 1;
            /**printf(" %d <-> %d at (%d)\n", index[0][i], index[1][i], crossoverPoint);*/
            for ( j = 0 ; j < COUNT_FUNC ; j++){
                if ( j < crossoverPoint){
                    group[ index[0][i] ][j] = copy[ index[0][i] ][j];
                    group[ index[1][i] ][j] = copy[ index[1][i] ][j];
                }
                else{
                    group[ index[0][i] ][j] = copy[ index[1][i] ][j];
                    group[ index[1][i] ][j] = copy[ index[0][i] ][j];
                }
            }
        }
    }
    if (GA_DEBUG_MODE == 1){
        printf("==Crossover==\n");
        PrintGene(group);
    }
}

/*依照每組基因的分數來篩選基因，分數越高越有機會被選上*/
void Eliminate( int group[][COUNT_FUNC]){

    int probability[GROUP_SIZE];
    int copy[GROUP_SIZE][COUNT_FUNC];

    int i,j;
    int sum, tmp;
    int index;
    int random;

    sum = 0;
    for (i = 0; i < GROUP_SIZE ; i++){
        tmp = (int)(Evalue(group[i]) + 0.5);
        sum += tmp;

        if ( i == 0 )
            probability[i] = tmp;
        else
            probability[i] = probability[i-1] + tmp;
    }

    for (i = 0 ; i < GROUP_SIZE ; i++){
        for ( j = 0 ; j < COUNT_FUNC ; j++){
            copy[i][j] = group[i][j];
        }
    }

    srand(time(NULL));

    for ( i = 0 ; i < GROUP_SIZE ; i++){
        random = rand()%sum + 1;
        for ( j = 0 ; j < GROUP_SIZE ; j++){
            if ( probability[j] >= random){
                index = j;
                break;
            }
        }
        for ( j = 0 ; j < COUNT_FUNC ;j++)
            group[i][j] = copy[index][j];
    }

    if ( i != GROUP_SIZE){
        printf("Eliminate Error\n");
    }
    if (GA_DEBUG_MODE == 1){
        printf("==Eliminate==\n");
        PrintGene(group);
    }
}

/*計算每組基因的分數*/
double Evalue(int *Parameter){

    int round = 10;
    double score = 0;
    int i;

    for (i = 0 ; i < round ; i++){
        score += PlayGame(Parameter);
    }
    return score/round;
}

void NextStep(Board nextBoard,Board gameBoard,int *weightParameter, int count_step,...){

    int i;
    int maxScore, tmp_score;
    int modified;
    Movement maxChoice, tmp_choice;
    va_list moveStep;
    va_start(moveStep, count_step);

    maxScore = -2147483647;
    for ( i = 0 ; i< count_step ; i ++){
        tmp_choice= va_arg(moveStep, Movement);
        modified =tmp_choice(nextBoard, gameBoard);
        if (modified == 1){
            tmp_score = Frame_CalculateScore(nextBoard, weightParameter);
            if ( tmp_score > maxScore){
                maxScore = tmp_score;
                maxChoice = tmp_choice;
            }
        }
    }

    maxChoice(nextBoard,gameBoard);

}
