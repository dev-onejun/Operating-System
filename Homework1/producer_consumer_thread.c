/*
 *** Konkuk University, 2022 1th Semester, Operating System, Prof. DooHyun Kim
 * Programming Homework #1 (Producer-Consumer)

 * producer_consumer_thread.c
 * This source code is compiled by gcc 9.4.0 in Ubuntu Server 20.04.4
 * writer: WONJUN PARK
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREAD 2
#define ENOUGH_TRYS 10000
#define EMPTY -1

int buffer = EMPTY;

void *producer();
void *consumer();

int main(){
    srand((unsigned int)time(NULL));

    pthread_t thread[NUM_THREAD];

    pthread_create(&thread[0], NULL, &producer, NULL);
    pthread_create(&thread[1], NULL, &consumer, NULL);

    // only wait until consumer thread finish
    pthread_join(thread[1], NULL);

    return 0;
}

void *producer(){
    int i;

    for(i = 0; i < ENOUGH_TRYS; i++){
        if(buffer == EMPTY) buffer = rand();
        else                i--;
    }
}

void *consumer(){
    int i;

    for(i = 0; i < ENOUGH_TRYS; i++){
        while(buffer == EMPTY);    // the code can have (u)sleep function in here not to waste resource.
        printf("%d\n", buffer);
        buffer = EMPTY;
    }
}
