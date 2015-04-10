#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <windows.h>
#include <omp.h>

#ifndef GLOBAL
#define GLOBAL "Global.h"
#include GLOBAL
#endif
#include "EvalueFunc.c"
#include "GameBoard.c"

#define GROUP_SIZE 50
#define CROSSOVER_RATE 50
#define MUTATION_RATE 10

FILE *fout, *record;

void NextStep(Board ,Board , int*, int,...);
int PlayGame();
double Evalue(int *);
void GeneticAlgorithm();
void InitGroup(int [][COUNT_FUNC]);
void Eliminate(int [][COUNT_FUNC]);
void Crossover(int [][COUNT_FUNC]);
void Mutation (int [][COUNT_FUNC]);
void PrintGene(int [][COUNT_FUNC]);
void Record(int [][COUNT_FUNC]);


int main(int argc, char *argv[]){

    int test[] = {1217,-183,1357,-1250,-345,193,-2,-1066,1285,-1127};

    srand(time(NULL));
    if (PLAY_MODE == 1)
        PlayGame(test);
    else
        GeneticAlgorithm();



    return 0;
}

int PlayGame(int *Parameter){

    Board gameBoard,nextBoard;
    int i;
    int maxLevel;

    InitGame(gameBoard);
    PrintBoard(gameBoard);

    while (1){
        NextStep(nextBoard,gameBoard, Parameter, COUNT_STEP, MoveLeft, MoveRight, MoveUp, MoveDown);
        UpdateBoard(gameBoard,nextBoard);
        maxLevel = FinishGame(gameBoard);
        if ( maxLevel != 0){
            break;
        }
        if (PLAY_MODE == 1)
            if ( TotalScore > 1000)
                Sleep(1000);
            else
                Sleep(100);
    }

    return maxLevel;
}

void GeneticAlgorithm(){

    int maxIter = 100;
    int i,j,k;
    int group[GROUP_SIZE][COUNT_FUNC];
    int oldParameter[COUNT_FUNC];
    int maxIndex;
    double sum,max,min,tmp;
    int count_record = 0;
    char output[300];

    /*初始族群*/
    InitGroup(group);



    for ( i = 0 ; i < maxIter ; i++){
        Eliminate(group);
        Crossover(group);
        Mutation(group);

        sum = 0;
        max = -1;
        min = 20;
        maxIndex = 0;
        #pragma omp parallel for
        for ( j = 0 ; j < GROUP_SIZE ; j++){
            tmp= Evalue( group[j]);

            if ( tmp > 10){
                fout = fopen("parameter.txt", "a");
                sprintf(output,"Parameter = ");
                for ( k = 0 ; k < COUNT_FUNC ; k++){
                    sprintf(output,"%s %5d",output, group[j][k]);
                }
                sprintf(output,"%s, Score = %lf\n",output,tmp);
                fprintf(fout,"%s",output);
                fclose(fout);
            }

            sum += tmp;

            if ( tmp > max ){
                max= tmp;
                maxIndex = j;
            }
            if ( tmp < min )
                min = tmp;
        }
        if ( i % 10 == 0)
            Record(group);
        printf("Level(%d) = %.2lf, %.2lf, %.2lf\n", i,max,sum/GROUP_SIZE,min);
        if ( max >= 10.5 && PlayGame(group[maxIndex]) >= 10.9)
            break;
    }

    printf("==Result==\n");
    for ( i = 0 ; i < COUNT_FUNC ; i++){
        printf(", %d", group[maxIndex][i]);
    }
    putchar('\n');

}

void Record(int group[][COUNT_FUNC]){

    int i,j;
    char output[10000];

    memset(output,0,10000);
    record = fopen("record.txt","w");
    for ( i = 0 ; i < GROUP_SIZE ; i++){
    	sprintf(output,"%sParameter %d :",output, i);
        for ( j = 0 ; j < COUNT_FUNC ; j++){
            sprintf(output,"%s %5d",output, group[i][j]);
        }
        sprintf(output,"%s , score = %d\n",output, PlayGame(group[i]) );
    }
    fprintf(record,"%s",output);

    fclose(record);
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
    FILE *fin;

    if ( INPUT_MODE != 0)
        fin = fopen("record.txt","r");

    for ( i = 0 ; i < GROUP_SIZE ; i++){
        for ( j = 0 ; j < COUNT_FUNC ; j++){
            if (INPUT_MODE == 0){
                if (EVALE_MODE == 0)
                    group[i][j] = rand()%21 -10;
                else
                    group[i][j] = rand()%11;
            }
            else
                fscanf(fin,"%d",&group[i][j]);
        }
    }
    if (INPUT_MODE != 0)
        fclose(fin);
    printf("Init...\n");
    PrintGene(group);
}

/*基因有一定的機率突變，突變範圍是 -50% ~ 50%*/
void Mutation(int group[][COUNT_FUNC]){

    int i,j;
    int tmp;

    #pragma omp parallel for
    for ( i = 0 ; i < GROUP_SIZE ; i++){
        if ( (rand()%100 + 1) <= MUTATION_RATE ){
            for ( j = 0 ; j < COUNT_FUNC ; j++){
                if ( EVALE_MODE == 0)
                    group[i][j] = rand()%3100 - 1500 ;
                else
                    group[i][j] = rand()%1500 ;
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

    for ( i = 0 ; i < GROUP_SIZE ; i++){
        if ( (rand()%2 == 0 && countA < GROUP_SIZE/2) || countB >= GROUP_SIZE/2 )
            index[0][countA++] = i;
        else
            index[1][countB++] = i;
    }

    if ( countA != countB)
        printf("Partition Error\n");

    #pragma omp parallel for
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
    //int offset[] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
    int offset[] = {1,4,9,16,25,36,49,64,81,100,121};
    sum = 0;

    for (i = 0; i < GROUP_SIZE ; i++){
        tmp = (int)(Evalue(group[i]));
        tmp = offset[tmp];
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

    double round = ROUND;
    double score = 0;
    int i;

    #pragma omp parallel for
    for (i = 0 ; i < ROUND ; i++){
        score += PlayGame(Parameter);
    }

    score /= round;

    return score;
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
