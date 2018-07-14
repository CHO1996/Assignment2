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

using namespace std;

struct worker {
  worker() {}
  worker(char *argv[], const set<int>& openfds = set<int>()) : sp(subprocess(argv, true, false, openfds)), available(false) {}
  subprocess_t sp;
  bool available;
};

static const size_t kNumCPUs = sysconf(_SC_NPROCESSORS_ONLN);
// restore static keyword once you start using these, commented out to suppress compiler warning
/* static */ vector<worker> workers(kNumCPUs);
/* static */ size_t numWorkersAvailable = 0;

static void markWorkersAsAvailable(int sig) {

}

// restore static keyword once you start using it, commented out to suppress compiler warning
/* static */ const char *kWorkerArguments[] = {"./factor.py", "--self-halting", NULL};
static void spawnAllWorkers() {
    //do something here

  cout << "There are this many CPUs: " << kNumCPUs << ", numbered 0 through " << kNumCPUs - 1 << "." << endl;
  for (size_t i = 0; i < kNumCPUs; i++) {

      //workers[i].available = true;
      //markWorkersAsAvailable(workers[i.sp.pid]);
      //char *point = kWorkerArguments;
      //worker(kWorkerArguments);
      char *argv[] = {const_cast<char*>(kWorkerArguments[0]),const_cast<char*>(kWorkerArguments[1]),NULL};
      workers[i] = worker(argv);
      //cout << "This is to test if subprocess changed or not " << workers[i].sp.supplyfd << endl;
      
      cout << "Worker " << workers[i].sp.pid << " is set to run on CPU " << i << "." << " is it free? " << workers[i].available  << endl;
  }
}

// restore static keyword once you start using it, commented out to suppress compiler warning
/* static */ size_t getAvailableWorker() {
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

static void waitForAllWorkers() {}

static void closeAllWorkers() {}

int main(int argc, char *argv[]) {
  signal(SIGCHLD, markWorkersAsAvailable);
  spawnAllWorkers();
  broadcastNumbersToWorkers();
  waitForAllWorkers();
  closeAllWorkers();
  return 0;
}
