#include "data.hpp"
#include <iostream>

int main() {
  auto path = std::string{"phc.txt"};
  auto phc = PHC_index::load_file(path);
  auto core = phc.search(2,6,7);
  phc.display();

}