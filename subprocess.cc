/**
 * File: subprocess.cc
 * -------------------
 * Presents the implementation of the subprocess routine.
 */

#include "subprocess.h"
using namespace std;

subprocess_t subprocess(char *argv[], bool supplyChildInput, bool ingestChildOutput, const set<int>& openfds) throw (SubprocessException) {
  throw SubprocessException("subprocess: not yet implemented.");
}
