#pragma once
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// get thread id
int get_tid();

// run threads
int run_threads(int num_of_consumers, int sleep_limit, bool debug);
