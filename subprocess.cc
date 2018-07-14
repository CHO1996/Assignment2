/**
 * File: subprocess.cc
 * -------------------
 * Presents the implementation of the subprocess routine.
 */

#include "subprocess.h"
#include <iostream>
using namespace std;

subprocess_t subprocess(char *argv[], bool supplyChildInput, bool ingestChildOutput, const set<int>& openfds) throw (SubprocessException) {
    int fds1[2];
    int fds2[2];
    pipe(fds1);
    //cout << "argv[0] " << argv[0] << endl;

    subprocess_t process = {fork(),kNotInUse,kNotInUse};
    if (process.pid == 0){
        //printf("Hello... this is child process\n");

        for(auto variable : openfds){
            //cout << "variable: " << variable << endl;
            close(variable);
        }

        pipe(fds2);
        if(supplyChildInput){

            //cout << "supplyChildInput is called" << endl;

            dup2(fds1[0],STDIN_FILENO);
            dup2(fds1[1],fds1[0]);

            process.supplyfd = fds1[0];
        }
        if(ingestChildOutput){

            //cout << "ingestChildOutput is called" << endl;

            dup2(fds2[1], STDOUT_FILENO);
            dup2(fds2[0], fds1[1]);

            process.supplyfd = fds1[1];
        }
        close(fds2[0]);
        close(fds2[1]);
        execvp(argv[0],argv);
        //cout << "something went wrong here" << endl;
        printf("something went wrong here\n");
    }
    //throw SubprocessException("subprocess: not yet implemented.");
    
   
    return process;
}
