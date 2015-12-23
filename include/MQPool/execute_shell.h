#ifndef MQPOOL_INCLUDE_EXECUTE_SHELL_H_
#define MQPOOL_INCLUDE_EXECUTE_SHELL_H_

#include "object.h"
#include <string>

using std::string;
/*
 * 其实这个功能一个non-member函数就能做
 */
class ExecuteShell : public Object {
 public:
  ExecuteShell();
  virtual ~ExecuteShell();
  void Dump() const;

  int execute(const char* command, string& output);
  int execute(const string& command, string& output);

};

#endif /* MQPOOL_INCLUDE_EXECUTE_SHELL_H_ */
