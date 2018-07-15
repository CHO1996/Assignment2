#include <cassert>
#include <ctime>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>
#include "subprocess.h"

//signals
//{atomic}
//
//sigprocmask(SIG_SETMASK, &old, &old2);
//
//sigsuspend()
//
//~some signal~
//
//handle the signal
//
//sigprocmask(SIG_SETMASK, &old2, ...)
//
//{/atomic} 

using namespace std;

struct worker {
  worker() {}
  worker(char *argv[], const set<int>& openfds = set<int>()) : sp(subprocess(argv, true, false, openfds)), available(false) {}
  subprocess_t sp;
  bool available;
};

static const size_t kNumCPUs = sysconf(_SC_NPROCESSORS_ONLN);
// restore static keyword once you start using these, commented out to suppress compiler warning
static vector<worker> workers(kNumCPUs);
static size_t numWorkersAvailable = 0;

static void markWorkersAsAvailable(int sig) {
    int check = 0;
    while(true){
        pid_t pid = waitpid(-1, NULL, WNOHANG);
        for(size_t i = 0; i < kNumCPUs; i++){
            if(workers[i].sp.pid == sig){
               check++;
               workers[i].available = true;
               numWorkersAvailable++;
            }
        }
        //cout << " This is pid for waitpid : " << pid << endl;
        if(check == 0){
            break;
            //cout << "This is the sig value : " << sig << endl;
            //cout << "Something went wrong with markWorkersAsAvailable" << endl;
        }
    }
}

// restore static keyword once you start using it, commented out to suppress compiler warning
static const char *kWorkerArguments[] = {"./factor.py", "--self-halting", NULL};
static void spawnAllWorkers() {

  cout << "There are this many CPUs: " << kNumCPUs << ", numbered 0 through " << kNumCPUs - 1 << "." << endl;
  for (size_t i = 0; i < kNumCPUs; i++) {
      char *argv[] = {const_cast<char*>(kWorkerArguments[0]),const_cast<char*>(kWorkerArguments[1]),NULL};
      workers[i] = worker(argv);
      cout << "Worker " << workers[i].sp.pid << " is set to run on CPU " << i << "." << endl;
  }
}

// restore static keyword once you start using it, commented out to suppress compiler warning
/* static */ size_t getAvailableWorker() {
    while(numWorkersAvailable < 1){
        sigset_t old;
        sigset_t old2;
        sigprocmask(SIG_SETMASK, &old, &old2);
        sigsuspend();
        signal(SIGCHLD,markWorkersAvailable);
        for(size_t i = 0; i < kNumCPUs; i++){
            if(workers[i].sp.pid == old2){
                return old2;
            }
        }
        sigprocmask(SIG_SETMASK,&old2,NULL);


    }
  return 0;
}

static void broadcastNumbersToWorkers() {
  while (true) {
    string line;
    getline(cin, line);
    if (cin.fail()) break;
    size_t endpos;
    /* long long num = */ stoll(line, &endpos);
    if (endpos != line.size()) break;
    // you shouldn't need all that many lines of additional code
  }
}

static void waitForAllWorkers() {
    bool all_true;
   while(true){
        for(size_t i = 0; i < kNumCPUs; i++){
            if(workers[i].avilable == false){
                if(waitpid(workers[i].sp.pid, NULL, WIFSTOPPED) == -1) 
                    //I think somewhere here is where I need to actually check to see if all the child is halted and available.
            }
        }

   }
}

static void closeAllWorkers() {
    for(size_t i = 0; i < kNumCPUs; i++){
        //somehow I need to signal EOF to each workers 
        close(workers[i].sp.pid);
        workers[i].sp.supplyfd = kNotInUse;
        workers[i].sp.ingestfd = kNotInUse;
        numWorkersAvilable--;
    }
}

int main(int argc, char *argv[]) {
  signal(SIGCHLD, markWorkersAsAvailable);
  spawnAllWorkers();
  broadcastNumbersToWorkers();
  waitForAllWorkers();
  closeAllWorkers();
  return 0;
}
