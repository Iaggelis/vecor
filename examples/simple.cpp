#include "../vecor.hpp"
#include <iostream>

auto main(int argc, char* argv[]) -> int {

  Vecor::Vec<int> veci;

  veci.push_back(0);
  veci.push_back(1);
  veci.push_back(3);
  veci.push_back(4);
  veci.push_back(5);
  veci.push_back(6);
  veci.push_back(7);
  veci.push_back(8);
  veci.push_back(9);
  veci.push_back(10);
  veci.push_back(11);
  veci.push_back(12);

  auto temp = veci.filter([](const auto& x) { return x % 2 == 0; });

  std::cout << temp << '\n';
  return 0;
}
