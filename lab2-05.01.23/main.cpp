#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

struct {
    bool debug = false;
    int num_of_consumers;
    int sleep_limit;
} params;
vector<pthread_t> threads_storage;
int SUM = 0;
vector<int> nums;

int rand_include(int max, int min = 0) {
    return max <= min ? 0 : min + rand() % ((max + 1) - min);
}

void* producer_routine(void* arg) {
    string nums_raw;
    getline(cin, nums_raw);
    stringstream is(nums_raw);
    vector<int> nums_parsed((std::istream_iterator<int>(is)), (std::istream_iterator<int>()));
    nums = nums_parsed;
    cout << "Я Богдан, я Богом дан" << endl;
}

void* consumer_routine(void* arg) {
    usleep(rand_include(params.sleep_limit) * 1000); // convert microsecond to millis
    cout << "А я томат, умереть за веру рад" << endl;
}

void* interruptor_routine(void* arg) {
    cout << "Я слива лиловая, спелая, садовая!" << endl;
}

int run_threads() {
    pthread_t producer;
    pthread_create(&producer, NULL, producer_routine, NULL);
    threads_storage.push_back(producer);
    pthread_t interruptor;
    pthread_create(&interruptor, NULL, interruptor_routine, NULL);
    threads_storage.push_back(interruptor);
    for (int i = 0; i < params.num_of_consumers; i++) {
        pthread_t consumer;
        pthread_create(&consumer, NULL, consumer_routine, NULL);
        threads_storage.push_back(consumer);
    }
    for (auto th: threads_storage) pthread_join(th, NULL);
    return SUM;
}

int get_tid() {
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) return -1;
    vector<string> args(argv, argv + argc);
    params.num_of_consumers = stoi(args[1]);
    params.sleep_limit = stoi(args[2]);
    params.debug = args[3].compare("-debug") == 0;
    int sum = run_threads();
    cout << "sum = " << sum << endl;
    return 0;
}