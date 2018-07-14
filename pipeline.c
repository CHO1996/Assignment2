/**
 * File: pipeline.c
 * ----------------
 * Presents the implementation of the pipeline routine.
 */

#include "pipeline.h"
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

void pipeline(char *argv1[], char *argv2[], pid_t pids[]) {
  //fprintf(stderr, "pipeline: not yet implemented.\n");
    int fds[2];
    pipe(fds);

    pid_t child1 = fork();
    //first child
    if(child1 == 0){
    
        pid_t child2 = fork();
        if(child2 == 0){

            dup2(0,fds[0]);
            //dup2(STDIN_FILENO,fds[0]);
            close(fds[0]);
            dup2(1,fds[1]);
            //dup2(STDOUT_FILENO,fds[1]);
            close(fds[1]);
            execvp(argv2[0],argv2);

            printf("something went wrong with the second child\n");
            return;
        }
        //execute one command
        dup2(0,fds[1]);
        //dup2(STDIN_FILENO,fds[1]);
        close(fds[1]);
        dup2(1,fds[0]);
        //dup2(STDOUT_FILENO,fds[0]);
        close(fds[0]);
        execvp(argv1[0],argv1);

        printf("something went wrong in the first child\n");
        return;
    }

    close(fds[0]);
    close(fds[1]);
    //printf("What\n");
    return;
}
