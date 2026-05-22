#include "map_utils_test.c"
#include "sample_lib_test.c"
#include "utils.h"
#include <stdio.h>

int main() {
  {
    // Call all test modules you want to run here
    sample_lib_test();
    map_utils_test();
  }
  allsuccess();
}