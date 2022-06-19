#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREAD 2
#define ENOUGH_TRYS 10000

int buffer;

void *producer();
void *consumer();

int main(){
    srand((unsigned int)time(NULL));

    pthread_t thread[NUM_THREAD];

    pthread_create(&thread[0], NULL, &producer, NULL);
    pthread_create(&thread[1], NULL, &consumer, NULL);

    pthread_join(thread[1], NULL);

    return 0;
}

void *producer(){
    int i;

    for(i = 0; i<ENOUGH_TRYS; i++){
        buffer = rand();

        usleep(200001); // 200 ms   // timeslice is between 10ms and 200ms
    }
}

void *consumer(){
    int i;

    for(i = 0; i < ENOUGH_TRYS; i++){
        printf("%d\n", buffer);

        usleep(200001);
    }
}
