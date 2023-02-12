#include <cstdlib>
#include <iostream>
#include "pruned_landmark_labeling.h"
#include <chrono>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: construct_index INDEX" << endl;
    exit(EXIT_FAILURE);
  }

  PrunedLandmarkLabeling<> pll;
  if (!pll.LoadIndex(argv[1])) {
    cerr << "error: Load failed" << endl;
    exit(EXIT_FAILURE);
  }

  auto start = std::chrono::high_resolution_clock::now();
  for (int u, v; cin >> u >> v; ) {
    cout << pll.QueryDistance(u, v) << endl;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Elapsed time: " << elapsed.count() << " microseconds\n";
  exit(EXIT_SUCCESS);
}
