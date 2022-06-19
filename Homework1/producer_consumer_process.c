/*
 *** Konkuk University, 2022 1th Semester, Operating System, Prof. DooHyun Kim
 * Programming Homework #1 (Producer-Consumer)

 * producer_consumer_process.c
 * This source code is compiled by gcc 9.4.0 in Ubuntu Server 20.04.4
 * writer: WONJUN PARK
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define ENOUGH_TRYS 10000

int main(void){
    srand((unsigned int)time(NULL));

    int i;

    int file_descriptor[2];
    pid_t childpid;
    int random_number;

    if(pipe(file_descriptor) == -1){
        perror("Failed to pipe");
        return 1;
    }

    childpid = fork();
    if(childpid == -1){
        perror("Failed to fork");
        return 1;
    }

    /* execute different code both parent and child process */
    if(childpid){   // producer(Parent Process)
        for(i = 0; i < ENOUGH_TRYS; i++){
            random_number = rand();
            write(file_descriptor[1], &random_number, sizeof(random_number));
        }
        wait(NULL);
    }
    else{           // consumer(Child Process)
        for(i = 0; i < ENOUGH_TRYS; i++){
            read(file_descriptor[0], &random_number, sizeof(random_number));
            printf("%d\n", random_number);
        }
    }

    return 0;
}
