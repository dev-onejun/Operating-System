/*
 *** 건국대학교, 2022년 1학기, 운영체제, 김두현 교수님
 * 프로그래밍 숙제 #2 (Producer-Consumer)
 * 작성자: 컴퓨터공학부, 201811182, 박원준
 *
 * producer_consumer.c
 * This source code is compiled by gcc 9.4.0 in Ubuntu Server 20.04.4
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define BUFFER_SIZE             5
#define THE_NUMBER_OF_DATA      200
#define THE_NUMBER_OF_CONSUMER  5
#define THE_NUMBER_OF_PRODUCER  5

// circular_buffer: It will consume a number,
//      (THE_NUMBER_OF_PRODUCER + THE_NUMBER_OF_CONSUMER) X THE_NUMBER_OF_DATA of data.
int circular_buffer[BUFFER_SIZE];
/*** VARIABLES WHICH CAN MAKE THREAD INTERFERENCE
 * in:  The index of circular_buffer where the producer threads push integer data.
 * out: The index of circular_buffer where the consumer threads pull integer data.
 */
int in = 0, out = 0;

/*** VARIABLE WHICH CAN MAKE THREAD INTERFERENCE
 * counter: the number of data in circular_buffer.
 */
int counter = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t buffer_full, buffer_empty;

/***
 * producer(void*):
 *  The function that makes random integer values and pushes to circular_buffer.
 *  It runs on threads, and the number of them is THE_NUMBER_OF_PRODUCER.
 *  The parameter will receive a integer value which have a range 0 to THE_NUMBER_OF_PRODUCER - 1,
 *      and I use it for thread classify.
 *
 * consumer(void*):
 *  The function that pulls the integer value from circular_buffer.
 *  It runs on threads, and the number of them is THE_NUMBER_OF_CONSUMER.
 *  The usage of parameter is same as above, and its range is 0 to THE_NUMBER_OF_CONSUMER - 1.
 */
void *producer(void*);   void *consumer(void*);

int main(){
    srand((unsigned int)time(NULL));

    /***
     * producer_thread: The variable which will contain producer thread IDs.
     * consumer_thread: The variable which will contain consumer thread IDs.
     */
    pthread_t producer_thread[THE_NUMBER_OF_PRODUCER];
    pthread_t consumer_thread[THE_NUMBER_OF_CONSUMER];

    int i;

    /*** sem_init(semaphore_instance, flag, value)
     * semaphore_instance: semaphore instance which want to initialize
     * flag: 0 means a thread will use semaphore instance, non-zero means process
     * value: the number of resources
     */
    sem_init(&buffer_full, 0, BUFFER_SIZE);
    sem_init(&buffer_empty, 0, 0);

    // Create producer and consumer threads. (Below 4 lines)
    for(i = 0; i < THE_NUMBER_OF_PRODUCER; i++)
        pthread_create(&producer_thread[i], NULL, &producer, (void*)((long)i));
    for(i = 0; i < THE_NUMBER_OF_CONSUMER; i++)
        pthread_create(&consumer_thread[i], NULL, &consumer, (void*)((long)i));

    // Wait until all threads are done. (Below 4 lines)
    for(i = 0; i < THE_NUMBER_OF_PRODUCER; i++)
        pthread_join(producer_thread[i], NULL);
    for(i = 0; i < THE_NUMBER_OF_CONSUMER; i++)
        pthread_join(consumer_thread[i], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&buffer_full);  sem_destroy(&buffer_empty);

    return 0;
}

void *producer(void* arg){
    long thread_id = (long)arg;
    int random_sleep = rand() % 0xF;
    int value;
    int i, j;

    for(i = 0; i < THE_NUMBER_OF_DATA; i++){
        usleep(random_sleep);

        sem_wait(&buffer_full);
        pthread_mutex_lock(&lock);

        circular_buffer[in] = rand() % 0xFF;

        printf("[producer #%ld] produces %d\tBuffer [", thread_id, circular_buffer[in++]);
        for(j = 0; j < BUFFER_SIZE; j++)
            printf("%d, ", circular_buffer[j]);
        printf("]\n");

        in %= BUFFER_SIZE;
        counter++;
        printf("The buffer has %d data now\n", counter);

        pthread_mutex_unlock(&lock);
        sem_post(&buffer_empty);
    }

    return NULL;
}

void *consumer(void* arg){
    long thread_id = (long)arg;
    /***
     * NOTHING: just use once, to compare whether the buffer is empty.
     * USED: present the buffer space, where once have been used.
     */
    const int NOTHING = 0;
    const int USED = -1;

    int random_sleep = rand() % 0xF;
    int value;
    int i, j;

    for(i = 0; i < THE_NUMBER_OF_DATA; i++){
        usleep(random_sleep);

        sem_wait(&buffer_empty);
        pthread_mutex_lock(&lock);

        printf("[consumer #%ld] consumes %d\tBuffer [", thread_id, circular_buffer[out]);
        circular_buffer[out++] = USED;
        for(j = 0; j < BUFFER_SIZE; j++)
            printf("%d, ", circular_buffer[j]);
        printf("]\n");

        out %= BUFFER_SIZE;
        counter--;
        printf("The buffer has %d data now\n", counter);

        pthread_mutex_unlock(&lock);
        sem_post(&buffer_full);
    }

    return NULL;
}
