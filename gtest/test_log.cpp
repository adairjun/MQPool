#include <iostream>
#include "MQPool/logobj.h"

using namespace std;

int main() {
  /*LOG(TRACE) << "this is a trace" << endl;
  LOG(DEBUG) << "this is a debug" << endl;
  LOG(INFO) << "this is a info" << endl;
  LOG(ERROR) << "this is a error" << endl;
  LOG(FATAL) << "this is a fatal" << endl;*/
  LogObj g_log;
  g_log.MakeLogByStream(__FILE__, __LINE__, LogObj::TRACE) << "this is a trace";
  g_log.Dump();

  return 0;
}
