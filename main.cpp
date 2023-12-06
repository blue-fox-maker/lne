#include <iostream>
#include "data.hpp"
#include "test/unit_test.hpp"
#include "temporal.hpp"
using namespace Temporal;
int main()
{
  // auto path = std::string{"phc.txt"};
  // auto phc = PHC_index::load_file(path);
  // auto core = phc.search(2, 6, 7);
  // phc.display();
  unit_test<Intervals>();
  return 0;
}