#include <iostream>
#include <string>
#include <vector>
#include "producer_consumer.h"

int main(int argc, char* argv[]) {
  if (argc < 3) return -1;
  std::vector<std::string> args(argv, argv + argc);
  std::cout << run_threads(
                   stoi(args[1]), stoi(args[2]),
                   (args.size() >= 4 ? args[3] : "").compare("-debug") == 0)
            << std::endl;
  return 0;
}
