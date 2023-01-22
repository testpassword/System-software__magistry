#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <producer_consumer.h>
#include <chrono>
#include <fstream>
#include <thread>

using namespace std;

// i knew, it's return cores, not threads, but I think it's ok for testing
const int MAX_THREADS = std::thread::hardware_concurrency();

TEST_CASE("test get_tid()") {
  for (auto i = 1; i <= MAX_THREADS; i++) {
    pthread_t tester;
    pthread_create(&tester, NULL,
                   [](void* arg) -> void* {
                     int* expected_tid = (int*)arg;
                     auto test_tid = get_tid();
                     CHECK(test_tid == *expected_tid);
                     return NULL;
                   },
                   &i);
    pthread_join(tester, NULL);
    pthread_join(tester, NULL);
  }
}

TEST_CASE("test run_threads()") {
  srand(time(NULL));
  auto r = []() { return 1 + (rand() % 256); };
  vector<int> nums(r());
  generate(nums.begin(), nums.end(), [r]() { return r(); });
  auto expected_sum = accumulate(nums.begin(), nums.end(), 0);
  stringstream ss;
  copy(nums.begin(), nums.end(), ostream_iterator<int>(ss, " "));
  ifstream fin("input.in");
  istringstream data(ss.str());
  cin.rdbuf(data.rdbuf());
  auto MAX_SLEEP_MS = 512;
  auto start = chrono::high_resolution_clock::now();
  auto test_sum = run_threads(8, MAX_SLEEP_MS, false);
  auto stop = chrono::high_resolution_clock::now();
  auto duration =
      chrono::duration_cast<chrono::microseconds>(stop - start).count() * 1000;
  CHECK(test_sum == expected_sum);
  CHECK(duration >= MAX_SLEEP_MS * nums.size());
}