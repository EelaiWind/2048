#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef GLOBAL
#define GLOBAL "global.h"
#include GLOBAL
#endif

int main(){

    int i,j;
    char num[10] ;
    char command[1000];

    srand(time(NULL));

    for (i = 0; i < 1000 ; i++){
        sprintf(command,"Conscious.exe");
        for ( j = 0 ; j < COUNT_FUNC ;j++){
            sprintf(num," %d",rand() % 201 - 100);
            strcat(command,num);
        }
        printf("%d > %s\n",i,command);
        system(command);
    }

    return 0;
}
