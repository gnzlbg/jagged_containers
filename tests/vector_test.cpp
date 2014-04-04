#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/tuple/tuple.hpp>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "jagged/vector.hpp"

/// \test scattered::vector tests
TEST_CASE("Test jagged::vector<T>", "[jagged][vector]") {

  jagged::vector<int> frc(jagged::reserve, 10, 10);

  std::vector<int> a = {0};
  std::vector<int> b = {1, 2, 3};
  std::vector<int> c = {};
  std::vector<int> d = {4, 5};
  std::vector<int> e = {6};
  frc.push_back(a);
  frc.push_back(b);
  frc.push_back(c);
  frc.push_back(d);
  frc.push_back(e);

  for (auto r : frc) {
    std::cout << "r: ";
    for (auto i : r) {
      std::cout << i << " ";
    }
    std::cout << "\n";
  }
}
