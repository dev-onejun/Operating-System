#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void){
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

    if(childpid){   // producer(Parent Process)
        random_number = rand();
        write(file_descriptor[1], &random_number, sizeof(random_number));

        wait(NULL);
    }
    else{           // consumer(Child Process)
        read(file_descriptor[0], &random_number, sizeof(random_number));
        printf("first: %d\n", random_number);

        read(file_descriptor[0], &random_number, sizeof(random_number));
        printf("second: %d", random_number);
    }
    return 0;
}
