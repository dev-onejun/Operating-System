#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define BUFFER_SIZE 32
#define THE_NUMBER_OF_DATA 200
#define THE_NUMBER_OF_CONSUMER 5
#define THE_NUMBER_OF_PRODUCER 5

int circular_buffer[BUFFER_SIZE];
int in = 0, out = 0;
int counter = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t buffer_full, buffer_empty;

void* producer(void*);
void* consumer(void*);

int main(){
    srand((unsigned int)time(NULL));

    pthread_t producer_thread[THE_NUMBER_OF_PRODUCER];
    pthread_t consumer_thread[THE_NUMBER_OF_CONSUMER];

    int i;

    sem_init(&buffer_full, 0, 0);
    sem_init(&buffer_empty, 0, 0);

    for(i = 0; i < THE_NUMBER_OF_PRODUCER; i++)
        pthread_create(&producer_thread[i], NULL, &producer, (void*)((long)i));
    for(i = 0; i < THE_NUMBER_OF_CONSUMER; i++)
        pthread_create(&pconsumer_thread[i], NULL, &consumer, (void*)((long)i));

    printf("a\n");

    return 0;
}
