#include <iostream>
#include "MQueue/logobj.h"

using namespace std;

int main() {
  LOG(TRACE) << "this is a trace " << 1 << " " << 1.2;
  LOG(DEBUG) << "this is a debug " << 2 << " " << 2.2;
  LOG(INFO) << "this is a info " << 3 << " " << 3.2;
  LOG(ERROR) << "this is a error " << 4 << " " << 4.2;
  LOG(FATAL) << "this is a fatal " << 5 << " " << 5.2;

  return 0;
}
