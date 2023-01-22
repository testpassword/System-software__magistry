#include "producer_consumer.h"

static struct {
  bool debug = false;
  int num_of_consumers;
  int sleep_limit;
} PARAMS;

static struct {
  vector<pair<pthread_t, int*>> consumers;
  int waiting_threads_num = 0;
  bool is_tasks_supplied = false;
  pthread_cond_t consumer_cond, producer_cond;
  pthread_mutex_t tasks_mutex;
  queue<int> tasks;
} STATE;

int rand_include(int max, int min = 0) {
  srand(time(NULL));
  return (max <= min || abs(max) == abs(min))
             ? 0
             : min + rand() % ((max + 1) - min);
}

int get_tid() {
  static atomic_int last_thread_id(0);
  thread_local int id = -1;
  if (id == -1) id = ++last_thread_id;
  return id;
}

void* producer_routine(void*) {
  while (!STATE.is_tasks_supplied) {
    pthread_mutex_lock(&STATE.tasks_mutex);
    if (!STATE.tasks.empty())
      pthread_cond_wait(&STATE.producer_cond, &STATE.tasks_mutex);
    string nums_raw;
    getline(cin, nums_raw);
    stringstream is(nums_raw);
    vector<int> nums_parsed((istream_iterator<int>(is)),
                            (istream_iterator<int>()));
    for (auto it : nums_parsed) STATE.tasks.push(it);
    STATE.is_tasks_supplied = true;
    pthread_mutex_unlock(&STATE.tasks_mutex);
  }
  bool polling = true;
  while (polling) {
    pthread_mutex_lock(&STATE.tasks_mutex);
    if (!STATE.tasks.empty() || STATE.waiting_threads_num != 0)
      pthread_cond_signal(&STATE.consumer_cond);
    if (STATE.tasks.empty() && STATE.waiting_threads_num == 0) polling = false;
    pthread_mutex_unlock(&STATE.tasks_mutex);
  }
  return NULL;
}

void* consumer_routine(void* arg) {
  int* partitial_sum = (int*)arg;
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  while (true) {
    bool task_completed = false;
    pthread_mutex_lock(&STATE.tasks_mutex);
    if (STATE.tasks.empty() && !STATE.is_tasks_supplied) {
      pthread_cond_signal(&STATE.producer_cond);
      STATE.waiting_threads_num++;
      pthread_cond_wait(&STATE.consumer_cond, &STATE.tasks_mutex);
      STATE.waiting_threads_num--;
    }
    if (!STATE.tasks.empty()) {
      int candidate = STATE.tasks.front();
      *partitial_sum += candidate;
      STATE.tasks.pop();
      task_completed = true;
      if (PARAMS.debug)
        cout << "(" << get_tid() << ", " << *partitial_sum << ")" << endl;
    }
    if (STATE.is_tasks_supplied && STATE.tasks.empty()) {
      pthread_mutex_unlock(&STATE.tasks_mutex);
      break;
    }
    pthread_mutex_unlock(&STATE.tasks_mutex);
    if (task_completed)
      usleep(rand_include(PARAMS.sleep_limit) *
             1000);  // convert microsecond to millis
  }
  return NULL;
}

void* consumer_interruptor_routine(void*) {
  while (!STATE.is_tasks_supplied)
    pthread_cancel(
        STATE.consumers[rand_include(0, STATE.consumers.size() - 1)].first);
  return NULL;
}

int run_threads(int num_of_consumers, int sleep_limit, bool debug) {
  PARAMS.num_of_consumers = num_of_consumers;
  PARAMS.sleep_limit = sleep_limit;
  PARAMS.debug = debug;
  pthread_t producer;
  pthread_t interruptor;
  pthread_mutex_init(&STATE.tasks_mutex, NULL);
  pthread_cond_init(&STATE.producer_cond, NULL);
  pthread_cond_init(&STATE.consumer_cond, NULL);
  pthread_create(&producer, NULL, producer_routine, NULL);
  for (auto i = 0; i < PARAMS.num_of_consumers; i++) {
    pthread_t consumer;
    int* partitial_sum = new int(0);
    pthread_create(&consumer, NULL, consumer_routine, (void*)partitial_sum);
    STATE.consumers.push_back(make_pair(consumer, partitial_sum));
  }
  pthread_create(&interruptor, NULL, consumer_interruptor_routine, NULL);
  pthread_join(interruptor, NULL);
  pthread_join(producer, NULL);
  return accumulate(STATE.consumers.begin(), STATE.consumers.end(), 0,
                    [](int acc, pair<pthread_t, int*> it) {
                      pthread_join(it.first, NULL);
                      auto psum = acc + *it.second;
                      delete it.second;
                      return psum;
                    });
}
