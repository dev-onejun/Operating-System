#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 11

int main(void){
    int file_descriptor[2];
    pid_t childpid;
    char bufin[BUFSIZE] = "empty space";
    char bufout[] = "hello";
    int bytesin;

    if(pipe(file_descriptor) == -1){
        perror("Failed to create the pipe");
        return 1;
    }

    bytesin = strlen(bufin);

    childpid = fork();
    if(childpid == -1){
        perror("Failed to fork");
        return 1;
    }

    if(childpid){
        /* parent process */
        write(file_descriptor[1], bufout, strlen(bufout) + 1);
    }
    else{
        /* child process */
        bytesin = read(file_descriptor[0], bufin, BUFSIZE);
    }

    fprintf(stderr, "[%ld]:my bufin is {%.*s}, my bufout is {%s}\n", (long)getpid(), bytesin, bufin, bufout);

    return 0;
}
