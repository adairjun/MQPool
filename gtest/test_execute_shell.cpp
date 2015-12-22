#include <iostream>
#include "MQPool/execute_shell.h"

using namespace std;

int main() {
  string output = "";
  ExecuteShell myshell;
  string hello = "hello";
  myshell.execute("touch " + hello, output); 
  cout << output << endl;
  return 0;
}
